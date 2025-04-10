#!/bin/sh
set -e  # 任何命令失败时立即退出
cd "$(dirname "$0")" || exit 1
rm -rf out && \
mkdir out && \
gn gen ./out -v && \
ninja -C out test_main && \
./out/test_main