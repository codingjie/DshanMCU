#!/bin/bash

# 获取当前脚本所在目录
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# 从当前目录开始，向上查找直到找到CMakeLists.txt文件，确定项目根目录
PROJECT_DIR=$SCRIPT_DIR
while [ ! -f "$PROJECT_DIR/CMakeLists.txt" ]; do
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
cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake
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

# 将HEX文件路径转换为Windows格式
HEX_FILE=$(cygpath -w "$HEX_FILE" | sed 's/\\/\//g')

# 执行OpenOCD进行烧录，如果发现烧录后执行了两次可以取消-c "reset run"
# openocd -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg -c "program $HEX_FILE verify reset" -c "reset run" -c "exit"

# 前者依赖硬件nRESET引脚执行reset halt，而后者通过SWD halt + reset run绕过了对nRESET的依赖，所以能成功
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