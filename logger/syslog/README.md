

# syslog

## syslog

``` C
#include <syslog.h>

void syslog(int priority, const char *format, ...);
```
> 记录日志消息，如果priority省略，则会用openlog()调用指定的 facility 值，如果也省略，默认为LOG_USER。

syslog中的priority参数的level值如下(严重性从最高到最低)

| 值 | 描述 |
|:--:|:----:|
| LOG_EMERG | 紧急或令人恐慌的情况 |
| LOG_ALERT | 需要立即处理的情况(如破坏了系统数据库) |
| LOG_CRIT  | 关键情况 |
| LOG_ERR   | 常规错误 |
| LOG_WARNING | 警告 |
| LOG_NOTICE | 可能需要处理的普通情况 |
| LOG_INFO | 情报性消息 |
| LOG_DEBUG | 调试消息 |


## ubuntu18.04系统测试

### 日志文件配置

>file: `sudo vi /etc/rsyslog.conf`

```
# Test log
local2.* /var/log/TestLog
```

### 重启rsyslog服务

```
service rsyslog restart
```
