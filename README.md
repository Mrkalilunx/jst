# JST - JSON Tree Viewer

一款高效的命令行 JSON 树状查看器，可将 JSON 文件以带有颜色高亮的树形结构展示，帮助开发者更直观地查看和理解 JSON 数据结构。

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)

## 功能特性

- 🎨 **彩色输出** - 支持简化模式和丰富模式，根据数据类型使用不同颜色
- 📊 **可配置深度** - 灵活控制显示的最大深度，避免输出过长
- 📁 **多种输入方式** - 支持从文件或标准输入读取 JSON 数据
- 🌳 **树形可视化** - 使用 Unicode 字符呈现简洁美观的树形结构
- ⚡ **轻量高效** - 纯 C 语言实现，依赖极小，性能优异

## 安装

### 前置要求

- GCC 编译器
- Make 构建工具
- cJSON 库（已包含在项目中）

### 从源码构建

```bash
# 克隆仓库
git clone https://github.com/Mrkalilunx/jst.git
cd jst

# 编译项目
make

# （可选）清理构建产物
make clean
```

编译完成后，会在当前目录生成可执行文件 `jst`。

## 使用方法

### 基本用法

```bash
# 查看文件
./jst example.json

# 从标准输入读取
cat example.json | ./jst

# 使用管道
curl -s https://api.example.com/data | ./jst
```

### 命令行选项

```bash
./jst [选项] [文件路径]
```

可用选项：
- `-c, --color <模式>` - 设置颜色模式（simplified/rich/none）
- `-d, --depth <数值>` - 设置最大显示深度
- `-h, --help` - 显示帮助信息

### 使用示例

```bash
# 使用丰富颜色模式
./jst -c rich data.json

# 限制显示深度为 3 层
./jst -d 3 data.json

# 无颜色输出（适合重定向到文件）
./jst -c none data.json > output.txt
```

## 颜色模式

### 简化模式 (simplified)
所有键使用统一的紫色，简洁明了。

### 丰富模式 (rich)
根据数据类型使用不同颜色：
- 🔵 **对象键** - 蓝色
- 🟠 **数组键** - 橙色
- 🟢 **字符串键** - 绿色
- 🟣 **数字键** - 紫色
- 🔴 **布尔键** - 红色
- ⚫ **null 键** - 灰色
- 💚 **字符串值** - 亮绿色
- 💜 **数字值** - 紫色
- ❤️ **布尔值** - 红色
- 🖤 **null 值** - 灰色

## 项目结构

```
jst/
├── src/                    # 源代码目录
│   ├── main.c             # 程序入口
│   ├── args.c/h           # 命令行参数解析
│   ├── color.c/h          # 颜色模式管理
│   ├── file.c/h           # 文件读取和处理
│   ├── tree.c/h           # 树形结构打印
│   ├── json.c/h           # JSON 数据预处理
│   └── util.c/h           # 工具函数
├── third_party/           # 第三方库
│   └── cJSON/             # JSON 解析库
├── resources/             # 资源文件
├── Makefile               # 构建配置
└── README.md              # 项目说明
```

## 技术栈

- **语言**: C
- **编译器**: GCC
- **依赖库**: cJSON
- **构建系统**: Make

## 贡献

欢迎提交 Issue 和 Pull Request！

## 许可证

MIT License

## 作者

悠悠的小球球

## 致谢

感谢 [cJSON](https://github.com/DaveGamble/cJSON) 项目提供的优秀 JSON 解析库。