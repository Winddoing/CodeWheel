
## Section “ServerLayout”

```
Option  "Xinerama"  "on"  #扩展桌面，off则为个体桌面
```


## Section “Monitor”

```
ModeLine    "..."  #指定显示器的显示模式
```



## Section "Device"

```
Option      "ConnectedMonitor" "DFP" #强制让系统认为你连接了一个显示器
```









## 命令

``` shell
$gtf 1920 1080 60

  # 1920x1080 @ 60.00 Hz (GTF) hsync: 67.08 kHz; pclk: 172.80 MHz
    Modeline "1920x1080_60.00"  172.80  1920 2040 2248 2576  1080 1081 1084 1118  -HSync +Vsync
```
