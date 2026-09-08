
# `libusb_hook`

## `LD_PRELOAD` 的作用

`LD_PRELOAD`环境变量指示动态链接器在加载程序所依赖的共享库之前，优先加载指定的共享库。这使得我们可以用自己的函数版本覆盖（override）程序调用的标准库函数，从而实现函数拦截（hooking）。


## 构造函数`(__attribute__((constructor)))`

这是一个 GCC（及兼容编译器）的扩展属性，用于标记一个函数为共享库的初始化函数。当共享库被加载时，这些构造函数会自动执行（在 main 之前）。构造函数常用于执行初始化操作，例如本例中打印一条确认信息或初始化真实函数指针。


```shell
export LD_PRELOAD=./libusb_hook.so
```
动态链接器（ld-linux.so）在启动新进程时，会读取环境变量`LD_PRELOAD`，加载指定的钩子库到新进程的地址空间。随后，钩子库的构造函数被调用。因此，无论运行什么程序，只要该程序是动态链接的（绝大多数都是），且`LD_PRELOAD`指向你的钩子库，构造函数就会执行。

> 因此，`export`后在当前终端运行如何程序，都会加载一次`LD_PRELOAD`指定的钩子库，构造函数也就会被执行。

最好的用法：`export LD_PRELOAD=./libusb_hook.so ./a.out`

## RTLD_NEXT

`RTLD_NEXT` 是 `dlsym` 函数的一个特殊**伪句柄**，它并不是指向某个具体的共享库文件，而是告诉动态链接器：“请返回在**当前库之后**、按照共享库搜索顺序找到的**下一个**同名符号的地址。” 这种机制主要用于实现函数拦截（hooking），特别是在通过 `LD_PRELOAD` 预加载的自定义库中，需要调用原始库函数时使用。

``` c
int libusb_init(libusb_context **ctx) {
    // 获取原始 libusb_init 的地址
    typeof(libusb_init) *real_libusb_init = dlsym(RTLD_NEXT, "libusb_init");
    // 调用原始函数
    int ret = real_libusb_init(ctx);
    // 记录日志...
    return ret;
}
```

`RTLD_NEXT`不是共享库路径，而是一个特殊的伪句柄常量（通常定义为`((void *) -1l)`），由动态链接器内部处理。

```c
#ifndef RTLD_NEXT
#define RTLD_NEXT ((void *) -1l)
#endif
```

`RTLD_NEXT`的工作原理

- 动态链接器维护一个共享库的搜索顺序（通常包括可执行文件、`LD_PRELOAD`指定的库、系统库路径等）。
- 当使用`dlsym(RTLD_NEXT, "func")`时，链接器会从当前调用库之后的库开始查找名为 "func" 的符号，并返回第一个找到的地址。
- 因此，在钩子库（当前库）内部调用 `dlsym(RTLD_NEXT, ...)`会跳过当前库，找到下一个包含该符号的库（例如真正的 libusb-1.0.so），从而得到原始函数指针。

与`RTLD_DEFAULT`的区别
- `RTLD_DEFAULT`：查找整个搜索顺序中第一个匹配的符号。在钩子库中使用 `RTLD_DEFAULT` 会返回钩子库自身的函数地址（因为当前库是第一个匹配的），无法获得原始函数。
- `RTLD_NEXT`：查找下一个匹配的符号，即跳过了当前库，适合钩子场景。
