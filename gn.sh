# when add or delete source file, first hand cmd `python3 generate_sources.py`
rm -rf out && \
mkdir out && \
gn gen ./out -v && \
ninja -C out test_main && \
./out/test_main