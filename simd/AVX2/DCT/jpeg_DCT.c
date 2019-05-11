#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "jpeg_DCT.h"

#if __x86_64__
#include "immintrin.h"
#include "emmintrin.h"
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SHIFT16 16

#if __x86_64__
#define barrier() __asm__ __volatile__("": : :"memory")
void fastDCT_x86(unsigned char *image_data, short *dct_data)
{
	int i, j, k, l;
	int temp_data1[64], temp_data2[64];
	__m256i c1 = _mm256_set1_epi32(0x1F62);   // Q13   coeff
	__m256i c3 = _mm256_set1_epi32(0x1A9B);   // Q13   coeff
	__m256i c5 = _mm256_set1_epi32(0x11C7);   // Q13   coeff
	__m256i c7 = _mm256_set1_epi32(0x063E);   // Q13   coeff
	__m256i c2 = _mm256_set1_epi32(0x29CF);   // Q13.5 coeff
	__m256i c6 = _mm256_set1_epi32(0x1151);   // Q13.5 coeff
	__m256i C1 = _mm256_set1_epi32(0xFB15);   // Q16   coeff
	__m256i C3 = _mm256_set1_epi32(0xD4DB);   // Q16   coeff
	__m256i C5 = _mm256_set1_epi32(0x8E3A);   // Q16   coeff
	__m256i C7 = _mm256_set1_epi32(0x31F1);   // Q16   coeff
	__m256i C2 = _mm256_set1_epi32(0xA73D);   // Q15.5 coeff
	__m256i C6 = _mm256_set1_epi32(0x4546);   // Q15.5 coeff
	__m256i C4 = _mm256_set1_epi32(0xB505);   // Q16   coeff

	__m256i p_128 = _mm256_set1_epi32(128);
	__m256i p_4 = _mm256_set1_epi32(4);
	__m256i p_32768 = _mm256_set1_epi32(32768);

	__m256i f0, f1, f2, f3, f4, f5, f6, f7;
	__m256i o0, o1, o2, o3, o4, o5, o6, o7;
	__m256i ff0, ff1, ff2, ff3, ff4, ff5, ff6, ff7;
	__m256i g0, g1, h0, h1;   // Even-half intermediate.
	__m256i p0, p1;           // Even-half intermediate.
	__m256i r0, r1, r0_, r1_;  // Even-half intermediate.
	__m256i P0, P1, R0, R1;   // Even-half intermediate.
	__m256i g2, g3, h2, h3;   // Odd-half intermediate.
	__m256i q1a, s1a, q0, q1;   // Odd-half intermediate.
	__m256i s0, s1;           // Odd-half intermediate.
	__m256i Q0, Q1, S0, S1;   // Odd-half intermediate.
	__m256i F[8], G[8];

	__m256i gg0, gg1, gg2, gg3, hh0, hh1, hh2, hh3;
	__m256i pp0, pp1, rr0, rr1, qq1a, ss1a, qq0, qq1, ss1, ss0;
	__m256i PP0, PP1, RR1, RR0, QQ0, QQ1, SS0, SS1;

	f0 = _mm256_cvtepu8_epi16(*(((__m128i*)image_data) + 0));
	f1 = _mm256_cvtepu8_epi16(*(((__m128i*)image_data) + 1));
	f2 = _mm256_cvtepu8_epi16(*(((__m128i*)image_data) + 2));
	f3 = _mm256_cvtepu8_epi16(*(((__m128i*)image_data) + 3));

	o0 = _mm256_cvtepi16_epi32(*((__m128i*)&f0));
	o1 = _mm256_cvtepi16_epi32(*(((__m128i*)&f0) + 1));
	o2 = _mm256_cvtepi16_epi32(*((__m128i*)&f1));
	o3 = _mm256_cvtepi16_epi32(*(((__m128i*)&f1) + 1));
	o4 = _mm256_cvtepi16_epi32(*((__m128i*)&f2));
	o5 = _mm256_cvtepi16_epi32(*(((__m128i*)&f2) + 1));
	o6 = _mm256_cvtepi16_epi32(*((__m128i*)&f3));
	o7 = _mm256_cvtepi16_epi32(*(((__m128i*)&f3) + 1));

	/*
	for (i = 0; i < 64; i++) {
	    temp_data1[i] = (int) image_data[i];
	}
	o0 = _mm256_lddqu_si256((__m256i *)(temp_data1+0));
	o1 = _mm256_lddqu_si256((__m256i *)(temp_data1+8));
	o2 = _mm256_lddqu_si256((__m256i *)(temp_data1+16));
	o3 = _mm256_lddqu_si256((__m256i *)(temp_data1+24));
	o4 = _mm256_lddqu_si256((__m256i *)(temp_data1+32));
	o5 = _mm256_lddqu_si256((__m256i *)(temp_data1+40));
	o6 = _mm256_lddqu_si256((__m256i *)(temp_data1+48));
	o7 = _mm256_lddqu_si256((__m256i *)(temp_data1+56));
	*/
	f0 = _mm256_sub_epi32(o0, p_128);
	f1 = _mm256_sub_epi32(o1, p_128);
	f2 = _mm256_sub_epi32(o2, p_128);
	f3 = _mm256_sub_epi32(o3, p_128);
	f4 = _mm256_sub_epi32(o4, p_128);
	f5 = _mm256_sub_epi32(o5, p_128);
	f6 = _mm256_sub_epi32(o6, p_128);
	f7 = _mm256_sub_epi32(o7, p_128);

	g0 = _mm256_add_epi32(f0, f7);
	g1 = _mm256_add_epi32(f1, f6);
	h1 = _mm256_add_epi32(f2, f5);
	h0 = _mm256_add_epi32(f3, f4);
	g2 = _mm256_sub_epi32(f3, f4);
	g3 = _mm256_sub_epi32(f2, f5);
	h3 = _mm256_sub_epi32(f1, f6);
	h2 = _mm256_sub_epi32(f0, f7);

	p0 = _mm256_add_epi32(g0, h0);
	p1 = _mm256_add_epi32(g1, h1);
	r0 = _mm256_sub_epi32(g0, h0);
	r1 = _mm256_sub_epi32(g1, h1);

	q1a = _mm256_add_epi32(g2, g2);
	s1a = _mm256_add_epi32(h2, h2);

	q1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_mullo_epi32(q1a, C4), p_32768), SHIFT16);
	s1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_mullo_epi32(s1a, C4), p_32768), SHIFT16);

	s0 = _mm256_add_epi32(h3, g3);
	q0 = _mm256_sub_epi32(h3, g3);

	P0 = _mm256_add_epi32(p0, p1);
	P1 = _mm256_sub_epi32(p0, p1);

	r0_ = _mm256_add_epi32(r0, r0);               //  r0_ is now Q2
	r1_ = _mm256_add_epi32(r1, r1);               //  r1_ is now Q2
	R1 = _mm256_srai_epi32(
			 _mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(C6, r1_), _mm256_mullo_epi32(C2, r0_)), p_32768),
			 SHIFT16);
	R0 = _mm256_srai_epi32(
			 _mm256_add_epi32(_mm256_sub_epi32(_mm256_mullo_epi32(C6, r0_), _mm256_mullo_epi32(C2, r1_)), p_32768),
			 SHIFT16);

	Q1 = _mm256_add_epi32(q1, q0);               //  Results in Q1.5
	S1 = _mm256_add_epi32(s1, s0);               //  Results in Q1.5
	Q0 = _mm256_sub_epi32(q1, q0);               //  Results in Q1.5
	S0 = _mm256_sub_epi32(s1, s0);               //  Results in Q1.5

	F[0] = P0;                    //  Results in Q1.5
	F[4] = P1;                    //  Results in Q1.5
	F[2] = R1;                    //  Results in Q1.5
	F[6] = R0;                    //  Results in Q1.5

	F[1] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(C7, Q1), _mm256_mullo_epi32(C1, S1)), p_32768),
			   SHIFT16);
	F[7] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_sub_epi32(_mm256_mullo_epi32(C7, S1), _mm256_mullo_epi32(C1, Q1)), p_32768),
			   SHIFT16);
	F[5] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(C3, Q0), _mm256_mullo_epi32(C5, S0)), p_32768),
			   SHIFT16);
	F[3] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_sub_epi32(_mm256_mullo_epi32(C3, S0), _mm256_mullo_epi32(C5, Q0)), p_32768),
			   SHIFT16);

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++) temp_data2[j * 8 + i] = *(((int *)(F + i)) + j);

	ff0 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 0));
	ff1 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 8));
	ff2 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 16));
	ff3 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 24));
	ff4 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 32));
	ff5 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 40));
	ff6 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 48));
	ff7 = _mm256_lddqu_si256((__m256i *)(temp_data2 + 56));

	gg0 = _mm256_add_epi32(ff0, ff7);
	gg1 = _mm256_add_epi32(ff1, ff6);
	hh1 = _mm256_add_epi32(ff2, ff5);
	hh0 = _mm256_add_epi32(ff3, ff4);
	gg2 = _mm256_sub_epi32(ff3, ff4);
	gg3 = _mm256_sub_epi32(ff2, ff5);
	hh3 = _mm256_sub_epi32(ff1, ff6);
	hh2 = _mm256_sub_epi32(ff0, ff7);

	pp0 = _mm256_add_epi32(_mm256_add_epi32(gg0, hh0), p_4);
	pp1 = _mm256_add_epi32(gg1, hh1);
	rr0 = _mm256_sub_epi32(gg0, hh0);
	rr1 = _mm256_sub_epi32(gg1, hh1);

	qq1a = _mm256_srai_epi32(_mm256_add_epi32(_mm256_mullo_epi32(gg2, C4), p_32768), SHIFT16);
	ss1a = _mm256_srai_epi32(_mm256_add_epi32(_mm256_mullo_epi32(hh2, C4), p_32768), SHIFT16);
	qq1 = _mm256_add_epi32(qq1a, qq1a);
	ss1 = _mm256_add_epi32(ss1a, ss1a);

	ss0 = _mm256_add_epi32(hh3, gg3);
	qq0 = _mm256_sub_epi32(hh3, gg3);

	PP0 = _mm256_srai_epi32(_mm256_add_epi32(pp0, pp1), 3);
	PP1 = _mm256_srai_epi32(_mm256_sub_epi32(pp0, pp1), 3);
	RR1 = _mm256_srai_epi32(
			  _mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(c6, rr1), _mm256_mullo_epi32(c2, rr0)), p_32768),
			  SHIFT16);
	RR0 = _mm256_srai_epi32(
			  _mm256_add_epi32(_mm256_sub_epi32(_mm256_mullo_epi32(c6, rr0), _mm256_mullo_epi32(c2, rr1)), p_32768),
			  SHIFT16);

	QQ1 = _mm256_add_epi32(qq1, qq0);               //  Results in Q1.5
	QQ0 = _mm256_sub_epi32(qq1, qq0);               //  Results in Q1.5
	SS1 = _mm256_add_epi32(ss1, ss0);               //  Results in Q1.5
	SS0 = _mm256_sub_epi32(ss1, ss0);               //  Results in Q1.5

	G[0] = PP0;
	G[4] = PP1;
	G[2] = RR1;
	G[6] = RR0;
	G[1] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(c7, QQ1), _mm256_mullo_epi32(c1, SS1)), p_32768),
			   SHIFT16);
	G[7] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_sub_epi32(_mm256_mullo_epi32(c7, SS1), _mm256_mullo_epi32(c1, QQ1)), p_32768),
			   SHIFT16);
	G[5] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(c3, QQ0), _mm256_mullo_epi32(c5, SS0)), p_32768),
			   SHIFT16);
	G[3] = _mm256_srai_epi32(
			   _mm256_add_epi32(_mm256_sub_epi32(_mm256_mullo_epi32(c3, SS0), _mm256_mullo_epi32(c5, QQ0)), p_32768),
			   SHIFT16);

	for(k = 0; k < 8; k++)
		for(l = 0; l < 8; l++)
			dct_data[l * 8 + k] = (short) * ((int *)(G + k) + l);

	barrier();
}

