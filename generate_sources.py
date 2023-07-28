#!/usr/bin/env python3

import os
import sys

# 获取所有源文件
sources = []
for root, dirs, files in os.walk('.'):
    for file in files:
        if any(file.endswith(ext) for ext in ['.h','.cpp']):
            sources.append(os.path.join(root, file))

# 输出源文件列表
# sys.stdout.write(str(sources))

# 输出源文件列表
# sys.stdout.write(str(includes))

# 生成 GN 文件
with open('sources.gn', 'w') as f:
    f.write('sources = [\n')
    for source in sources:
        f.write('  "{}",\n'.format(source))
    f.write(']\n')