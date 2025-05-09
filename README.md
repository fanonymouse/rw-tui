rw-tui :: Read & Write Everything
---------------------------------

This utility was created as a TUI application for Linux with reference to [RWEverything](http://rweverything.com/).

![rw-tui](./rw-tui_screenshot_1.gif)

![Example of using rw-tui](./rw-tui_screenshot_2.gif)

# How to use
1. Build the code in [@LeeKyuHyuk/fmem](https://github.com/LeeKyuHyuk/fmem) to `insmod`.
    - This is the latest fmem kernel module that implements Write function in the existing fmem.
2. Run `rw-tui` as root privileges

# Building from source
## Prerequisites
- CMake 3.11 or higher
- C++17 compatible compiler
- Git (for cloning the repository)

## Build steps
1. Clone the repository:
```bash
git clone https://github.com/yourusername/rw-tui.git
cd rw-tui
git submodule update --init --recursive
```

2. Create and enter the build directory:
```bash
mkdir build
cd build
```

3. Generate build files:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build .
```

The `rw-tui` executable will be created in the build directory.

## Execute
1. Build the fmem kernel module:
```bash
cd fmem
make
```

2. Load the fmem kernel module:
```bash
./run.sh
```

3. Run rw-tui:
```bash
cd ..  # Go back to rw-tui root directory
./rw-tui
```

Note: Make sure to run these commands with root privileges.

# Features supported
- Output the memory area as a table and change the value
- Output configuration space MMIO area of PCI device as table
- PCI configuration space output

# Hotkey
The shortcut keys below operate within the table:
- `PageUp` Key: Move forward 0x100 of the current address
- `PageDown` Key: Move back 0x100 of the current address
- `F2` Key : Move to Address Change Input
- `F5` Key : Refresh

# Thanks to
- [RWEverything](http://rweverything.com/) for the idea of this Application.
- [Nate Brune](https://github.com/NateBrune) and the contributors who developed fmem.

# License
MIT © [KyuHyuk Lee](https://kyuhyuk.kr) <[lee@kyuhyuk.kr](mailto:lee@kyuhyuk.kr)>