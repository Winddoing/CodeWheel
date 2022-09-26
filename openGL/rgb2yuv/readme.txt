
https://gitlab.freedesktop.org/mkedzier/rgb2yuv.git


```
sudo apt install libepoxy-dev
```


```
sudo ./rgb2nv12.out 1920 1080 desktop.bmp 1
```

由于bmp格式的文件转换为NV12后，~生成的图像是镜像文件，因为bmp中保存的数据格式rgb, 而该demo是将rgba转NV12~

生成的图像是以水平方向上下颠倒的方式显示，主要是由于读取bmp文件时的方式不对造成的，现在测试正常可以正常显示
