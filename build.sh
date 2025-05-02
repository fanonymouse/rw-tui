#!/bin/bash

# Exit on error
set -e

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "Please run as root (use sudo)"
    exit 1
fi

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building and setting up rw-tui...${NC}"

# Build fmem kernel module
echo -e "${GREEN}Building fmem kernel module...${NC}"
cd fmem
make clean
make
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to build fmem module${NC}"
    exit 1
fi

# Load fmem module
echo -e "${GREEN}Loading fmem kernel module...${NC}"
./run.sh
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to load fmem module${NC}"
    exit 1
fi

# Go back to root directory
cd ..

# Create and enter build directory
echo -e "${GREEN}Building rw-tui...${NC}"
mkdir -p build
cd build

# Generate build files and build
cmake ..
cmake --build .
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to build rw-tui${NC}"
    exit 1
fi

# Run rw-tui
echo -e "${GREEN}Starting rw-tui...${NC}"
./rw-tui 