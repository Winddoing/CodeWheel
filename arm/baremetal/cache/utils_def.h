#ifndef __UTILS_DEF_H__
#define __UTILS_DEF_H__

/*
 * For those constants to be shared between C and other sources, apply a 'U',
 * 'UL', 'ULL', 'L' or 'LL' suffix to the argument only in C, to avoid
 * undefined or unintended behaviour.
 *
 * The GNU assembler and linker do not support these suffixes (it causes the
 * build process to fail) therefore the suffix is omitted when used in linker
 * scripts and assembler files.
*/

#if defined(__ASSEMBLER__)
# define   U(_x)	(_x)
# define  UL(_x)	(_x)
# define ULL(_x)	(_x)
# define   L(_x)	(_x)
# define  LL(_x)	(_x)
#else
# define  U_(_x)	(_x##U)
# define   U(_x)	U_(_x)
# define  UL(_x)	(_x##UL)
# define ULL(_x)	(_x##ULL)
# define   L(_x)	(_x##L)
# define  LL(_x)	(_x##LL)
#endif

/*
 * Import an assembly or linker symbol as a C expression with the specified
 * type
 */
#define IMPORT_SYM(type, sym, name) \
	extern char sym[];\
	static const __attribute__((unused)) type name = (type) sym;

/* Compute the number of elements in the given array */
#define ARRAY_SIZE(a)				\
	(sizeof(a) / sizeof((a)[0]))

/*
 * The round_up() macro rounds up a value to the given boundary in a
 * type-agnostic yet type-safe manner. The boundary must be a power of two.
 * In other words, it computes the smallest multiple of boundary which is
 * greater than or equal to value.
 *
 * round_down() is similar but rounds the value down instead.
 */
#define round_boundary(value, boundary)		\
	((__typeof__(value))((boundary) - 1))

#define round_up(value, boundary)		\
	((((value) - 1) | round_boundary(value, boundary)) + 1)

#define round_down(value, boundary)		\
	((value) & ~round_boundary(value, boundary))

#define BIT(nr)			(ULL(1) << (nr))


#define MIN(x, y) __extension__ ({	\
	__typeof__(x) _x = (x);		\
	__typeof__(y) _y = (y);		\
	(void)(&_x == &_y);		\
	_x < _y ? _x : _y;		\
})

#define MAX(x, y) __extension__ ({	\
	__typeof__(x) _x = (x);		\
	__typeof__(y) _y = (y);		\
	(void)(&_x == &_y);		\
	_x > _y ? _x : _y;		\
})

#endif