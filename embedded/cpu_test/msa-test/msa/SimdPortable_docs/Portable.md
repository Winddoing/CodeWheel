AbsDifferenceSum
================

* Files

>SimdMsaAbsDifferenceSum.cpp

* Formula

$$ sum = \sum_{i=0}^{w*h - 1} |a - b|_i $$

* Code

``` c++
	void AbsDifferenceSum(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride,
		size_t width, size_t height, uint64_t * sum);
```

AbsDifferenceSumMasked
======================

* Files

>SimdMsaAbsDifferenceSum.cpp

* Formula

$$ sum = \sum_{i=0}^{w*h - 1}{(\left| a - b \right|_i  \land m_i )} $$

$$ m_i = \begin{cases}
			0xff    & \text {if $index_i = mask_i$} \\\
			0x00    & \text {if $index_i \neq mask_i $}
		 \end{cases} $$

* Code

``` c++
	void AbsDifferenceSumMasked(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride,
		const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sum)
```


AbsDifferenceSums3x3
====================

* Files

>SimdMsaAbsDifferenceSum.cpp

* Formula

$$ sum = \sum_{n=0}^{w*h - 1} v_n$$

$$ v=\sum_{k=1}^2 h_k $$

$$ h=\sum_{i=0}^2 \left|current_n - background_i \right|$$

* Code

``` c++
	void AbsDifferenceSums3x3(const uint8_t * current, size_t currentStride,
		const uint8_t * background, size_t backgroundStride, size_t width, size_t height, uint64_t * sums)
```

AbsDifferenceSums3x3Masked
==========================

* Files

>SimdMsaAbsDifferenceSum.cpp

* Formula

$$ sum = \sum_{n=0}^{w * h -1} v_n $$

$$ v=\sum_{j=0}^2 h_j $$

$$ h=\sum_{i=0}^2 \left|current_n - background_i \right| \land m_n $$

$$ m_i =\begin{cases}
				0xff    & \text {if $index_i = mask_i$} \\\
				0x00    & \text {if $index_i \neq mask_i $}
		\end{cases} $$

* Code

``` c++
	void AbsDifferenceSums3x3Masked(const uint8_t *current, size_t currentStride, const uint8_t *background, size_t backgroundStride,
		const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sums)
```

SquaredDifferenceSum
====================

* Files

>SimdMsaSquaredDifferenceSum.cpp

* Formula

$$ sum = \sum_{i=0}^{w*h - 1} |a - b|_i^2 $$

* Code

``` c++
void SquaredDifferenceSum(
			const uint8_t * a, size_t aStride, const uint8_t * b, size_t bStride,
			size_t width, size_t height, uint64_t * sum)
```

SquaredDifferenceSumMasked
==========================

* Files

>SimdMsaSquaredDifferenceSum.cpp

* Formula

$$ sum = \sum_{i=0}^{w*h - 1}{(|a - b|_i^2  \land m_i )} $$

$$ m_i =
			\begin{cases}
				0xff    & \text {if $index_i = mask_i$} \\\
				0x00    & \text {if $index_i \neq mask_i $}
			\end{cases} $$


* Code

``` c++
void SquaredDifferenceSumMasked(
			const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride,
			const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sum)
```

SquaredDifferenceSum32f
=======================

* Files

>SimdMsaSquaredDifferenceSum.cpp

* Formula

$$ sum = \sum_{i=0}^{size} |a - b|_i^2 $$

* Code

``` c++
void SquaredDifferenceSum32f(const float * a, const float * b, size_t size, float * sum)
```
SquaredDifferenceKahanSum32f
============================

* Files

>SimdMsaSquaredDifferenceSum.cpp

* Formula

$$ sum = \sum_{i=0}^{size} |a - b|_i^2 $$

* Code

``` c++
void SquaredDifferenceKahanSum32f(const float * a, const float * b, size_t size, float * sum)
```

Kahan求和公式原理：
----------------

>   kahan求和算法能避免大数吃小数的情况。

>>   大数吃小数是什么意思呢？举个例子，我们用两个float相加，float是32位，它的精度是小数点后6-7位
(详见http://blog.csdn.net/zhangpinghao/article/details/8138732），设有a=123456;b=2.189；
a+b应该是123458.189但是由于float的精度只有小数点后6-7位，所以必然得不到123458.189，
后面的89可能会截掉，8不一定，9是必然会截掉的。好的，才做一个加法就产生至少了0.009的误差，做1000个这样的加法，
误差就是9了，这显然不是我们想要的。

>   kahan求和算法可以避免这种情况，它有一个数用来记住那个被截断的小数，同样做下面的计算，
设有a=123456;b=2.189；计算a+b。kahan求和算法是这样做的：sum=a+b（不准确）;temp= (a+b)-a-b;
temp等于多少呢，初看这不就是0吗？不是的，计算机此时算的可不是0，而是等于-0.009，
就是被截断的那个小数。通过一个临时变量我们就记住了这个误差，当计算下一个加法的时候，
可以把这个误差补上，并且更新误差到sum。

>  其实也可以这样理解，sum不是由于数太大，占用了小数的精度吗，而这个小数在当前一步看似是可以忽略的，
但是由于，迭代的次数旁道，小数会累积成大误差，那么我们另外用的float专
门记住这个误差小数不就得了吗。

详细的可以看维基百科:
http://en.wikipedia.org/wiki/Kahan_summation_algorithm

``` basic
function KahanSum(input)
    var sum = 0.0
    var c = 0.0          //A running compensation for lost low-order bits.
    for i = 1 to input.length do
        y = input[i] - c    //So far, so good: c is zero.
        t = sum + y         //Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y   //(t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t             //Algebraically, c should always be zero. Beware eagerly optimising compilers!
        //Next time around, the lost low part will be added to y in a fresh attempt.
    return sum
```


AddFeatureDifference
============================
	This function is used for difference estimation in algorithm of motion detection.

* Files

>SimdMsaSquaredDifferenceSum.cpp

* Formula

>>excess = max(lo[i] - value[i], 0) + max(value[i] - hi[i], 0);
>>difference[i] += (weight * excess*excess) >> 16;

* Code

``` c++
void AddFeatureDifference(const uint8_t * value, size_t valueStride, size_t width, size_t height,
	const uint8_t * lo, size_t loStride, const uint8_t * hi, size_t hiStride,
	uint16_t weight, uint8_t * difference, size_t differenceStride)
```
