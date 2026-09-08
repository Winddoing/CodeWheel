/*
 * usb_descriptor_tool.c
 * 
 * 获取USB设备描述符的工具
 * 使用底层 ioctl 接口，不依赖 libusb
 * 每次获取描述符前先重置设备
 * 
 * 编译: gcc -o usb_descriptor_tool usb_descriptor_tool.c
 * 运行: sudo ./usb_descriptor_tool <bus> <device>
 *       例如: sudo ./usb_descriptor_tool 001 002
 * 
 * 或者直接使用设备路径:
 *       sudo ./usb_descriptor_tool /dev/bus/usb/001/002
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include <linux/types.h>

/* USB 描述符类型 */
#define USB_DT_DEVICE           0x01    /* 设备描述符 */
#define USB_DT_CONFIG           0x02    /* 配置描述符 */
#define USB_DT_STRING           0x03    /* 字符串描述符 */
#define USB_DT_INTERFACE        0x04    /* 接口描述符 */
#define USB_DT_ENDPOINT         0x05    /* 端点描述符 */
#define USB_DT_DEVICE_QUALIFIER 0x06    /* 设备限定符描述符 */
#define USB_DT_OTHER_SPEED      0x07    /* 其他速度配置描述符 */
#define USB_DT_INTERFACE_POWER  0x08    /* 接口电源描述符 */
#define USB_DT_BOS              0x0f    /* Binary Object Store */
#define USB_DT_CAPABILITY       0x10    /* 设备能力描述符 */

/* USB 请求类型 */
#define USB_TYPE_STANDARD       0x00
#define USB_TYPE_CLASS          0x01
#define USB_TYPE_VENDOR         0x02
#define USB_TYPE_RESERVED       0x03

#define USB_DIR_OUT             0x00
#define USB_DIR_IN              0x80

#define USB_RECIP_DEVICE        0x00
#define USB_RECIP_INTERFACE     0x01
#define USB_RECIP_ENDPOINT      0x02

/* 标准 USB 请求 */
#define USB_REQ_GET_DESCRIPTOR  0x06
#define USB_REQ_SET_DESCRIPTOR  0x07
#define USB_REQ_GET_CONFIGURATION 0x08
#define USB_REQ_SET_CONFIGURATION 0x09
#define USB_REQ_GET_INTERFACE   0x0a
#define USB_REQ_SET_INTERFACE   0x0b

/* 设备描述符结构 */
struct usb_device_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
} __attribute__((packed));

/* 配置描述符结构 */
struct usb_config_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  bMaxPower;
} __attribute__((packed));

/* 字符串描述符结构 */
struct usb_string_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wData[];
} __attribute__((packed));

/* 控制传输参数 */
struct ctrl_transfer {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
    uint32_t timeout;
    void     *data;
};

