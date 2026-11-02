# Q3 Pro 自动层选择与 VIA 恢复

适用固件：`keychron/q3_pro/ansi_encoder_se:via`。Mac/Win 开关选择策略：Mac 为手动模式（默认层 0），Win 为自动模式（主机通过 VIA 请求默认层）。Cable/BT 连接开关保持原用途。其他变体通过 weak hook 保留原有 DIP 行为。

## 协议与容量

32 字节 VIA 报告，custom channel `0`，value ID `0x10`：

- SET：`07 00 10 <layer> <lease_seconds>`，有效层为 0–7；租约 0 表示默认 7 秒。手动模式或非法层返回 `FF`。
- GET：`08 00 10`，从 byte3 起返回 `[mode,last_layer,lease_remaining_s,signal_present]`。
- `last_layer` 是最后接受的请求，并非当前实际默认层。超时后它可能保留原值。
- companion 每 2 秒续租；租约失效后固件设置默认层 0。拨回手动模式撤销旧信号。
- 使用 32 位毫秒计时器，支持 SET 的 1–255 秒租约。剩余秒数向下取整，因此刚发出的 7 秒租约可能读到 6。

EEPROM：34 字节 QMK 配置 + 4 字节 VIA 元数据 + 1728 字节键位（8×6×18×2）+ 32 字节旋钮 + 250 字节宏 = 2048 字节。修改层数会移动旋钮和宏的地址，必须备份后重新恢复，不能假设旧数据自动迁移。

## 构建与主机测试

在仓库根目录：

```sh
make keychron/q3_pro/ansi_encoder_se:via -j4
sh util/q3pro_auto/test-policy.sh
swiftc util/q3pro_auto/companion.swift -o /tmp/q3pro-companion
/tmp/q3pro-companion --probe
```

策略测试直接编译 `ansi_encoder_se.c`，模拟时钟和 `default_layer_set`，开启 ASan/UBSan，覆盖手动拒绝、层数边界、7 秒到期、255 秒租约、续租、模式切换、32 位回绕和短包。它不替代硬件按键验收。

`companion --probe` 只读取一次状态并关闭设备。无参数运行会按前台应用和 `rules` 表持续发送 SET。当前示例规则为 Terminal/Ghostty → 2、Chrome/Safari → 3，其余 → 0；使用前应按实际键位调整。第 3 层可能是功能层，不一定适合作为日常默认层。

当前 companion 为已验证的原型：不安装后台服务，不自动处理设备断开重连；应用切换映射尚未做物理按键验收。请求串行执行，但超时后的迟到应答关联和共享回调状态仍需在长期运行版本中加固。保留 `IOHIDManager` 生命周期并在注册回调后 `IOHIDDeviceActivate` 是接收正常工作的必要实现步骤。IOHID 与 hidapi 的 report-ID 接口不同，不应直接照搬缓冲区格式。

## VIA 备份、恢复和验证

需要安装 Python `hidapi` 包；导入名为 `hid`。使用现有环境或单独 venv。以下以 `python` 指代该环境的解释器：

```sh
python -I util/q3pro_auto/via_transfer.py backup /absolute/path/device-before.json
python -I util/q3pro_auto/via_transfer.py restore /absolute/path/source.json --before /absolute/path/before-restore.json
python -I util/q3pro_auto/via_transfer.py verify /absolute/path/source.json
```

备份支持当前设备层数；恢复要求源和设备均为 8 层、250 字节宏区。源 JSON 包含：`layers`、`keymap.layer0` 至 `layer7`（各 216 个字节）、`encoder_map.layerN.ccw/cw`（16 位键码）、`macro_buffer_size`、`macro_buffer`、`layout_options`。早期 corrected 导出缺少 `layout_options` 时，要从独立导出核实并补齐，不猜值。

工具限定唯一 `3434:0630 / FF60:61` 设备，采用非阻塞有界读取与必关闭句柄，按全局 16 位字节偏移读写，写前保存新快照；宏写入期间设置末字节禁用标志；完成后逐字节读回。失败时停止并保留快照，不执行不透明的自动回滚。恢复期间不要敲击目标键盘。

用户键位和宏可能包含私人文本，备份文件以 0600 新建，不能放进仓库。工具只恢复 VIA 可导出的目标数据，不包含原设备未导出的 EECONFIG（灯光等核心设置）。

## 刷写与设备身份

STM32 DFU 键盘是 `0483:df11`，必须核实序列号和 `Internal Flash` alt0。`17e9:6000` 是坞站，不能用作键盘目标。`dfu-util -D` 写入，`-U` 读回；不要碰 Option Bytes、OTP 或执行整片擦除。当前固件没有实现 VIA `id_bootloader_jump`，进入 DFU 需要物理操作。

刷写完成应对全部代码段读回比对。若设备仍在 DFU，松开 reset/Esc，Off、拔线、重新插线、Cable 后验证 VIA；正常模式不是由下载成功返回码保证的。

## 2026-11-02 验证记录

- 8 层 raw 固件 55904 字节，SHA256 `cdb7277a545b5fff4b8bf8314e894ca19fa098881d5411d84a5bb66f904b6f8b`；全部代码段读回一致。这是当日构建哈希，重新编译可能不同。
- 1728 字节键位、16 个旋钮键码、250 字节宏、布局选项恢复后，关闭并重新打开设备独立读回一致。
- 手动模式拒绝 SET；Win 模式接受 layer6，拒绝 layer8；每 2 秒续租四次后，最后 SET 后 6.938 秒仍有效，7.047 秒观察到到期。
- 实际 Swift companion 在 10.5 秒内成功 heartbeat 五次；终止后 6.545 秒观察到剩余租约到期；最后显式恢复 layer0 并等待租约失效。
- 验收边界：GET 自身会检查到期，未独立观测“完全无查询时 housekeeping 回退”的物理按键输出；真实应用激活映射和有活跃信号时拨回手动的实机验收仍待完成。

完整历史：`skldr read ctx-XnQc`（原交接）、`skldr read ctx-Z3sE`（修复证据与复盘请求）、`skldr read ctx-Sx7e`（原 Agent 自省）。临时证据曾保存在 `/tmp/q3pro-8layer-20261102/`，不保证长期存在。

## 内化的排障规则

1. 先确认唯一设备和进程所有权，再进行读写；探测必须有界并在 finally 关闭。
2. 在同一设备和状态下，用最小 hidapi GET 与 Swift 对照；一种客户端成功不能为另一种实现背书。
3. 检查 SDK 对调度、activation、对象持有的要求，再追固件端点。未收到应答不等于固件接收函数从未执行。
4. 分开记录枚举、open、write 返回、协议应答、全段固件读回、数据读回及物理行为验收。
5. 历史 hidapi 全部超时及当时只能读 16 KB 的原因仍未知。不要用 Swift 修复或后来的成功填补历史因果缺口。
6. 复盘也要与源码核对：`via.c` 先调用 `via_command_kb`，Q3 Pro 在启用蓝牙时会把 `0xAA` 分派到蓝牙 DFU 处理，不能采纳“VIA 不处理 0xAA”的绝对说法。也不要拿未明确协议语义的 0xAA 当无副作用探针。IOHID API 名称与成功返回值不直接证明总线传输走哪条端点。