#endif

void fastDCT(unsigned char *image_data, short *dct_data, unsigned num_fdcts)
{
	// ----------------------------------------------------
	//  Set up the cosine coefficients.
	// ----------------------------------------------------
	const unsigned short c1 = 0x1F62;   // Q13   coeff
	const unsigned short c3 = 0x1A9B;   // Q13   coeff
	const unsigned short c5 = 0x11C7;   // Q13   coeff
	const unsigned short c7 = 0x063E;   // Q13   coeff
	const unsigned short c2 = 0x29CF;   // Q13.5 coeff
	const unsigned short c6 = 0x1151;   // Q13.5 coeff
	const unsigned short C1 = 0xFB15;   // Q16   coeff
	const unsigned short C3 = 0xD4DB;   // Q16   coeff
	const unsigned short C5 = 0x8E3A;   // Q16   coeff
	const unsigned short C7 = 0x31F1;   // Q16   coeff
	const unsigned short C2 = 0xA73D;   // Q15.5 coeff
	const unsigned short C6 = 0x4546;   // Q15.5 coeff
	const unsigned short C4 = 0xB505;   // Q16   coeff

	// ----------------------------------------------------
	//  Intermediate calculations.
	// ----------------------------------------------------
	short f0, f1, f2, f3;   // Spatial domain samples.
	short f4, f5, f6, f7;   // Spatial domain samples.
	short g0, g1, h0, h1;   // Even-half intermediate.
	short p0, p1;           // Even-half intermediate.
	short r0, r1, r0_, r1_;  // Even-half intermediate.
	short P0, P1, R0, R1;   // Even-half intermediate.
	short g2, g3, h2, h3;   // Odd-half intermediate.
	short q1a, s1a, q0, q1;   // Odd-half intermediate.
	short s0, s1;           // Odd-half intermediate.
	short Q0, Q1, S0, S1;   // Odd-half intermediate.
	short F0, F1, F2, F3;   // Freq. domain results.
	short F4, F5, F6, F7;   // Freq. domain results.

	// ----------------------------------------------------
	//  Input and output pointers, loop control.
	// ----------------------------------------------------
	unsigned i, j;
	short *dct_io_ptr;

	// ----------------------------------------------------
	//  Outer vertical loop -- Process each 8x8 block.
	// ----------------------------------------------------
	dct_io_ptr = dct_data;

	for(i = 0; i < num_fdcts; i++) {
		// ------------------------------------------------
		//  Perform Vertical 1-D FDCT on columns within
		//  each block.  The inputs to this pass are in Q0
		//  and the outputs are in Q1.5.
		// ------------------------------------------------
		for(j = 0; j < 8; j++) {
			// --------------------------------------------
			//  Load the spatial-domain samples.
			//  The incoming terms start at Q0 precision.
			// --------------------------------------------
			/*
			f0 = dct_io_ptr[0];
			f1 = dct_io_ptr[8];
			f2 = dct_io_ptr[16];
			f3 = dct_io_ptr[24];
			f4 = dct_io_ptr[32];
			f5 = dct_io_ptr[40];
			f6 = dct_io_ptr[48];
			f7 = dct_io_ptr[56];


			f0 = image_data[0];
			f1 = image_data[8];
			f2 = image_data[16];
			f3 = image_data[24];
			f4 = image_data[32];
			f5 = image_data[40];
			f6 = image_data[48];
			f7 = image_data[56];
			*/
			//所有值减去128，可以认为是简单预测
			f0 = image_data[0] - 128;
			f1 = image_data[8] - 128;
			f2 = image_data[16] - 128;
			f3 = image_data[24] - 128;
			f4 = image_data[32] - 128;
			f5 = image_data[40] - 128;
			f6 = image_data[48] - 128;
			f7 = image_data[56] - 128;

			// --------------------------------------------
			//  Stage 1:  Separate into even & odd halves.
			//
			//  The results of this stage are implicitly
			//  in Q1, since we do not explicitly multiply
			//  by 0.5.
			// --------------------------------------------
			g0 = f0 + f7;               //  Results in Q1
			g1 = f1 + f6;               //  Results in Q1
			h1 = f2 + f5;               //  Results in Q1
			h0 = f3 + f4;               //  Results in Q1
			g2 = f3 - f4;               //  Results in Q1
			g3 = f2 - f5;               //  Results in Q1
			h3 = f1 - f6;               //  Results in Q1
			h2 = f0 - f7;               //  Results in Q1

			// --------------------------------------------
			//  Stage 2
			//
			//  Note, on the odd-half, the results are in
			//  Q1.5 since those values are scaled upwards
			//  by sqrt(2) at this point.
			// --------------------------------------------
			p0 = g0 + h0;               //  Results in Q1
			p1 = g1 + h1;               //  Results in Q1
			r0 = g0 - h0;               //  Results in Q1
			r1 = g1 - h1;               //  Results in Q1

			q1a = g2 + g2;              //  q1a is now Q2
			s1a = h2 + h2;              //  s1a is now Q2
			q1 = (q1a * C4 + 0x8000) >> 16; //  .. in Q1.5
			s1 = (s1a * C4 + 0x8000) >> 16; //  .. in Q1.5

			s0 = h3 + g3;               //  Results in Q1.5
			q0 = h3 - g3;               //  Results in Q1.5

			// --------------------------------------------
			//  Stage 3
			//
			//  Now, the even-half ends up in Q1.5.  On P0
			//  and P1, this happens because the multiply-
			//  by-C4 was canceled with an upward scaling
			//  by sqrt(2).  On R0 and R1, this happens
			//  because C2 and C6 are at Q15.5, and we
			//  scale r0 and r1 to Q2 before we multiply.
			// --------------------------------------------
			P0 = p0 + p1;               //  Results in Q1.5
			P1 = p0 - p1;               //  Results in Q1.5

			r0_ = r0 + r0;               //  r0_ is now Q2
			r1_ = r1 + r1;               //  r1_ is now Q2
			R1 = (C6 * r1_ + C2 * r0_ + 0x8000) >> 16; // Q1.5
			R0 = (C6 * r0_ - C2 * r1_ + 0x8000) >> 16; // Q1.5

			Q1 = q1 + q0;               //  Results in Q1.5
			S1 = s1 + s0;               //  Results in Q1.5
			Q0 = q1 - q0;               //  Results in Q1.5
			S0 = s1 - s0;               //  Results in Q1.5

			// --------------------------------------------
			//  Stage 4
			//  No further changes in Q-point happen here.
			// --------------------------------------------
			F0 = P0;                    //  Results in Q1.5
			F4 = P1;                    //  Results in Q1.5
			F2 = R1;                    //  Results in Q1.5
			F6 = R0;                    //  Results in Q1.5

			F1 = (C7 * Q1 + C1 * S1 + 0x8000) >> 16; // Q1.5
			F7 = (C7 * S1 - C1 * Q1 + 0x8000) >> 16; // Q1.5
			F5 = (C3 * Q0 + C5 * S0 + 0x8000) >> 16; // Q1.5
			F3 = (C3 * S0 - C5 * Q0 + 0x8000) >> 16; // Q1.5

			// --------------------------------------------
			//  Store the frequency domain results.
			//  These values are all at Q1.5 precision.
			// --------------------------------------------
			dct_io_ptr[0] = F0;
			dct_io_ptr[8] = F1;
			dct_io_ptr[16] = F2;
			dct_io_ptr[24] = F3;
			dct_io_ptr[32] = F4;
			dct_io_ptr[40] = F5;
			dct_io_ptr[48] = F6;
			dct_io_ptr[56] = F7;

			dct_io_ptr++;
			image_data++;
		}

		// ------------------------------------------------
		//  Update pointer to next 8x8 FDCT block.
		// ------------------------------------------------
		dct_io_ptr += 56;
		image_data += 56;
	}

	// ----------------------------------------------------
	//  Perform Horizontal 1-D FDCT on each 8x8 block.
	// ----------------------------------------------------
	dct_io_ptr = dct_data;

	for(i = 0; i < 8 * num_fdcts; i++) {
		// ------------------------------------------------
		//  Load the spatial-domain samples.
		//  The incoming terms are at Q1.5 precision from
		//  the first pass.
		// ------------------------------------------------
		f0 = dct_io_ptr[0];
		f1 = dct_io_ptr[1];
		f2 = dct_io_ptr[2];
		f3 = dct_io_ptr[3];
		f4 = dct_io_ptr[4];
		f5 = dct_io_ptr[5];
		f6 = dct_io_ptr[6];
		f7 = dct_io_ptr[7];

		// ------------------------------------------------
		//  Stage 1:  Separate into even and odd halves.
		//
		//  The results of this stage are implicitly in
		//  Q2.5, since we do not explicitly multiply by
		//  0.5.
		// ------------------------------------------------
		g0 = f0 + f7;                   //  Results in Q2.5
		g1 = f1 + f6;                   //  Results in Q2.5
		h1 = f2 + f5;                   //  Results in Q2.5
		h0 = f3 + f4;                   //  Results in Q2.5
		g2 = f3 - f4;                   //  Results in Q2.5
		g3 = f2 - f5;                   //  Results in Q2.5
		h3 = f1 - f6;                   //  Results in Q2.5
		h2 = f0 - f7;                   //  Results in Q2.5

		// ------------------------------------------------
		//  Stage 2
		//
		//  Note, on the odd-half, the results are in Q3
		//  since those values are scaled upwards by
		//  sqrt(2) at this point.  The order of operations
		//  differs in this pass as compared to the first
		//  due to overflow concerns.
		//
		//  We also inject a rounding term into the DC
		//  term which will also round the Nyquist term,
		//  F4.  This trick works despite the fact that we
		//  are technically still at Q2.5 here, since
		//  the step from Q2.5 to Q3 later is done
		//  implicitly, rather than with a multiply. (This
		//  is due to the sqrt(2) terms cancelling on the
		//  P0/P1 butterfly.)
		// ------------------------------------------------
		p0 = g0 + h0 + 4;               //  Results in Q2.5
		p1 = g1 + h1;                   //  Results in Q2.5
		r0 = g0 - h0;                   //  Results in Q2.5
		r1 = g1 - h1;                   //  Results in Q2.5

		q1a = (g2 * C4 + 0x8000) >> 16;  //  q1a now in Q2
		s1a = (h2 * C4 + 0x8000) >> 16;  //  s1a now in Q2
		q1 = q1a + q1a;                 //  Results in Q3
		s1 = s1a + s1a;                 //  Results in Q3

		s0 = h3 + g3;                   //  Results in Q3
		q0 = h3 - g3;                   //  Results in Q3

		// ------------------------------------------------
		//  Stage 3
		//
		//  Now, the even-half becomes Q0.  This happens
		//  on P0 and P1 because the multiply-by-c4 was
		//  canceled with an upward scaling by sqrt(2),
		//  yielding Q3 intermediate values.  The final
		//  >> 3 leaves these at Q0.  On R0 and R1, this
		//  happens because c2 and c6 are at Q13.5,
		//  yielding Q16 intermediate values.  The final
		//  >> 16 then leaves those values at Q0.
		// ------------------------------------------------
		P0 = ((short)(p0 + p1)) >> 3;    //  Results in Q0
		P1 = ((short)(p0 - p1)) >> 3;    //  Results in Q0
		R1 = (c6 * r1 + c2 * r0 + 0x8000) >> 16; // .. Q0
		R0 = (c6 * r0 - c2 * r1 + 0x8000) >> 16; // .. Q0

		Q1 = q1 + q0;                   //  Results in Q3
		Q0 = q1 - q0;                   //  Results in Q3
		S1 = s1 + s0;                   //  Results in Q3
		S0 = s1 - s0;                   //  Results in Q3

		// ------------------------------------------------
		//  Stage 4
		//
		//  Next, the odd-half ends up in Q0. This happens
		//  because our values are in Q3 and our cosine
		//  terms are in Q13, giving us Q16 intermediate
		//  values. The final >> 16 leaves us a Q0 result.
		// ------------------------------------------------
		F0 = P0;                        //  Results in Q0
		F4 = P1;                        //  Results in Q0
		F2 = R1;                        //  Results in Q0
		F6 = R0;                        //  Results in Q0

		F1 = (c7 * Q1 + c1 * S1 + 0x8000) >> 16; // .. Q0
		F7 = (c7 * S1 - c1 * Q1 + 0x8000) >> 16; // .. Q0
		F5 = (c3 * Q0 + c5 * S0 + 0x8000) >> 16; // .. Q0
		F3 = (c3 * S0 - c5 * Q0 + 0x8000) >> 16; // .. Q0

		// ------------------------------------------------
		//  Store the results
		// ------------------------------------------------
		dct_io_ptr[0] = F0;
		dct_io_ptr[1] = F1;
		dct_io_ptr[2] = F2;
		dct_io_ptr[3] = F3;
		dct_io_ptr[4] = F4;
		dct_io_ptr[5] = F5;
		dct_io_ptr[6] = F6;
		dct_io_ptr[7] = F7;

		// ------------------------------------------------
		//  Update pointer to next FDCT row.
		// ------------------------------------------------
		dct_io_ptr += 8;
	}

	return;
}


