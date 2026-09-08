/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : doprntx.c
 *  Author       : wqshao
 *  Created Time : 2026-03-30 13:55:22
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>

/* Control characters */
#define CTRL_A          ('A'-0x40)
#define CTRL_B          ('B'-0x40)
#define CTRL_C          ('C'-0x40)
#define CTRL_D          ('D'-0x40)
#define CTRL_E          ('E'-0x40)
#define CTRL_F          ('F'-0x40)
#define CTRL_H          ('H'-0x40)
#define CTRL_K          ('K'-0x40)
#define CTRL_N          ('N'-0x40)
#define CTRL_P          ('P'-0x40)
#define CTRL_U          ('U'-0x40)
#define BACKSPACE       0x08
#define DEL             0x7F
#define TAB             0x09
#define CR              0x0D /* Enter Key */
#define LF              0x0A
#define ESC             0x1B
#define SP              0x20
#define CSI             0x9B


//#define U64 unsigned long long
#define U32 unsigned int
#define U16 unsigned short
#define U8  unsigned char
//#define S64 signed long long
//#define S32 int
#define S16 short int
//#define S8  signed char
//#define bool U8


/* for _doprintx */
#define	BUF	256

#define	ARG()								\
            _ulong =							\
	        (flags & SHORTINT) ?					\
	            (U32)((S16)((U16)va_arg(argp, U32))) :	\
		    (U32)va_arg(argp, U32);

#define	todigit(c)	((c) - '0')
#define	tochar(n)	((n) + '0')

/* have to deal with the negative buffer count kludge */
#define	NEGATIVE_COUNT_KLUDGE

#define	LONGINT		0x01		/* long integer 		*/
#define	LONGDBL		0x02		/* long double; unimplemented 	*/
#define	SHORTINT	0x04		/* short integer 		*/
#define	ALT		0x08		/* alternate form 		*/
#define	LADJUST		0x10		/* left adjustment 		*/
#define	ZEROPAD		0x20		/* zero (as opposed to blank) pad */
#define	HEXPREFIX	0x40		/* add 0x or 0X prefix 		*/

static void *memchr(const void *s, int c, size_t n)
{
        const unsigned char *p = s;
        while (n-- != 0) {
                if ((unsigned char)c == *p++) {
                        return (void *)(p-1);
                }
        }
        return NULL;
}

static size_t strlen(const char * s)
{
        const char *sc;

        for (sc = s; *sc != '\0'; ++sc)
                /* nothing */;
        return sc - s;
}

