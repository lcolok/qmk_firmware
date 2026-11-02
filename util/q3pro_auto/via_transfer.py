#!/usr/bin/env python3
"""Q3 Pro 的 VIA 备份、恢复与逐字节校验；不打印宏内容。"""
import argparse
import json
import os
import time
from pathlib import Path
import hid


class Via:
    def __enter__(self):
        matches = [d for d in hid.enumerate(0x3434, 0x0630)
                   if d['usage_page'] == 0xFF60 and d['usage'] == 0x61]
        if len(matches) != 1:
            raise RuntimeError(f'需要唯一 Q3 Pro raw HID 接口，实际 {len(matches)} 个')
        self.h = hid.device()
        try:
            self.h.open_path(matches[0]['path'])
            self.h.set_nonblocking(1)
            if self.call([1])[1:3] != bytes([0, 12]):
                raise RuntimeError('VIA 协议不是 12')
        except BaseException:
            self.h.close()
            raise
        return self

    def __exit__(self, *args):
        self.h.close()

    def call(self, payload, echo=1):
        if not 1 <= len(payload) <= 32:
            raise ValueError('无效包长')
        # hidapi 接口要求 report ID；设备收到后续 32 字节。
        packet = bytes([0] + payload + [0] * (32 - len(payload)))
        drain_until = time.monotonic() + 0.2
        while self.h.read(64):
            if time.monotonic() >= drain_until:
                raise RuntimeError('输入队列持续繁忙')
        if self.h.write(packet) != len(packet):
            raise RuntimeError('HID 写入不完整')
        deadline = time.monotonic() + 3
        while time.monotonic() < deadline:
            response = bytes(self.h.read(64))
            if response:
                if len(response) != 32 or response[:echo] != bytes(payload[:echo]):
                    raise RuntimeError(f'命令 {payload[0]:02x} 收到无效应答')
                return response
            time.sleep(0.005)
        raise TimeoutError(f'命令 {payload[0]:02x} 超时')

    def read_buffer(self, cmd, size):
        result = bytearray()
        for offset in range(0, size, 28):
            count = min(28, size - offset)
            reply = self.call([cmd, offset >> 8, offset & 255, count], 4)
            result.extend(reply[4:4 + count])
        return list(result)

    def write_buffer(self, cmd, values, base=0):
        for start in range(0, len(values), 28):
            block = values[start:start + 28]
            offset = base + start
            self.call([cmd, offset >> 8, offset & 255, len(block)] + block, 4)

    def snapshot(self):
        layers = self.call([0x11])[1]
        if not 1 <= layers <= 32:
            raise RuntimeError('设备报告无效层数')
        size = int.from_bytes(self.call([0x0D])[1:3], 'big')
        if size > 2048:
            raise RuntimeError('宏区大小超出预期')
        keys = self.read_buffer(0x12, layers * 216)
        encoders = {}
        for layer in range(layers):
            encoders[f'layer{layer}'] = {
                name: int.from_bytes(self.call([0x14, layer, 0, flag], 4)[4:6], 'big')
                for name, flag in [('ccw', 0), ('cw', 1)]}
        return dict(layers=layers,
                    keymap={f'layer{i}': keys[i*216:(i+1)*216] for i in range(layers)},
                    encoder_map=encoders, macro_buffer_size=size,
                    macro_buffer=self.read_buffer(0x0E, size),
                    layout_options=int.from_bytes(self.call([2, 2], 2)[2:6], 'big'))


def validate(data):
    if data['layers'] != 8 or data['macro_buffer_size'] != 250:
        raise ValueError('恢复源必须为 8 层、250 字节宏区')
    for layer in range(8):
        values = data['keymap'][f'layer{layer}']
        if len(values) != 216 or any(type(x) is not int or not 0 <= x <= 255 for x in values):
            raise ValueError('键位字节无效')
        for name in ('ccw', 'cw'):
            value = data['encoder_map'][f'layer{layer}'][name]
            if type(value) is not int or not 0 <= value <= 65535:
                raise ValueError('旋钮键码无效')
    macro = data['macro_buffer']
    if len(macro) != 250 or any(type(x) is not int or not 0 <= x <= 255 for x in macro) or macro[-1] != 0:
        raise ValueError('宏缓冲区无效')
    if type(data['layout_options']) is not int or not 0 <= data['layout_options'] <= 0xFFFFFFFF:
        raise ValueError('布局选项无效')


def save_new(path, data):
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    with os.fdopen(fd, 'w') as f:
        json.dump(data, f, indent=2)
        f.write('\n')


def compare(expected, actual):
    for field in ('layers', 'keymap', 'encoder_map', 'macro_buffer_size', 'macro_buffer', 'layout_options'):
        if expected[field] != actual[field]:
            raise RuntimeError(f'读回校验失败：{field}；已保留写入前快照')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('action', choices=['backup', 'restore', 'verify'])
    p.add_argument('path', type=Path)
    p.add_argument('--before', type=Path)
    args = p.parse_args()
    source = None
    if args.action != 'backup':
        source = json.loads(args.path.read_text())
        validate(source)
    with Via() as via:
        current = via.snapshot()
        if args.action == 'backup':
            save_new(args.path, current)
            print(f'已备份：{current["layers"]} 层；宏区 {current["macro_buffer_size"]} 字节')
        elif args.action == 'verify':
            compare(source, current)
            print('PASS：8 层键位、16 个旋钮键码、250 字节宏、布局选项全部一致')
        else:
            if current['layers'] != 8 or current['macro_buffer_size'] != 250:
                raise RuntimeError('设备尚未运行预期的 8 层固件，拒绝写入')
            if not args.before:
                raise ValueError('恢复必须提供 --before 保存写入前状态')
            save_new(args.before, current)
            # 先禁止执行宏，全部写完再清除末字节标记。
            via.write_buffer(0x0F, [255], 249)
            keys = [x for i in range(8) for x in source['keymap'][f'layer{i}']]
            via.write_buffer(0x13, keys)
            for layer in range(8):
                for name, flag in [('ccw', 0), ('cw', 1)]:
                    value = source['encoder_map'][f'layer{layer}'][name]
                    via.call([0x15, layer, 0, flag, value >> 8, value & 255], 6)
            via.call([3, 2] + list(source['layout_options'].to_bytes(4, 'big')), 6)
            via.write_buffer(0x0F, source['macro_buffer'][:-1])
            via.write_buffer(0x0F, [0], 249)
            compare(source, via.snapshot())
            print('PASS：恢复完成，全部目标数据逐字节读回一致')


if __name__ == '__main__':
    main()
