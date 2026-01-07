# JST - JSON Tree Viewer

一款高效的命令行 JSON 树状查看器，可将 JSON 文件以带有颜色高亮的树形结构展示，帮助开发者更直观地查看和理解 JSON 数据结构。

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)

## 功能特性

- 🎨 **彩色输出** - 支持无色、简洁和丰富三种颜色模式
- 📊 **可配置深度** - 灵活控制显示的最大深度，避免输出过长
- 🔍 **键过滤** - 支持按键名过滤，只显示匹配的键
- 📁 **多种输入方式** - 支持从文件或标准输入读取 JSON 数据
- 🌳 **树形可视化** - 使用 Unicode 字符呈现简洁美观的树形结构
- 📍 **路径显示** - 可选显示数组/对象项的完整路径
- 📈 **统计信息** - 可选显示 JSON 项目的统计信息
- 🔢 **类型标注** - 可选显示值的类型信息
- 🗜️ **紧凑模式** - 支持紧凑输出，减少间距
- 🌍 **多语言** - 支持中文和英文界面
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
./jst [选项] [文件]
```

#### 可用选项

| 选项 | 长选项 | 参数 | 说明 |
|------|--------|------|------|
| `-c` | `--color` | MODE | 颜色模式：none（无色）、simple（简洁，默认）、rich（丰富） |
| `-k` | `--filter` | KEY | 过滤输出，仅显示匹配的键 |
| `-L` | `--max-depth` | N | 设置最大显示深度 |
| `-p` | `--compact` | - | 紧凑输出（减少间距） |
| `-s` | `--path` | - | 显示数组/对象项的完整路径 |
| `-l` | `--lang` | LANG | 语言：zh（中文）、en（英文） |
| `-t` | `--types` | - | 显示值的类型信息 |
| `--stats` | - | - | 显示 JSON 统计信息 |
| `-h` | `--help` | - | 显示帮助信息 |
| `-v` | `--version` | - | 显示版本信息 |

### 使用示例

```bash
# 基本用法
./jst file.json

# 使用丰富颜色模式
./jst -c rich data.json

# 限制显示深度为 2 层
./jst -L 2 data.json

# 仅显示包含 'name' 的键
./jst -k name data.json

# 紧凑输出
./jst -p data.json

# 显示路径和类型信息
./jst -s -t data.json

# 显示统计信息
./jst --stats data.json

# 无颜色输出（适合重定向到文件）
./jst -c none data.json > output.txt

# 组合使用多个选项
./jst -c rich -L 3 -s -t data.json

# 从标准输入读取
cat data.json | ./jst -c rich

# 使用管道
curl -s https://api.github.com/users | ./jst -L 2
```

## 颜色模式

### 无色模式 (none)
不使用任何颜色，适合重定向到文件或在无颜色支持的终端中使用。

### 简洁模式 (simple)
使用柔和的颜色方案，清晰易读：
- 🔵 **键** - 亮蓝色
- 🟢 **字符串值** - 柔和绿色
- 🟡 **数字值** - 暖黄色
- 🟣 **布尔值** - 柔和紫红色
- ⚫ **null 值** - 灰色
- 🩵 **数组索引** - 柔和青色

### 丰富模式 (rich)
根据数据类型使用精心设计的颜色方案，提供更丰富的视觉信息：

**键（冷色调）：**
- 🔵 **对象键** - 深蓝色
- 🔷 **数组键** - 青色
- 🩵 **字符串键** - 浅蓝色
- 🟣 **数字键** - 紫色
- 🟪 **布尔键** - 柔和紫红色
- ⚫ **null 键** - 灰色

**值（暖色调和鲜艳色）：**
- 💚 **字符串值** - 亮绿色
- 🟠 **数字值** - 橙色
- ❤️ **布尔值** - 红色
- 🖤 **null 值** - 深灰色
- 💎 **数组索引** - 亮青色

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