static int _doprntx(
	char 		*fmt0,
	va_list 	argp,
	void		(*putc)(),
	char		**pca)
{
	U8 	*fmt;		/* format string 			*/
	int 	ch;		/* character from fmt 			*/
	int 	cnt;		/* return value accumulator 		*/
	int 	n;		/* random handy integer 		*/
	char 	*t;		/* buffer pointer */
	U32 	_ulong;		/* integer arguments %[diouxX] 		*/
	int 	base;		/* base for [diouxX] conversion 	*/
	int 	dprec;		/* decimal precision in [diouxX] 	*/
	int 	fieldsz;	/* field size expanded by sign, etc 	*/
	int 	flags;		/* flags as above 			*/
	int 	fpprec;		/* `extra' floating precision in [eEfgG]*/
	int 	prec;		/* precision from format (%.3d), or -1 	*/
	int 	realsz;		/* field size expanded by decimal precision */
	int 	size;		/* size of converted field or string 	*/
	int 	width;		/* width from format (%8d), or 0 	*/
	char	sign;		/* sign prefix (' ', '+', '-', or \0) 	*/
	char	*digs;		/* digits for [diouxX] conversion 	*/
	char 	buf[BUF];	/* space for %c, %[diouxX], %[eEfgG] 	*/

	fmt = (U8*)fmt0;
	digs = "0123456789abcdef";

	for (cnt = 0;; ++fmt) {
		for (; (ch = *fmt) && ch != '%';
		     ++cnt, ++fmt)
		    if(ch!='\n')
		      putc(ch, pca);
		    else
		    { putc(CR, pca); putc(LF, pca); ++cnt; };
		if (!ch)
			return (cnt);

		flags = 0; dprec = 0; fpprec = 0; width = 0;
		prec = -1;
		sign = '\0';

rflag:		switch (*++fmt) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a  positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			if ((width = va_arg(argp, int)) >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if (*++fmt == '*')
				n = va_arg(argp, int);
			else {
				n = 0;
				while (isascii(*fmt) && isdigit(*fmt))
					n = 10 * n + todigit(*fmt++);
				--fmt;
			}
			prec = n < 0 ? -1 : n;
			goto rflag;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *	-- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + todigit(*fmt);
			} while (isascii(*++fmt) && isdigit(*fmt));
			width = n;
			--fmt;
			goto rflag;
		case 'L':
			flags |= LONGDBL;
			goto rflag;
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			flags |= LONGINT;
			goto rflag;
		case 'c':
			*(t = buf) = va_arg(argp, int);
			size = 1;
			sign = '\0';
			goto pforw;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			ARG();
			if ((long)_ulong < 0) {
				_ulong = -_ulong;
				sign = '-';
			}
			base = 10;
			goto number;
		case 'n':
			if (flags & LONGINT)
				*va_arg(argp, long *) = cnt;
			else if (flags & SHORTINT)
				*va_arg(argp, short *) = cnt;
			else
				*va_arg(argp, int *) = cnt;
			break;
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			ARG();
			base = 8;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *	-- ANSI X3J11
			 */
			/* NOSTRICT */
			_ulong = (U32)va_arg(argp, void *);
			base = 16;
			goto nosign;
		case 's':
			if (!(t = va_arg(argp, char *)))
				t = "(null)";
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p;

				if ((p = (void *)memchr(t, 0, prec))) {
					size = p - t;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(t);
			sign = '\0';
			goto pforw;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			ARG();
			base = 10;
			goto nosign;
		case 'X':
			digs = "0123456789ABCDEF";
			/* FALLTHROUGH */
		case 'x':
			ARG();
			base = 16;
			/* leading 0x/X only if non-zero */
			if (flags & ALT && _ulong != 0)
				flags |= HEXPREFIX;

			/* unsigned conversions */
nosign:			sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *	-- ANSI X3J11
			 */
number:			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *	-- ANSI X3J11
			 */
			t = buf + BUF;
			if (_ulong != 0 || prec != 0) {
				do {
					*--t = digs[_ulong % base];
					_ulong /= base;
				} while (_ulong);
				digs = "0123456789abcdef";
				if (flags & ALT && base == 8 && *t != '0')
					*--t = '0'; /* octal leading 0 */
			}
			size = buf + BUF - t;

pforw:
			/*
			 * All reasonable formats wind up here.  At this point,
			 * `t' points to a string which (if not flags&LADJUST)
			 * should be padded out to `width' places.  If
			 * flags&ZEROPAD, it should first be prefixed by any
			 * sign or other prefix; otherwise, it should be blank
			 * padded before the prefix is emitted.  After any
			 * left-hand padding and prefixing, emit zeroes
			 * required by a decimal [diouxX] precision, then print
			 * the string proper, then emit zeroes required by any
			 * leftover floating precision; finally, if LADJUST,
			 * pad with blanks.
			 */

			/*
			 * compute actual size, so we know how much to pad
			 * fieldsz excludes decimal prec; realsz includes it
			 */
			fieldsz = size + fpprec;
			if (sign)
				fieldsz++;
			if (flags & HEXPREFIX)
				fieldsz += 2;
			realsz = dprec > fieldsz ? dprec : fieldsz;

			/* right-adjusting blank padding */
			if ((flags & (LADJUST|ZEROPAD)) == 0 && width)
				for (n = realsz; n < width; n++)
					putc(' ', pca);
			/* prefix */
			if (sign)
				putc(sign, pca);
			if (flags & HEXPREFIX) {
				putc('0', pca);
				putc((char)*fmt, pca);
			}
			/* right-adjusting zero padding */
			if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
				for (n = realsz; n < width; n++)
					putc('0', pca);
			/* leading zeroes from decimal precision */
			for (n = fieldsz; n < dprec; n++)
				putc('0', pca);

			/* the string or number proper */
			n=size;
			while (--n >= 0)
			  putc(*t++, pca);
			/* trailing f.p. zeroes */
			while (--fpprec >= 0)
				putc('0', pca);
			/* left-adjusting padding (always blank) */
			if (flags & LADJUST)
				for (n = realsz; n < width; n++)
					putc(' ', pca);
			/* finally, adjust cnt */
			cnt += width > realsz ? width : realsz;
			break;
		case '\0':	/* "%?" prints ?, unless ? is NULL */
			return (cnt);
		default:
			putc((char)*fmt, pca);
			cnt++;
		}
	}
	/* NOTREACHED */
}

static void _putc(char ch, char **pca)
{
	putchar(ch);
}

static int my_printf(const char *fmt, ...)
{
	int ret;
	va_list args;
	va_start(args,fmt);
	ret = _doprntx((char *)fmt, args, (void (*)())_putc, 0);
	va_end(args);
	return ret;
}

int main(int argc, const char *argv[])
{
	
	my_printf("abcdef\n");
	return 0;
}
