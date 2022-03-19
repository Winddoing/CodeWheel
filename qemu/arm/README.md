
# QEMU模拟调试裸机ARM64汇编


## 安装Qemu

```
sudo apt install qemu-system-arm gdb-multiarch
```

## 运行

```
qemu-system-aarch64 -machine raspi3 -m 256M -serial stdio -kernel test.elf -S -s
```

## 调试


- 开启GDB
```
gdb-multiarch test.elf
```

- 连接Qemu
```
(gdb) target remote localhost:1234
```


## vscode

打开当前工作区可以直接使用

- 编译
 - 快捷键`ctrl+shift+B`，选择`Build`
 
- 启动Qemu
 - 快捷键`ctrl+shift+B`，选择`Boot Qemu`

- 启动gdb调试
 - `F5`启动