#define RIGHT_SHIFT(x, n) ((x) >> (n))
#define CONST_BITS 13
#define ONE         ((int32_t) 1)
#define CONST_SCALE (ONE << CONST_BITS)
#define PASS1_BITS  2
#define MULTIPLY(var, const)  ((var) * (const))

/* Descale and correctly round an int32_t value that's scaled by N bits.
 * We assume RIGHT_SHIFT rounds towards minus infinity, so adding
 * the fudge factor is correct for either sign of X.
 */

#define DESCALE(x, n)  RIGHT_SHIFT((x) + (ONE << ((n)-1)), n)

/*
  Unlike our decoder where we approximate the FIXes, we need to use exact
ones here or successive P-frames will drift too much with Reference frame coding
*/
#define FIX_0_211164243 1730
#define FIX_0_275899380 2260
#define FIX_0_298631336 2446
#define FIX_0_390180644 3196
#define FIX_0_509795579 4176
#define FIX_0_541196100 4433
#define FIX_0_601344887 4926
#define FIX_0_765366865 6270
#define FIX_0_785694958 6436
#define FIX_0_899976223 7373
#define FIX_1_061594337 8697
#define FIX_1_111140466 9102
#define FIX_1_175875602 9633
#define FIX_1_306562965 10703
#define FIX_1_387039845 11363
#define FIX_1_451774981 11893
#define FIX_1_501321110 12299
#define FIX_1_662939225 13623
#define FIX_1_847759065 15137
#define FIX_1_961570560 16069
#define FIX_2_053119869 16819
#define FIX_2_172734803 17799
#define FIX_2_562915447 20995
#define FIX_3_072711026 25172

