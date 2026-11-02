#!/bin/sh
# 从任意工作目录运行实际固件策略的主机测试。
set -eu
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
repo_dir=$(CDPATH= cd -- "$script_dir/../.." && pwd)
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT HUP INT TERM
clang -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined \
    -I "$script_dir/tests" \
    "$script_dir/tests/lease_test.c" \
    "$repo_dir/keyboards/keychron/q3_pro/ansi_encoder_se/ansi_encoder_se.c" \
    -o "$test_dir/lease_test"
"$test_dir/lease_test"
