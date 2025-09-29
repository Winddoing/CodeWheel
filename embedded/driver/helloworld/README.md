

## 编译

```
make
```

- arm64
```
make CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64
make CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 clean
```

## 测试

```
sudo insmod hello.ko
```

```
dmesg
```

```
sudo rmmod hello
```