/*
 * Perform the inverse DCT on one block of coefficients.
 */

#if 0
void fastIDCTInit(short block[64], short dst[64]) {
    for (int n = 0; n < 64; n++)
        dst[(n & 0x38) | ((n & 6) >> 1) | ((n & 1) << 2)] = block[n];
}
#else
void fastIDCTInit(short block[64], short dst[64])
{
	char seq[64] = {0, 4, 1, 5, 2, 6, 3, 7, 8, 12, 9, 13, 10, 14, 11, 15, 16, 20, 17, 21, 18, 22, 19, 23, 24, 28, 25, 29, 26, 30, 27, 31, 32, 36, 33, 37,
					34, 38, 35, 39, 40, 44, 41, 45, 42, 46, 43, 47, 48, 52, 49, 53, 50, 54, 51, 55, 56, 60, 57, 61, 58, 62, 59, 63
				   };

	for(int n = 0; n < 64; n++)
		dst[seq[n]] = block[n];
}
#endif

void fastIDCT(short data[JPEG_BLOCK_SIZE * JPEG_BLOCK_SIZE])
{
	int32_t tmp0, tmp1, tmp2, tmp3;
	int32_t tmp10, tmp11, tmp12, tmp13;
	int32_t z1, z2, z3, z4, z5;
	int32_t d0, d1, d2, d3, d4, d5, d6, d7;
	register int16_t *dataptr;
	int rowctr;

	/* Pass 1: process rows. */
	/* Note results are scaled up by sqrt(8) compared to a true IDCT; */
	/* furthermore, we scale the results by 2**PASS1_BITS. */

	dataptr = data;

	for(rowctr = JPEG_BLOCK_SIZE - 1; rowctr >= 0; rowctr--) {
		/* Due to quantization, we will usually find that many of the input
		 * coefficients are zero, especially the AC terms.  We can exploit this
		 * by short-circuiting the IDCT calculation for any row in which all
		 * the AC terms are zero.  In that case each output is equal to the
		 * DC coefficient (with scale factor as needed).
		 * With typical images and quantization tables, half or more of the
		 * row DCT calculations can be simplified this way.
		 */

		int *idataptr = (int *) dataptr;

		/* WARNING: we do the same permutation as MMX idct to simplify the
		   video core */
		d0 = dataptr[0];
		d2 = dataptr[1];
		d4 = dataptr[2];
		d6 = dataptr[3];
		d1 = dataptr[4];
		d3 = dataptr[5];
		d5 = dataptr[6];
		d7 = dataptr[7];

		if((d1 | d2 | d3 | d4 | d5 | d6 | d7) == 0) {
			/* AC terms all zero */
			if(d0) {
				/* Compute a 32 bit value to assign. */
				int16_t dcval = (int16_t)(d0 * (1 << PASS1_BITS));
				int v = (dcval & 0xffff) | ((dcval * (1 << 16)) & 0xffff0000);

				idataptr[0] = v;
				idataptr[1] = v;
				idataptr[2] = v;
				idataptr[3] = v;
			}

			dataptr += JPEG_BLOCK_SIZE;       /* advance pointer to next row */
			continue;
		}

		/* Even part: reverse the even part of the forward DCT. */
		/* The rotator is sqrt(2)*c(-6). */
		{
			if(d6) {
				if(d2) {
					/* d0 != 0, d2 != 0, d4 != 0, d6 != 0 */
					z1 = MULTIPLY(d2 + d6, FIX_0_541196100);
					tmp2 = z1 + MULTIPLY(-d6, FIX_1_847759065);
					tmp3 = z1 + MULTIPLY(d2, FIX_0_765366865);

					tmp0 = (d0 + d4) * CONST_SCALE;
					tmp1 = (d0 - d4) * CONST_SCALE;

					tmp10 = tmp0 + tmp3;
					tmp13 = tmp0 - tmp3;
					tmp11 = tmp1 + tmp2;
					tmp12 = tmp1 - tmp2;

				} else {
					/* d0 != 0, d2 == 0, d4 != 0, d6 != 0 */
					tmp2 = MULTIPLY(-d6, FIX_1_306562965);
					tmp3 = MULTIPLY(d6, FIX_0_541196100);

					tmp0 = (d0 + d4) * CONST_SCALE;
					tmp1 = (d0 - d4) * CONST_SCALE;

					tmp10 = tmp0 + tmp3;
					tmp13 = tmp0 - tmp3;
					tmp11 = tmp1 + tmp2;
					tmp12 = tmp1 - tmp2;
				}

			} else {
				if(d2) {
					/* d0 != 0, d2 != 0, d4 != 0, d6 == 0 */
					tmp2 = MULTIPLY(d2, FIX_0_541196100);
					tmp3 = MULTIPLY(d2, FIX_1_306562965);

					tmp0 = (d0 + d4) * CONST_SCALE;
					tmp1 = (d0 - d4) * CONST_SCALE;

					tmp10 = tmp0 + tmp3;
					tmp13 = tmp0 - tmp3;
					tmp11 = tmp1 + tmp2;
					tmp12 = tmp1 - tmp2;

				} else {
					/* d0 != 0, d2 == 0, d4 != 0, d6 == 0 */
					tmp10 = tmp13 = (d0 + d4) * CONST_SCALE;
					tmp11 = tmp12 = (d0 - d4) * CONST_SCALE;
				}
			}

			/* Odd part per figure 8; the matrix is unitary and hence its
			 * transpose is its inverse.  i0..i3 are y7,y5,y3,y1 respectively.
			 */

			if(d7) {
				if(d5) {
					if(d3) {
						if(d1) {
							/* d1 != 0, d3 != 0, d5 != 0, d7 != 0 */
							z1 = d7 + d1;
							z2 = d5 + d3;
							z3 = d7 + d3;
							z4 = d5 + d1;
							z5 = MULTIPLY(z3 + z4, FIX_1_175875602);

							tmp0 = MULTIPLY(d7, FIX_0_298631336);
							tmp1 = MULTIPLY(d5, FIX_2_053119869);
							tmp2 = MULTIPLY(d3, FIX_3_072711026);
							tmp3 = MULTIPLY(d1, FIX_1_501321110);
							z1 = MULTIPLY(-z1, FIX_0_899976223);
							z2 = MULTIPLY(-z2, FIX_2_562915447);
							z3 = MULTIPLY(-z3, FIX_1_961570560);
							z4 = MULTIPLY(-z4, FIX_0_390180644);

							z3 += z5;
							z4 += z5;

							tmp0 += z1 + z3;
							tmp1 += z2 + z4;
							tmp2 += z2 + z3;
							tmp3 += z1 + z4;

						} else {
							/* d1 == 0, d3 != 0, d5 != 0, d7 != 0 */
							z2 = d5 + d3;
							z3 = d7 + d3;
							z5 = MULTIPLY(z3 + d5, FIX_1_175875602);

							tmp0 = MULTIPLY(d7, FIX_0_298631336);
							tmp1 = MULTIPLY(d5, FIX_2_053119869);
							tmp2 = MULTIPLY(d3, FIX_3_072711026);
							z1 = MULTIPLY(-d7, FIX_0_899976223);
							z2 = MULTIPLY(-z2, FIX_2_562915447);
							z3 = MULTIPLY(-z3, FIX_1_961570560);
							z4 = MULTIPLY(-d5, FIX_0_390180644);

							z3 += z5;
							z4 += z5;

							tmp0 += z1 + z3;
							tmp1 += z2 + z4;
							tmp2 += z2 + z3;
							tmp3 = z1 + z4;
						}

					} else {
						if(d1) {
							/* d1 != 0, d3 == 0, d5 != 0, d7 != 0 */
							z1 = d7 + d1;
							z4 = d5 + d1;
							z5 = MULTIPLY(d7 + z4, FIX_1_175875602);

							tmp0 = MULTIPLY(d7, FIX_0_298631336);
							tmp1 = MULTIPLY(d5, FIX_2_053119869);
							tmp3 = MULTIPLY(d1, FIX_1_501321110);
							z1 = MULTIPLY(-z1, FIX_0_899976223);
							z2 = MULTIPLY(-d5, FIX_2_562915447);
							z3 = MULTIPLY(-d7, FIX_1_961570560);
							z4 = MULTIPLY(-z4, FIX_0_390180644);

							z3 += z5;
							z4 += z5;

							tmp0 += z1 + z3;
							tmp1 += z2 + z4;
							tmp2 = z2 + z3;
							tmp3 += z1 + z4;

						} else {
							/* d1 == 0, d3 == 0, d5 != 0, d7 != 0 */
							tmp0 = MULTIPLY(-d7, FIX_0_601344887);
							z1 = MULTIPLY(-d7, FIX_0_899976223);
							z3 = MULTIPLY(-d7, FIX_1_961570560);
							tmp1 = MULTIPLY(-d5, FIX_0_509795579);
							z2 = MULTIPLY(-d5, FIX_2_562915447);
							z4 = MULTIPLY(-d5, FIX_0_390180644);
							z5 = MULTIPLY(d5 + d7, FIX_1_175875602);

							z3 += z5;
							z4 += z5;

							tmp0 += z3;
							tmp1 += z4;
							tmp2 = z2 + z3;
							tmp3 = z1 + z4;
						}
					}

				} else {
					if(d3) {
						if(d1) {
							/* d1 != 0, d3 != 0, d5 == 0, d7 != 0 */
							z1 = d7 + d1;
							z3 = d7 + d3;
							z5 = MULTIPLY(z3 + d1, FIX_1_175875602);

							tmp0 = MULTIPLY(d7, FIX_0_298631336);
							tmp2 = MULTIPLY(d3, FIX_3_072711026);
							tmp3 = MULTIPLY(d1, FIX_1_501321110);
							z1 = MULTIPLY(-z1, FIX_0_899976223);
							z2 = MULTIPLY(-d3, FIX_2_562915447);
							z3 = MULTIPLY(-z3, FIX_1_961570560);
							z4 = MULTIPLY(-d1, FIX_0_390180644);

							z3 += z5;
							z4 += z5;

							tmp0 += z1 + z3;
							tmp1 = z2 + z4;
							tmp2 += z2 + z3;
							tmp3 += z1 + z4;

						} else {
							/* d1 == 0, d3 != 0, d5 == 0, d7 != 0 */
							z3 = d7 + d3;

							tmp0 = MULTIPLY(-d7, FIX_0_601344887);
							z1 = MULTIPLY(-d7, FIX_0_899976223);
							tmp2 = MULTIPLY(d3, FIX_0_509795579);
							z2 = MULTIPLY(-d3, FIX_2_562915447);
							z5 = MULTIPLY(z3, FIX_1_175875602);
							z3 = MULTIPLY(-z3, FIX_0_785694958);

							tmp0 += z3;
							tmp1 = z2 + z5;
							tmp2 += z3;
							tmp3 = z1 + z5;
						}

					} else {
						if(d1) {
							/* d1 != 0, d3 == 0, d5 == 0, d7 != 0 */
							z1 = d7 + d1;
							z5 = MULTIPLY(z1, FIX_1_175875602);

							z1 = MULTIPLY(z1, FIX_0_275899380);
							z3 = MULTIPLY(-d7, FIX_1_961570560);
							tmp0 = MULTIPLY(-d7, FIX_1_662939225);
							z4 = MULTIPLY(-d1, FIX_0_390180644);
							tmp3 = MULTIPLY(d1, FIX_1_111140466);

							tmp0 += z1;
							tmp1 = z4 + z5;
							tmp2 = z3 + z5;
							tmp3 += z1;

						} else {
							/* d1 == 0, d3 == 0, d5 == 0, d7 != 0 */
							tmp0 = MULTIPLY(-d7, FIX_1_387039845);
							tmp1 = MULTIPLY(d7, FIX_1_175875602);
							tmp2 = MULTIPLY(-d7, FIX_0_785694958);
							tmp3 = MULTIPLY(d7, FIX_0_275899380);
						}
					}
				}

			} else {
				if(d5) {
					if(d3) {
						if(d1) {
							/* d1 != 0, d3 != 0, d5 != 0, d7 == 0 */
							z2 = d5 + d3;
							z4 = d5 + d1;
							z5 = MULTIPLY(d3 + z4, FIX_1_175875602);

							tmp1 = MULTIPLY(d5, FIX_2_053119869);
							tmp2 = MULTIPLY(d3, FIX_3_072711026);
							tmp3 = MULTIPLY(d1, FIX_1_501321110);
							z1 = MULTIPLY(-d1, FIX_0_899976223);
							z2 = MULTIPLY(-z2, FIX_2_562915447);
							z3 = MULTIPLY(-d3, FIX_1_961570560);
							z4 = MULTIPLY(-z4, FIX_0_390180644);

							z3 += z5;
							z4 += z5;

							tmp0 = z1 + z3;
							tmp1 += z2 + z4;
							tmp2 += z2 + z3;
							tmp3 += z1 + z4;

						} else {
							/* d1 == 0, d3 != 0, d5 != 0, d7 == 0 */
							z2 = d5 + d3;

							z5 = MULTIPLY(z2, FIX_1_175875602);
							tmp1 = MULTIPLY(d5, FIX_1_662939225);
							z4 = MULTIPLY(-d5, FIX_0_390180644);
							z2 = MULTIPLY(-z2, FIX_1_387039845);
							tmp2 = MULTIPLY(d3, FIX_1_111140466);
							z3 = MULTIPLY(-d3, FIX_1_961570560);

							tmp0 = z3 + z5;
							tmp1 += z2;
							tmp2 += z2;
							tmp3 = z4 + z5;
						}

					} else {
						if(d1) {
							/* d1 != 0, d3 == 0, d5 != 0, d7 == 0 */
							z4 = d5 + d1;

							z5 = MULTIPLY(z4, FIX_1_175875602);
							z1 = MULTIPLY(-d1, FIX_0_899976223);
							tmp3 = MULTIPLY(d1, FIX_0_601344887);
							tmp1 = MULTIPLY(-d5, FIX_0_509795579);
							z2 = MULTIPLY(-d5, FIX_2_562915447);
							z4 = MULTIPLY(z4, FIX_0_785694958);

							tmp0 = z1 + z5;
							tmp1 += z4;
							tmp2 = z2 + z5;
							tmp3 += z4;

						} else {
							/* d1 == 0, d3 == 0, d5 != 0, d7 == 0 */
							tmp0 = MULTIPLY(d5, FIX_1_175875602);
							tmp1 = MULTIPLY(d5, FIX_0_275899380);
							tmp2 = MULTIPLY(-d5, FIX_1_387039845);
							tmp3 = MULTIPLY(d5, FIX_0_785694958);
						}
					}

				} else {
					if(d3) {
						if(d1) {
							/* d1 != 0, d3 != 0, d5 == 0, d7 == 0 */
							z5 = d1 + d3;
							tmp3 = MULTIPLY(d1, FIX_0_211164243);
							tmp2 = MULTIPLY(-d3, FIX_1_451774981);
							z1 = MULTIPLY(d1, FIX_1_061594337);
							z2 = MULTIPLY(-d3, FIX_2_172734803);
							z4 = MULTIPLY(z5, FIX_0_785694958);
							z5 = MULTIPLY(z5, FIX_1_175875602);

							tmp0 = z1 - z4;
							tmp1 = z2 + z4;
							tmp2 += z5;
							tmp3 += z5;

						} else {
							/* d1 == 0, d3 != 0, d5 == 0, d7 == 0 */
							tmp0 = MULTIPLY(-d3, FIX_0_785694958);
							tmp1 = MULTIPLY(-d3, FIX_1_387039845);
							tmp2 = MULTIPLY(-d3, FIX_0_275899380);
							tmp3 = MULTIPLY(d3, FIX_1_175875602);
						}

					} else {
						if(d1) {
							/* d1 != 0, d3 == 0, d5 == 0, d7 == 0 */
							tmp0 = MULTIPLY(d1, FIX_0_275899380);
							tmp1 = MULTIPLY(d1, FIX_0_785694958);
							tmp2 = MULTIPLY(d1, FIX_1_175875602);
							tmp3 = MULTIPLY(d1, FIX_1_387039845);

						} else {
							/* d1 == 0, d3 == 0, d5 == 0, d7 == 0 */
							tmp0 = tmp1 = tmp2 = tmp3 = 0;
						}
					}
				}
			}
		}
		/* Final output stage: inputs are tmp10..tmp13, tmp0..tmp3 */

		dataptr[0] = (int16_t) DESCALE(tmp10 + tmp3, CONST_BITS - PASS1_BITS);
		dataptr[7] = (int16_t) DESCALE(tmp10 - tmp3, CONST_BITS - PASS1_BITS);
		dataptr[1] = (int16_t) DESCALE(tmp11 + tmp2, CONST_BITS - PASS1_BITS);
		dataptr[6] = (int16_t) DESCALE(tmp11 - tmp2, CONST_BITS - PASS1_BITS);
		dataptr[2] = (int16_t) DESCALE(tmp12 + tmp1, CONST_BITS - PASS1_BITS);
		dataptr[5] = (int16_t) DESCALE(tmp12 - tmp1, CONST_BITS - PASS1_BITS);
		dataptr[3] = (int16_t) DESCALE(tmp13 + tmp0, CONST_BITS - PASS1_BITS);
		dataptr[4] = (int16_t) DESCALE(tmp13 - tmp0, CONST_BITS - PASS1_BITS);

		dataptr += JPEG_BLOCK_SIZE;         /* advance pointer to next row */
	}

	/* Pass 2: process columns. */
	/* Note that we must descale the results by a factor of 8 == 2**3, */
	/* and also undo the PASS1_BITS scaling. */

	dataptr = data;

	for(rowctr = JPEG_BLOCK_SIZE - 1; rowctr >= 0; rowctr--) {
		/* Columns of zeroes can be exploited in the same way as we did with rows.
		 * However, the row calculation has created many nonzero AC terms, so the
		 * simplification applies less often (typically 5% to 10% of the time).
		 * On machines with very fast multiplication, it's possible that the
		 * test takes more time than it's worth.  In that case this section
		 * may be commented out.
		 */

//        d0 = dataptr[JPEG_BLOCK_SIZE * 0];
//        d1 = dataptr[JPEG_BLOCK_SIZE * 1];
//        d2 = dataptr[JPEG_BLOCK_SIZE * 2];
//        d3 = dataptr[JPEG_BLOCK_SIZE * 3];
//        d4 = dataptr[JPEG_BLOCK_SIZE * 4];
//        d5 = dataptr[JPEG_BLOCK_SIZE * 5];
//        d6 = dataptr[JPEG_BLOCK_SIZE * 6];
//        d7 = dataptr[JPEG_BLOCK_SIZE * 7];

		d0 = dataptr[0];
		d1 = dataptr[8];
		d2 = dataptr[16];
		d3 = dataptr[24];
		d4 = dataptr[32];
		d5 = dataptr[40];
		d6 = dataptr[48];
		d7 = dataptr[56];

		/* Even part: reverse the even part of the forward DCT. */
		/* The rotator is sqrt(2)*c(-6). */
		if(d6) {
			if(d2) {
				/* d0 != 0, d2 != 0, d4 != 0, d6 != 0 */
				z1 = MULTIPLY(d2 + d6, FIX_0_541196100);
				tmp2 = z1 + MULTIPLY(-d6, FIX_1_847759065);
				tmp3 = z1 + MULTIPLY(d2, FIX_0_765366865);

				tmp0 = (d0 + d4) * CONST_SCALE;
				tmp1 = (d0 - d4) * CONST_SCALE;

				tmp10 = tmp0 + tmp3;
				tmp13 = tmp0 - tmp3;
				tmp11 = tmp1 + tmp2;
				tmp12 = tmp1 - tmp2;

			} else {
				/* d0 != 0, d2 == 0, d4 != 0, d6 != 0 */
				tmp2 = MULTIPLY(-d6, FIX_1_306562965);
				tmp3 = MULTIPLY(d6, FIX_0_541196100);

				tmp0 = (d0 + d4) * CONST_SCALE;
				tmp1 = (d0 - d4) * CONST_SCALE;

				tmp10 = tmp0 + tmp3;
				tmp13 = tmp0 - tmp3;
				tmp11 = tmp1 + tmp2;
				tmp12 = tmp1 - tmp2;
			}

		} else {
			if(d2) {
				/* d0 != 0, d2 != 0, d4 != 0, d6 == 0 */
				tmp2 = MULTIPLY(d2, FIX_0_541196100);
				tmp3 = MULTIPLY(d2, FIX_1_306562965);

				tmp0 = (d0 + d4) * CONST_SCALE;
				tmp1 = (d0 - d4) * CONST_SCALE;

				tmp10 = tmp0 + tmp3;
				tmp13 = tmp0 - tmp3;
				tmp11 = tmp1 + tmp2;
				tmp12 = tmp1 - tmp2;

			} else {
				/* d0 != 0, d2 == 0, d4 != 0, d6 == 0 */
				tmp10 = tmp13 = (d0 + d4) * CONST_SCALE;
				tmp11 = tmp12 = (d0 - d4) * CONST_SCALE;
			}
		}

		/* Odd part per figure 8; the matrix is unitary and hence its
		 * transpose is its inverse.  i0..i3 are y7,y5,y3,y1 respectively.
		 */
		if(d7) {
			if(d5) {
				if(d3) {
					if(d1) {
						/* d1 != 0, d3 != 0, d5 != 0, d7 != 0 */
						z1 = d7 + d1;
						z2 = d5 + d3;
						z3 = d7 + d3;
						z4 = d5 + d1;
						z5 = MULTIPLY(z3 + z4, FIX_1_175875602);

						tmp0 = MULTIPLY(d7, FIX_0_298631336);
						tmp1 = MULTIPLY(d5, FIX_2_053119869);
						tmp2 = MULTIPLY(d3, FIX_3_072711026);
						tmp3 = MULTIPLY(d1, FIX_1_501321110);
						z1 = MULTIPLY(-z1, FIX_0_899976223);
						z2 = MULTIPLY(-z2, FIX_2_562915447);
						z3 = MULTIPLY(-z3, FIX_1_961570560);
						z4 = MULTIPLY(-z4, FIX_0_390180644);

						z3 += z5;
						z4 += z5;

						tmp0 += z1 + z3;
						tmp1 += z2 + z4;
						tmp2 += z2 + z3;
						tmp3 += z1 + z4;

					} else {
						/* d1 == 0, d3 != 0, d5 != 0, d7 != 0 */
						z2 = d5 + d3;
						z3 = d7 + d3;
						z5 = MULTIPLY(z3 + d5, FIX_1_175875602);

						tmp0 = MULTIPLY(d7, FIX_0_298631336);
						tmp1 = MULTIPLY(d5, FIX_2_053119869);
						tmp2 = MULTIPLY(d3, FIX_3_072711026);
						z1 = MULTIPLY(-d7, FIX_0_899976223);
						z2 = MULTIPLY(-z2, FIX_2_562915447);
						z3 = MULTIPLY(-z3, FIX_1_961570560);
						z4 = MULTIPLY(-d5, FIX_0_390180644);

						z3 += z5;
						z4 += z5;

						tmp0 += z1 + z3;
						tmp1 += z2 + z4;
						tmp2 += z2 + z3;
						tmp3 = z1 + z4;
					}

				} else {
					if(d1) {
						/* d1 != 0, d3 == 0, d5 != 0, d7 != 0 */
						z1 = d7 + d1;
						z3 = d7;
						z4 = d5 + d1;
						z5 = MULTIPLY(z3 + z4, FIX_1_175875602);

						tmp0 = MULTIPLY(d7, FIX_0_298631336);
						tmp1 = MULTIPLY(d5, FIX_2_053119869);
						tmp3 = MULTIPLY(d1, FIX_1_501321110);
						z1 = MULTIPLY(-z1, FIX_0_899976223);
						z2 = MULTIPLY(-d5, FIX_2_562915447);
						z3 = MULTIPLY(-d7, FIX_1_961570560);
						z4 = MULTIPLY(-z4, FIX_0_390180644);

						z3 += z5;
						z4 += z5;

						tmp0 += z1 + z3;
						tmp1 += z2 + z4;
						tmp2 = z2 + z3;
						tmp3 += z1 + z4;

					} else {
						/* d1 == 0, d3 == 0, d5 != 0, d7 != 0 */
						tmp0 = MULTIPLY(-d7, FIX_0_601344887);
						z1 = MULTIPLY(-d7, FIX_0_899976223);
						z3 = MULTIPLY(-d7, FIX_1_961570560);
						tmp1 = MULTIPLY(-d5, FIX_0_509795579);
						z2 = MULTIPLY(-d5, FIX_2_562915447);
						z4 = MULTIPLY(-d5, FIX_0_390180644);
						z5 = MULTIPLY(d5 + d7, FIX_1_175875602);

						z3 += z5;
						z4 += z5;

						tmp0 += z3;
						tmp1 += z4;
						tmp2 = z2 + z3;
						tmp3 = z1 + z4;
					}
				}

			} else {
				if(d3) {
					if(d1) {
						/* d1 != 0, d3 != 0, d5 == 0, d7 != 0 */
						z1 = d7 + d1;
						z3 = d7 + d3;
						z5 = MULTIPLY(z3 + d1, FIX_1_175875602);

						tmp0 = MULTIPLY(d7, FIX_0_298631336);
						tmp2 = MULTIPLY(d3, FIX_3_072711026);
						tmp3 = MULTIPLY(d1, FIX_1_501321110);
						z1 = MULTIPLY(-z1, FIX_0_899976223);
						z2 = MULTIPLY(-d3, FIX_2_562915447);
						z3 = MULTIPLY(-z3, FIX_1_961570560);
						z4 = MULTIPLY(-d1, FIX_0_390180644);

						z3 += z5;
						z4 += z5;

						tmp0 += z1 + z3;
						tmp1 = z2 + z4;
						tmp2 += z2 + z3;
						tmp3 += z1 + z4;

					} else {
						/* d1 == 0, d3 != 0, d5 == 0, d7 != 0 */
						z3 = d7 + d3;

						tmp0 = MULTIPLY(-d7, FIX_0_601344887);
						z1 = MULTIPLY(-d7, FIX_0_899976223);
						tmp2 = MULTIPLY(d3, FIX_0_509795579);
						z2 = MULTIPLY(-d3, FIX_2_562915447);
						z5 = MULTIPLY(z3, FIX_1_175875602);
						z3 = MULTIPLY(-z3, FIX_0_785694958);

						tmp0 += z3;
						tmp1 = z2 + z5;
						tmp2 += z3;
						tmp3 = z1 + z5;
					}

				} else {
					if(d1) {
						/* d1 != 0, d3 == 0, d5 == 0, d7 != 0 */
						z1 = d7 + d1;
						z5 = MULTIPLY(z1, FIX_1_175875602);

						z1 = MULTIPLY(z1, FIX_0_275899380);
						z3 = MULTIPLY(-d7, FIX_1_961570560);
						tmp0 = MULTIPLY(-d7, FIX_1_662939225);
						z4 = MULTIPLY(-d1, FIX_0_390180644);
						tmp3 = MULTIPLY(d1, FIX_1_111140466);

						tmp0 += z1;
						tmp1 = z4 + z5;
						tmp2 = z3 + z5;
						tmp3 += z1;

					} else {
						/* d1 == 0, d3 == 0, d5 == 0, d7 != 0 */
						tmp0 = MULTIPLY(-d7, FIX_1_387039845);
						tmp1 = MULTIPLY(d7, FIX_1_175875602);
						tmp2 = MULTIPLY(-d7, FIX_0_785694958);
						tmp3 = MULTIPLY(d7, FIX_0_275899380);
					}
				}
			}

		} else {
			if(d5) {
				if(d3) {
					if(d1) {
						/* d1 != 0, d3 != 0, d5 != 0, d7 == 0 */
						z2 = d5 + d3;
						z4 = d5 + d1;
						z5 = MULTIPLY(d3 + z4, FIX_1_175875602);

						tmp1 = MULTIPLY(d5, FIX_2_053119869);
						tmp2 = MULTIPLY(d3, FIX_3_072711026);
						tmp3 = MULTIPLY(d1, FIX_1_501321110);
						z1 = MULTIPLY(-d1, FIX_0_899976223);
						z2 = MULTIPLY(-z2, FIX_2_562915447);
						z3 = MULTIPLY(-d3, FIX_1_961570560);
						z4 = MULTIPLY(-z4, FIX_0_390180644);

						z3 += z5;
						z4 += z5;

						tmp0 = z1 + z3;
						tmp1 += z2 + z4;
						tmp2 += z2 + z3;
						tmp3 += z1 + z4;

					} else {
						/* d1 == 0, d3 != 0, d5 != 0, d7 == 0 */
						z2 = d5 + d3;

						z5 = MULTIPLY(z2, FIX_1_175875602);
						tmp1 = MULTIPLY(d5, FIX_1_662939225);
						z4 = MULTIPLY(-d5, FIX_0_390180644);
						z2 = MULTIPLY(-z2, FIX_1_387039845);
						tmp2 = MULTIPLY(d3, FIX_1_111140466);
						z3 = MULTIPLY(-d3, FIX_1_961570560);

						tmp0 = z3 + z5;
						tmp1 += z2;
						tmp2 += z2;
						tmp3 = z4 + z5;
					}

				} else {
					if(d1) {
						/* d1 != 0, d3 == 0, d5 != 0, d7 == 0 */
						z4 = d5 + d1;

						z5 = MULTIPLY(z4, FIX_1_175875602);
						z1 = MULTIPLY(-d1, FIX_0_899976223);
						tmp3 = MULTIPLY(d1, FIX_0_601344887);
						tmp1 = MULTIPLY(-d5, FIX_0_509795579);
						z2 = MULTIPLY(-d5, FIX_2_562915447);
						z4 = MULTIPLY(z4, FIX_0_785694958);

						tmp0 = z1 + z5;
						tmp1 += z4;
						tmp2 = z2 + z5;
						tmp3 += z4;

					} else {
						/* d1 == 0, d3 == 0, d5 != 0, d7 == 0 */
						tmp0 = MULTIPLY(d5, FIX_1_175875602);
						tmp1 = MULTIPLY(d5, FIX_0_275899380);
						tmp2 = MULTIPLY(-d5, FIX_1_387039845);
						tmp3 = MULTIPLY(d5, FIX_0_785694958);
					}
				}

			} else {
				if(d3) {
					if(d1) {
						/* d1 != 0, d3 != 0, d5 == 0, d7 == 0 */
						z5 = d1 + d3;
						tmp3 = MULTIPLY(d1, FIX_0_211164243);
						tmp2 = MULTIPLY(-d3, FIX_1_451774981);
						z1 = MULTIPLY(d1, FIX_1_061594337);
						z2 = MULTIPLY(-d3, FIX_2_172734803);
						z4 = MULTIPLY(z5, FIX_0_785694958);
						z5 = MULTIPLY(z5, FIX_1_175875602);

						tmp0 = z1 - z4;
						tmp1 = z2 + z4;
						tmp2 += z5;
						tmp3 += z5;

					} else {
						/* d1 == 0, d3 != 0, d5 == 0, d7 == 0 */
						tmp0 = MULTIPLY(-d3, FIX_0_785694958);
						tmp1 = MULTIPLY(-d3, FIX_1_387039845);
						tmp2 = MULTIPLY(-d3, FIX_0_275899380);
						tmp3 = MULTIPLY(d3, FIX_1_175875602);
					}

				} else {
					if(d1) {
						/* d1 != 0, d3 == 0, d5 == 0, d7 == 0 */
						tmp0 = MULTIPLY(d1, FIX_0_275899380);
						tmp1 = MULTIPLY(d1, FIX_0_785694958);
						tmp2 = MULTIPLY(d1, FIX_1_175875602);
						tmp3 = MULTIPLY(d1, FIX_1_387039845);

					} else {
						/* d1 == 0, d3 == 0, d5 == 0, d7 == 0 */
						tmp0 = tmp1 = tmp2 = tmp3 = 0;
					}
				}
			}
		}

		/* Final output stage: inputs are tmp10..tmp13, tmp0..tmp3 */

//        dataptr[JPEG_BLOCK_SIZE * 0] = (int16_t) DESCALE(tmp10 + tmp3,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 7] = (int16_t) DESCALE(tmp10 - tmp3,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 1] = (int16_t) DESCALE(tmp11 + tmp2,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 6] = (int16_t) DESCALE(tmp11 - tmp2,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 2] = (int16_t) DESCALE(tmp12 + tmp1,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 5] = (int16_t) DESCALE(tmp12 - tmp1,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 3] = (int16_t) DESCALE(tmp13 + tmp0,
//                                                         CONST_BITS + PASS1_BITS + 3);
//        dataptr[JPEG_BLOCK_SIZE * 4] = (int16_t) DESCALE(tmp13 - tmp0,
//                                                         CONST_BITS + PASS1_BITS + 3);
		/*
		        dataptr[0] = (int16_t) DESCALE(tmp10 + tmp3,
		                                       CONST_BITS + PASS1_BITS + 3);
		        dataptr[56] = (int16_t) DESCALE(tmp10 - tmp3,
		                                        CONST_BITS + PASS1_BITS + 3);
		        dataptr[8] = (int16_t) DESCALE(tmp11 + tmp2,
		                                       CONST_BITS + PASS1_BITS + 3);
		        dataptr[48] = (int16_t) DESCALE(tmp11 - tmp2,
		                                        CONST_BITS + PASS1_BITS + 3);
		        dataptr[16] = (int16_t) DESCALE(tmp12 + tmp1,
		                                        CONST_BITS + PASS1_BITS + 3);
		        dataptr[40] = (int16_t) DESCALE(tmp12 - tmp1,
		                                        CONST_BITS + PASS1_BITS + 3);
		        dataptr[24] = (int16_t) DESCALE(tmp13 + tmp0,
		                                        CONST_BITS + PASS1_BITS + 3);
		        dataptr[32] = (int16_t) DESCALE(tmp13 - tmp0,
		                                        CONST_BITS + PASS1_BITS + 3);
		*/
		//配合前面+128
		dataptr[0] = (int16_t) DESCALE(tmp10 + tmp3,
									   CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[56] = (int16_t) DESCALE(tmp10 - tmp3,
										CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[8] = (int16_t) DESCALE(tmp11 + tmp2,
									   CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[48] = (int16_t) DESCALE(tmp11 - tmp2,
										CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[16] = (int16_t) DESCALE(tmp12 + tmp1,
										CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[40] = (int16_t) DESCALE(tmp12 - tmp1,
										CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[24] = (int16_t) DESCALE(tmp13 + tmp0,
										CONST_BITS + PASS1_BITS + 3) + 128;
		dataptr[32] = (int16_t) DESCALE(tmp13 - tmp0,
										CONST_BITS + PASS1_BITS + 3) + 128;

		dataptr++;                  /* advance pointer to next column */
	}
}


