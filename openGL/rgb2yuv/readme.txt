
https://gitlab.freedesktop.org/mkedzier/rgb2yuv.git


```
sudo apt install libepoxy-dev
```


```
sudo ./rgb2nv12.out 1920 1080 desktop.bmp 1
```

由于bmp格式的文件转换为NV12后，生成的图像是镜像文件，因为bmp中保存的数据格式rgb, 而该demo是将rgba转NV12。
