#!/bin/bash

# 获取当前脚本所在目录
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# 从当前目录开始，向上查找直到找到CMakeLists.txt文件，确定项目根目录
PROJECT_DIR=$SCRIPT_DIR
while [ ! -f "$PROJECT_DIR/CMakeLists.txt" ]; do
    # 检查是否到达系统根目录
    if [ "$PROJECT_DIR" = "/" ]; then
        echo "Error: CMakeLists.txt not found in any parent directory."
        exit 1
    fi
    PROJECT_DIR=$(dirname "$PROJECT_DIR")
done

# 获取项目根目录名
PROJECT_NAME=$(basename "$PROJECT_DIR")

echo "Project root director name: $PROJECT_NAME"

# 创建并进入构建目录
BUILD_DIR=$PROJECT_DIR/build
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# 运行CMake配置和编译项目
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake
make -j4

# 根据项目名称生成ELF、BIN和HEX文件路径
ELF_FILE="$BUILD_DIR/${PROJECT_NAME}.elf"
BIN_FILE="$BUILD_DIR/${PROJECT_NAME}.bin"
HEX_FILE="$BUILD_DIR/${PROJECT_NAME}.hex"

# 检查ELF文件是否生成成功
if [ -f "$ELF_FILE" ]; then
    # 将ELF文件转换为BIN文件和HEX文件
    arm-none-eabi-objcopy -O binary "$ELF_FILE" "$BIN_FILE"
    arm-none-eabi-objcopy -O ihex "$ELF_FILE" "$HEX_FILE"
    echo "file $BIN_FILE and $HEX_FILE generated."
    # echo "Conversion to BIN and HEX files completed."
else
    echo "Error: file $ELF_FILE not found. Compilation might have failed."
    exit 1
fi

# ----------------------------------------------------------------------
# 关键删除：在 Linux/Ubuntu 上不需要进行 Windows 路径转换
# HEX_FILE=$(cygpath -w "$HEX_FILE" | sed 's/\\/\//g') 
# ----------------------------------------------------------------------

# 执行OpenOCD进行烧录
openocd \
    -f interface/cmsis-dap.cfg \
    -f target/stm32f1x.cfg \
    -c "adapter speed 1000" \
    -c "init" \
    -c "reset_config none" \
    -c "halt" \
    -c "program $HEX_FILE verify" \
    -c "reset run" \
    -c "exit"
