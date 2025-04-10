#!/usr/bin/env python3

import os
import sys

os.chdir(os.path.dirname(os.path.abspath(__file__)))

# 获取所有源文件
sources = []
for root, dirs, files in os.walk('.'):
    for file in files:
        if any(file.endswith(ext) for ext in ['.h','.cpp']):
            # 去掉 ./ 前缀
            sources.append(os.path.join(root, file)[2:])

# 输出源文件列表
# sys.stdout.write(str(sources))

# 输出源文件列表
# sys.stdout.write(str(includes))

# 生成 GN 文件
with open('sources.gn', 'w') as f:
    f.write('xlib_source = [\n')
    for source in sources:
        f.write('  "{}",\n'.format(source))
    f.write(']\n')