#!/bin/sh
# 在 macOS/arm64 开发机上用官方 QMK 容器构建 Q3 Pro VIA 固件。
# 宿主机不需要安装 arm-none-eabi-gcc、qmk 或 Python 依赖。
# 用法：util/q3pro_auto/build.sh [make 目标]
set -eu

cd "$(dirname "$0")/../.."

IMAGE="${QMK_IMAGE:-qmkfm/qmk_cli}"
TARGET="${1:-keychron/q3_pro/ansi_encoder_se:via}"

docker run --rm -v "$PWD":/qmk -w /qmk "$IMAGE" \
  sh -c "python3 -m pip install -q -r requirements.txt && make $TARGET"

# 容器内以 root 写入；把构建产物交还宿主机当前用户。
docker run --rm -v "$PWD":/qmk "$IMAGE" \
  sh -c "chown -R $(id -u):$(id -g) /qmk/.build /qmk/*.bin"
