# USB Descriptor Tool

使用底层 ioctl 接口获取 USB 设备描述符的工具，不依赖 libusb 库。

## 功能特点

- ✅ 使用 Linux `usbdevice_fs` 的 ioctl 接口
- ✅ 每次获取描述符前先重置设备 (`USBDEVFS_RESET`)
- ✅ 不依赖 libusb，纯原生实现
- ✅ 支持获取：
  - 设备描述符 (Device Descriptor)
  - 配置描述符 (Configuration Descriptor)
  - 字符串描述符 (String Descriptor)
  - 语言 ID 列表

## 编译

```bash
# 使用 Makefile
make

# 或直接编译
gcc -Wall -Wextra -O2 -o usb_descriptor_tool usb_descriptor_tool.c
```

## 使用方法

### 查看 USB 设备

```bash
lsusb
```

输出示例：
```
Bus 001 Device 002: ID 8087:8000 Intel Corp.
```

### 获取描述符

```bash
# 需要 root 权限
sudo ./usb_descriptor_tool <bus> <device>

# 示例：总线 1, 设备 2
sudo ./usb_descriptor_tool 001 002

# 或直接使用设备路径
sudo ./usb_descriptor_tool /dev/bus/usb/001/002
```

### 命令行选项

```
用法: ./usb_descriptor_tool <bus> <device> [选项]
   或: ./usb_descriptor_tool <设备路径>

选项:
  -d, --device      仅获取设备描述符
  -c, --config      获取配置描述符
  -s, --string      获取字符串描述符
  -a, --all         获取所有描述符
  -r, --reset-only  仅重置设备，不获取描述符
  -h, --help        显示帮助
```

### 示例

```bash
# 获取所有描述符
sudo ./usb_descriptor_tool 001 002 -a

# 仅获取设备描述符
sudo ./usb_descriptor_tool 001 002 -d

# 仅重置设备
sudo ./usb_descriptor_tool 001 002 -r

# 获取配置描述符
sudo ./usb_descriptor_tool 001 002 -c
```

## 技术实现

### 核心 ioctl 调用

1. **USBDEVFS_RESET** - 重置 USB 设备
   ```c
   ioctl(fd, USBDEVFS_RESET, 0);
   ```

2. **USBDEVFS_CONTROL** - 执行控制传输
   ```c
   struct usbdevfs_ctrltransfer ct = {
       .bmRequestType = USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE,
       .bRequest      = USB_REQ_GET_DESCRIPTOR,
       .wValue        = (desc_type << 8) | desc_index,
       .wIndex        = 0,
       .wLength       = buffer_size,
       .timeout       = 5000,
       .data          = buffer
   };
   ioctl(fd, USBDEVFS_CONTROL, &ct);
   ```

### USB 描述符类型

| 类型 | 值 | 说明 |
|------|-----|------|
| USB_DT_DEVICE | 0x01 | 设备描述符 |
| USB_DT_CONFIG | 0x02 | 配置描述符 |
| USB_DT_STRING | 0x03 | 字符串描述符 |
| USB_DT_INTERFACE | 0x04 | 接口描述符 |
| USB_DT_ENDPOINT | 0x05 | 端点描述符 |

### 控制传输请求

```
GET_DESCRIPTOR 请求格式:
  bmRequestType = 0x80 (DIR_IN | STANDARD | DEVICE)
  bRequest      = 0x06 (GET_DESCRIPTOR)
  wValue        = (描述符类型 << 8) | 描述符索引
  wIndex        = 0
  wLength       = 缓冲区大小
```

## 权限要求

需要 root 权限访问 `/dev/bus/usb/*` 设备文件：

```bash
sudo ./usb_descriptor_tool ...
```

或者添加 udev 规则（不推荐用于生产环境）：

```bash
# /etc/udev/rules.d/99-usb-permissions.rules
SUBSYSTEM=="usb", MODE="0666"
```

## 依赖

- Linux 内核 (2.6.30+ 推荐)
- gcc 编译器
- 标准 C 库

## 输出示例

```
========================================
USB 描述符获取工具
========================================
设备路径：/dev/bus/usb/001/002
[*] 重置 USB 设备...
[+] 设备重置成功
[*] 获取设备描述符...
[+] 获取成功，返回 18 字节

=== 设备描述符 ===
  bLength:            18
  bDescriptorType:    0x01
  bcdUSB:             0x0210 (USB 2.1)
  bDeviceClass:       0x00
  bDeviceSubClass:    0x00
  bDeviceProtocol:    0x00
  bMaxPacketSize0:    64
  idVendor:           0x8087
  idProduct:          0x8000
  bcdDevice:          0x0000
  iManufacturer:      0
  iProduct:           0
  iSerialNumber:      0
  bNumConfigurations: 1

原始数据:
  12 01 10 02 00 00 00 40 87 80 00 80 00 00 00 00 
  01 00 
```

## 故障排除

### 权限被拒绝

```
打开设备失败：Permission denied
```

**解决**: 使用 `sudo` 运行

### 设备未找到

```
打开设备失败：No such file or directory
```

**解决**: 
1. 确认设备已连接
2. 使用 `lsusb` 查看正确的总线/设备号
3. 检查设备路径是否正确

### 控制传输失败

```
USBDEVFS_CONTROL 失败：No such device
```

**解决**:
1. 设备可能已断开
2. 设备可能被其他程序占用
3. 尝试重新插拔设备

## 参考资料

- [Linux USB Documentation](https://www.kernel.org/doc/html/latest/driver-api/usb/index.html)
- [usbdevice_fs.h 源码](https://github.com/torvalds/linux/blob/master/include/uapi/linux/usbdevice_fs.h)
- [USB 规范](https://usb.org/document-library/usb-20-specification)

## 许可证

MIT License
