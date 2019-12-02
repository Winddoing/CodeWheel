
## 简单的图片格式转换

1. 打开看图软件（ubuntu eog）
2. 菜单选择另存为
3. 重命名，选择保存格式（bmp等）保存即可

## 测试文件

- https://raw.githubusercontent.com/leixiaohua1020/simplest_media_play/master/test_bgra_320x180.rgb


## convert

```
convert -depth 8 -size 320x180+0 bgra:test_bgra_320x180.rgb out.png
```

```
convert test.png test.bmp
```

```
display test.bmp
```


## 参考

- [最简单的视音频播放示例7：SDL2播放RGB/YUV](https://blog.csdn.net/leixiaohua1020/article/details/40525591/)
- https://github.com/leixiaohua1020/simplest_media_play