/* 打印十六进制数据 */
void print_hex(const char *prefix, const uint8_t *data, size_t len) {
    printf("%s", prefix);
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
            if (i < len - 1) {
                printf("    ");
            }
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

/* 打印设备描述符 */
void print_device_descriptor(const struct usb_device_descriptor *desc) {
    printf("\n=== 设备描述符 ===\n");
    printf("  bLength:            %u\n", desc->bLength);
    printf("  bDescriptorType:    0x%02x\n", desc->bDescriptorType);
    printf("  bcdUSB:             0x%04x (USB %d.%02d)\n", 
           desc->bcdUSB, 
           (desc->bcdUSB >> 8) & 0xf,
           (desc->bcdUSB >> 4) & 0xf);
    printf("  bDeviceClass:       0x%02x\n", desc->bDeviceClass);
    printf("  bDeviceSubClass:    0x%02x\n", desc->bDeviceSubClass);
    printf("  bDeviceProtocol:    0x%02x\n", desc->bDeviceProtocol);
    printf("  bMaxPacketSize0:    %u\n", desc->bMaxPacketSize0);
    printf("  idVendor:           0x%04x\n", desc->idVendor);
    printf("  idProduct:          0x%04x\n", desc->idProduct);
    printf("  bcdDevice:          0x%04x\n", desc->bcdDevice);
    printf("  iManufacturer:      %u\n", desc->iManufacturer);
    printf("  iProduct:           %u\n", desc->iProduct);
    printf("  iSerialNumber:      %u\n", desc->iSerialNumber);
    printf("  bNumConfigurations: %u\n", desc->bNumConfigurations);
}

/* 打印配置描述符 */
void print_config_descriptor(const struct usb_config_descriptor *desc) {
    printf("\n=== 配置描述符 ===\n");
    printf("  bLength:             %u\n", desc->bLength);
    printf("  bDescriptorType:     0x%02x\n", desc->bDescriptorType);
    printf("  wTotalLength:        %u\n", desc->wTotalLength);
    printf("  bNumInterfaces:      %u\n", desc->bNumInterfaces);
    printf("  bConfigurationValue: %u\n", desc->bConfigurationValue);
    printf("  iConfiguration:      %u\n", desc->iConfiguration);
    printf("  bmAttributes:        0x%02x\n", desc->bmAttributes);
    printf("  bMaxPower:           %u (=%u mA)\n", desc->bMaxPower, desc->bMaxPower * 2);
}

/* 打印字符串描述符 */
void print_string_descriptor(const struct usb_string_descriptor *desc) {
    printf("\n=== 字符串描述符 ===\n");
    printf("  bLength:         %u\n", desc->bLength);
    printf("  bDescriptorType: 0x%02x\n", desc->bDescriptorType);
    printf("  wData:           ");
    
    int len = (desc->bLength - 2) / 2;
    for (int i = 0; i < len; i++) {
        if (desc->wData[i] < 128) {
            printf("%c", (char)desc->wData[i]);
        } else {
            printf("\\u%04x", desc->wData[i]);
        }
    }
    printf("\n");
}

/* 重置 USB 设备 */
int usb_reset_device(int fd) {
    printf("[*] 重置 USB 设备...\n");
    
    if (ioctl(fd, USBDEVFS_RESET, 0) < 0) {
        perror("USBDEVFS_RESET 失败");
        return -1;
    }
    
    printf("[+] 设备重置成功\n");
    return 0;
}

/* 执行控制传输 */
int usb_control_transfer(int fd, struct ctrl_transfer *ctrl) {
    struct usbdevfs_ctrltransfer ct = {
        .bRequestType = ctrl->bmRequestType,
        .bRequest     = ctrl->bRequest,
        .wValue       = ctrl->wValue,
        .wIndex       = ctrl->wIndex,
        .wLength      = ctrl->wLength,
        .timeout      = ctrl->timeout,
        .data         = ctrl->data
    };
    
    int ret = ioctl(fd, USBDEVFS_CONTROL, &ct);
    if (ret < 0) {
        return -1;
    }
    
    return ret;
}

/* 获取描述符 */
int usb_get_descriptor(int fd, uint8_t desc_type, uint8_t desc_index, 
                       void *buffer, uint16_t buffer_size) {
    struct ctrl_transfer ctrl = {
        .bmRequestType = USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE,
        .bRequest      = USB_REQ_GET_DESCRIPTOR,
        .wValue        = (desc_type << 8) | desc_index,
        .wIndex        = 0,
        .wLength       = buffer_size,
        .timeout       = 5000,  /* 5 秒超时 */
        .data          = buffer
    };
    
    int ret = usb_control_transfer(fd, &ctrl);
    if (ret < 0) {
        perror("USBDEVFS_CONTROL 失败");
        return -1;
    }
    
    return ret;
}

/* 获取设备描述符 */
int get_device_descriptor(int fd, struct usb_device_descriptor *desc) {
    printf("[*] 获取设备描述符...\n");
    
    int ret = usb_get_descriptor(fd, USB_DT_DEVICE, 0, desc, sizeof(*desc));
    if (ret < 0) {
        return -1;
    }
    
    printf("[+] 获取成功，返回 %d 字节\n", ret);
    return ret;
}

/* 获取配置描述符 */
int get_config_descriptor(int fd, uint8_t config_index, 
                          struct usb_config_descriptor *desc,
                          uint8_t *full_config, uint16_t *full_size) {
    printf("[*] 获取配置描述符 (索引=%d)...\n", config_index);
    
    /* 先获取配置描述符头部 */
    int ret = usb_get_descriptor(fd, USB_DT_CONFIG, config_index, desc, sizeof(*desc));
    if (ret < 0) {
        return -1;
    }
    
    printf("[+] 配置描述符头部获取成功，总长度=%u\n", desc->wTotalLength);
    
    /* 获取完整的配置描述符（包括接口和端点描述符） */
    if (full_config && desc->wTotalLength <= 4096) {
        ret = usb_get_descriptor(fd, USB_DT_CONFIG, config_index, 
                                 full_config, desc->wTotalLength);
        if (ret > 0) {
            *full_size = ret;
            printf("[+] 完整配置描述符获取成功，%d 字节\n", ret);
        }
    }
    
    return ret;
}

/* 获取字符串描述符 */
int get_string_descriptor(int fd, uint8_t string_index, 
                          struct usb_string_descriptor *desc, 
                          size_t buffer_size) {
    if (string_index == 0) {
        printf("[!] 字符串索引为 0，跳过\n");
        return 0;
    }
    
    printf("[*] 获取字符串描述符 (索引=%d)...\n", string_index);
    
    int ret = usb_get_descriptor(fd, USB_DT_STRING, string_index, 
                                 desc, buffer_size);
    if (ret < 0) {
        return -1;
    }
    
    printf("[+] 字符串描述符获取成功，%d 字节\n", ret);
    return ret;
}

/* 获取语言 ID 列表 */
int get_language_ids(int fd, uint16_t *langs, int max_langs) {
    uint8_t buffer[256];
    
    printf("[*] 获取支持的语言 ID...\n");
    
    int ret = usb_get_descriptor(fd, USB_DT_STRING, 0, buffer, sizeof(buffer));
    if (ret < 0) {
        return -1;
    }
    
    if (ret < 4) {
        printf("[!] 语言 ID 列表太短\n");
        return -1;
    }
    
    int num_langs = (buffer[0] - 2) / 2;
    printf("[+] 支持 %d 种语言\n", num_langs);
    
    if (num_langs > max_langs) {
        num_langs = max_langs;
    }
    
    for (int i = 0; i < num_langs; i++) {
        langs[i] = buffer[2 + i * 2] | (buffer[3 + i * 2] << 8);
        printf("    语言 ID: 0x%04x\n", langs[i]);
    }
    
    return num_langs;
}

/* 打印用法 */
void print_usage(const char *prog) {
    printf("用法: %s <bus> <device> [选项]\n", prog);
    printf("   或: %s <设备路径>\n", prog);
    printf("\n示例:\n");
    printf("  %s 001 002              # 总线 1, 设备 2\n", prog);
    printf("  %s /dev/bus/usb/001/002 # 直接指定设备路径\n", prog);
    printf("\n选项:\n");
    printf("  -d, --device      仅获取设备描述符\n");
    printf("  -c, --config      获取配置描述符\n");
    printf("  -s, --string      获取字符串描述符\n");
    printf("  -a, --all         获取所有描述符\n");
    printf("  -r, --reset-only  仅重置设备，不获取描述符\n");
    printf("  -h, --help        显示帮助\n");
    printf("\n注意：需要 root 权限运行\n");
}

int main(int argc, char *argv[]) {
    char device_path[256];
    int fd;
    int ret;
    
    /* 解析参数 */
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    /* 检查是否是帮助 */
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    /* 构建设备路径 */
    if (argc >= 3) {
        /* 使用 bus 和 device 参数 */
        snprintf(device_path, sizeof(device_path), 
                 "/dev/bus/usb/%s/%s", argv[1], argv[2]);
    } else {
        /* 直接使用设备路径 */
        strncpy(device_path, argv[1], sizeof(device_path) - 1);
        device_path[sizeof(device_path) - 1] = '\0';
    }
    
    /* 确定要获取的描述符类型 */
    int get_device = 0;
    int get_config = 0;
    int get_string = 0;
    int reset_only = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
            get_device = 1;
            get_config = 0;
            get_string = 0;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
            get_config = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--string") == 0) {
            get_string = 1;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            get_device = 1;
            get_config = 1;
            get_string = 1;
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--reset-only") == 0) {
            reset_only = 1;
        }
    }
    
    printf("========================================\n");
    printf("USB 描述符获取工具\n");
    printf("========================================\n");
    printf("设备路径: %s\n", device_path);
    
    /* 打开设备 */
    fd = open(device_path, O_RDWR);
    if (fd < 0) {
        perror("打开设备失败");
        printf("\n提示:\n");
        printf("  1. 确保设备已连接\n");
        printf("  2. 使用 lsusb 查看设备路径\n");
        printf("  3. 需要 root 权限 (sudo)\n");
        return 1;
    }
    printf("[+] 设备打开成功 (fd=%d)\n", fd);
    
    /* 重置设备 */
    if (usb_reset_device(fd) < 0) {
        printf("[!] 设备重置失败，但继续尝试获取描述符...\n");
        /* 不退出，继续尝试 */
    }
    
    /* 如果只需要重置 */
    if (reset_only) {
        printf("[+] 操作完成\n");
        close(fd);
        return 0;
    }
    
    /* 获取设备描述符 */
    if (get_device) {
        struct usb_device_descriptor desc;
        memset(&desc, 0, sizeof(desc));
        
        ret = get_device_descriptor(fd, &desc);
        if (ret > 0) {
            print_device_descriptor(&desc);
            
            /* 打印原始数据 */
            printf("\n原始数据:\n");
            print_hex("  ", (uint8_t*)&desc, ret);
        } else {
            printf("[!] 获取设备描述符失败\n");
        }
    }
    
    /* 获取配置描述符 */
    if (get_config) {
        struct usb_config_descriptor desc;
        uint8_t full_config[4096];
        uint16_t full_size = 0;
        
        ret = get_config_descriptor(fd, 0, &desc, full_config, &full_size);
        if (ret > 0) {
            print_config_descriptor(&desc);
            
            if (full_size > 0) {
                printf("\n完整配置数据:\n");
                print_hex("  ", full_config, full_size);
            }
        } else {
            printf("[!] 获取配置描述符失败\n");
        }
    }
    
    /* 获取字符串描述符 */
    if (get_string) {
        uint16_t langs[16];
        int num_langs = get_language_ids(fd, langs, 16);
        
        if (num_langs > 0) {
            /* 使用第一个语言 ID 获取字符串 */
            uint8_t buffer[256];
            struct usb_device_descriptor dev_desc;
            
            /* 先获取设备描述符以获取字符串索引 */
            ret = get_device_descriptor(fd, &dev_desc);
            if (ret > 0) {
                /* 获取制造商字符串 */
                if (dev_desc.iManufacturer > 0) {
                    ret = usb_get_descriptor(fd, USB_DT_STRING, 
                                           dev_desc.iManufacturer,
                                           buffer, sizeof(buffer));
                    if (ret > 0) {
                        print_string_descriptor((struct usb_string_descriptor*)buffer);
                    }
                }
                
                /* 获取产品字符串 */
                if (dev_desc.iProduct > 0) {
                    ret = usb_get_descriptor(fd, USB_DT_STRING,
                                           dev_desc.iProduct,
                                           buffer, sizeof(buffer));
                    if (ret > 0) {
                        print_string_descriptor((struct usb_string_descriptor*)buffer);
                    }
                }
                
                /* 获取序列号 */
                if (dev_desc.iSerialNumber > 0) {
                    ret = usb_get_descriptor(fd, USB_DT_STRING,
                                           dev_desc.iSerialNumber,
                                           buffer, sizeof(buffer));
                    if (ret > 0) {
                        print_string_descriptor((struct usb_string_descriptor*)buffer);
                    }
                }
            }
        }
    }
    
    /* 关闭设备 */
    close(fd);
    
    printf("\n========================================\n");
    printf("操作完成\n");
    printf("========================================\n");
    
    return 0;
}
