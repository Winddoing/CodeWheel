建议：
	当前指令是二维结构及vector int,是否可以像arm一样增加3维指令．

1. RgbToGray
	三字节转换到１字节.
	由于Load时字节序是挨着的，用shuffle还需要填写一组序列，这样会多Load一组数据．
	RgbaToGray
	RgbToBayer
	BayerToRgb
	BayerToRgba
	BgrToBgra

2. Int16toGray
   需要16位有符号到8bit无符号饱和处理，就是需要饱和位数时有符号转换．
   现在使用max min两条质量解决．

3. 关于Pack指令
   Interleave: even odd, left right
   Pack:       even odd
   也需要left,right.

4.
