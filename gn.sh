rm -rf out && \
mkdir out && \
gn gen ./out -v && \
ninja -C out test_main && \
./out/test_main