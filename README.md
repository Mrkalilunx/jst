# JSON 树状查看器

这是一个轻量级的命令行 JSON 树状查看器，支持颜色高亮和可自定义主题。该项目使用纯 C 语言实现，依赖极小，能够将 JSON 数据以树形结构清晰地展示在终端中。

## 功能特性

- 树形结构 JSON 可视化
- 支持 ANSI/256/RGB 颜色编码
- 可自定义主题
- 可配置最大显示深度
- 支持文件和标准输入
- 纯 C 实现，依赖极小

## 安装

```bash
git clone https://github.com/Mrkalilunx/jst.git
cd jst
make
```

## 使用方法

```bash
# 查看 JSON 文件
./jst file.json

# 从标准输入读取
cat file.json | ./jst

# 使用自定义主题
./jst file.json -C themes/dark.json

# 限制显示深度
./jst file.json -L 2

# 显示详细时间信息
./jst file.json -d
```

## 选项

```
-h, --help                 显示帮助信息
-v, --verbose              显示版本信息
-d, --detail               显示详细信息（包括时间）
-i, --input FILE           输入 JSON 文件（可选）
-L, --max-depth N          最大显示深度
-C, --config FILE          使用指定的配置文件
--generate-config [FILE]   生成默认配置文件
```

## 主题

`themes/` 目录中提供的主题：

- `dark.json` - 深色主题
- `light.json` - 浅色主题
- `colorful.json` - 鲜艳色彩
- `minimal.json` - 极简风格
- `pastel.json` - 柔和色彩
- `neon.json` - 高对比度
- `ocean.json` - 蓝绿色调
- `forest.json` - 绿黄色调

## 项目结构

```
jst/
├── src/          # 源代码
├── tests/        # 测试 JSON 文件
├── themes/       # 颜色主题
├── Makefile
└── test.sh       # 测试脚本
```

## 测试

```bash
./test.sh
```

## 编译选项

```bash
# 正常编译（简洁输出）
make

# 详细编译（显示完整命令）
make V=1

# 清理
make clean
```

## 许可证

MIT License - 详见 [LICENSE](LICENSE) 文件