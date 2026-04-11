# JSON Tree Tool

This is a lightweight command-line JSON tree viewer with color highlighting and customizable themes. Written in pure C with minimal dependencies, it provides clear tree-structured visualization of JSON data in the terminal.

## Features

- Tree-structured JSON visualization
- Color-coded output with ANSI/256/RGB support
- Customizable themes
- Configurable maximum depth
- File and stdin input support
- Pure C implementation, minimal dependencies

## Installation

```bash
git clone https://github.com/Mrkalilunx/jst.git
cd jst
make
```

## Usage

```bash
# View a JSON file
./jst file.json

# Read from stdin
cat file.json | ./jst

# Use custom theme
./jst file.json -C themes/dark.json

# Limit display depth
./jst file.json -L 2

# Show timing details
./jst file.json -d
```

## Options

```
-h, --help                 Show help message
-v, --verbose              Show version information
-d, --detail               Show detailed output including timing
-i, --input FILE           Input JSON file (optional)
-L, --max-depth N          Maximum display depth
-C, --config FILE          Use specified config file
--generate-config [FILE]   Generate default config file
```

## Themes

Available themes in `themes/` directory:

- `dark.json` - Dark theme
- `light.json` - Light theme
- `colorful.json` - Vibrant colors
- `minimal.json` - Minimal styling
- `pastel.json` - Soft colors
- `neon.json` - High contrast
- `ocean.json` - Blue-green tones
- `forest.json` - Green-yellow tones

## Project Structure

```
jst/
├── src/          # Source code
├── tests/        # Test JSON files
├── themes/       # Color themes
├── Makefile
└── test.sh       # Test script
```

## Testing

```bash
./test.sh
```