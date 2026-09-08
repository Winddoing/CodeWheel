# USB Descriptor Tool Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = usb_descriptor_tool.out
SRC = usb_descriptor_tool.c

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# 测试目标（需要连接 USB 设备）
test: $(TARGET)
	@echo "请确保已连接 USB 设备"
	@echo "使用 lsusb 查看设备列表"
	lsusb

# 运行示例（需要 root 权限）
run: $(TARGET)
	sudo ./$(TARGET) 001 002

help:
	@echo "可用目标:"
	@echo "  all     - 编译程序 (默认)"
	@echo "  clean   - 清理编译产物"
	@echo "  install - 安装到 /usr/local/bin"
	@echo "  test    - 显示 USB 设备列表"
	@echo "  run     - 运行程序 (示例：总线 1, 设备 2)"
	@echo "  help    - 显示此帮助"
