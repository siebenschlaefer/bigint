#include "bigint.h"

#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <stdint.h>

#define STRINGIFYX(x) #x
#define STRINGIFY(x) STRINGIFYX(x)
#define HERE __FILE__ ":" STRINGIFY(__LINE__)
#define DEBUG std::cerr << HERE << " "
#define PING DEBUG << std::endl;
#define PRINT(x) DEBUG << (#x) << "=" << (x) << std::endl

struct not_implemented
	: public std::logic_error
{
	not_implemented(const std::string& what="not implemented")
		: std::logic_error(what)
	{ }
};

namespace // anonymous
{

template < typename Ty >
std::string to_string(const Ty& value)
{
	std::ostringstream out;
	out << value;
	return out.str();
}

template<typename T>
inline
bool isinf(const T& value)
{
	return std::numeric_limits<T>::has_infinity &&
		value == std::numeric_limits<T>::infinity();
}

char char2digit(char c)
{
	switch (c)
	{
	case '0': return  0;
	case '1': return  1;
	case '2': return  2;
	case '3': return  3;
	case '4': return  4;
	case '5': return  5;
	case '6': return  6;
	case '7': return  7;
	case '8': return  8;
	case '9': return  9;
	case 'a': return 10;
	case 'b': return 11;
	case 'c': return 12;
	case 'd': return 13;
	case 'e': return 14;
	case 'f': return 15;
	case 'g': return 16;
	case 'h': return 17;
	case 'i': return 18;
	case 'j': return 19;
	case 'k': return 20;
	case 'l': return 21;
	case 'm': return 22;
	case 'n': return 23;
	case 'o': return 24;
	case 'p': return 25;
	case 'q': return 26;
	case 'r': return 27;
	case 's': return 28;
	case 't': return 29;
	case 'u': return 30;
	case 'v': return 31;
	case 'w': return 32;
	case 'x': return 33;
	case 'y': return 34;
	case 'z': return 35;
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	case 'G': return 16;
	case 'H': return 17;
	case 'I': return 18;
	case 'J': return 19;
	case 'K': return 20;
	case 'L': return 21;
	case 'M': return 22;
	case 'N': return 23;
	case 'O': return 24;
	case 'P': return 25;
	case 'Q': return 26;
	case 'R': return 27;
	case 'S': return 28;
	case 'T': return 29;
	case 'U': return 30;
	case 'V': return 31;
	case 'W': return 32;
	case 'X': return 33;
	case 'Y': return 34;
	case 'Z': return 35;
	default: return std::numeric_limits<char>::max();
	}
	throw std::logic_error("invalid char");
}

char digit2char(char digit)
{
	switch (digit)
	{
	case  0: return '0';
	case  1: return '1';
	case  2: return '2';
	case  3: return '3';
	case  4: return '4';
	case  5: return '5';
	case  6: return '6';
	case  7: return '7';
	case  8: return '8';
	case  9: return '9';
	case 10: return 'a';
	case 11: return 'b';
	case 12: return 'c';
	case 13: return 'd';
	case 14: return 'e';
	case 15: return 'f';
	case 16: return 'g';
	case 17: return 'h';
	case 18: return 'i';
	case 19: return 'j';
	case 20: return 'k';
	case 21: return 'l';
	case 22: return 'm';
	case 23: return 'n';
	case 24: return 'o';
	case 25: return 'p';
	case 26: return 'q';
	case 27: return 'r';
	case 28: return 's';
	case 29: return 't';
	case 30: return 'u';
	case 31: return 'v';
	case 32: return 'w';
	case 33: return 'x';
	case 34: return 'y';
	case 35: return 'z';
	}
	throw std::invalid_argument("invalid digit");
}

void add_digit(bigint& result, int base, char c)
{
	char digit = char2digit(c);
	if (digit >= base)
		throw std::invalid_argument("invalid digit");
	result = result * base + digit;
}

void set_base(int& base, int fixed_base, int value)
{
	if (fixed_base == 0)
		base = value;
	else if (fixed_base != base)
		throw std::invalid_argument("invalid base");
}

bigint parse_bigint(const std::string& text, int fixed_base=0)
{
	int base = (fixed_base == 0)? 10 : fixed_base;
	bool is_positive = true;
	bigint result;
	enum { START, SIGN,
		PREFIX_0, PREFIX_0_X,
		PREFIX_12, PREFIX_12_X,
		PREFIX_3, PREFIX_3_X,
		PREFIX_49,
		BASE_END, DIGITS } state = START;
	for (std::string::size_type i = 0; i < text.size(); ++i)
	{
		char c = text[i];
		switch (state)
		{
		case START:
			if (std::isspace(c))
				;
			else if (c == '+')
				state = SIGN;
			else if (c == '-')
			{
				is_positive = false;
				state = SIGN;
			}
			else if (c == '0')
				state = PREFIX_0;
			else if (c == '1' || c == '2')
				state = PREFIX_12;
			else if (c == '3')
				state = PREFIX_3;
			else if ('4' <= c && c <= '9')
				state = PREFIX_49;
			else if (('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case SIGN:
			if (std::isspace(c))
				;
			else if (c == '0')
				state = PREFIX_0;
			else if (c == '1' || c == '2')
				state = PREFIX_12;
			else if (c == '3')
				state = PREFIX_3;
			else if ('4' <= c && c <= '9')
				state = PREFIX_49;
			else if (('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_0:
			if (c == '0')
				state = DIGITS;
			else if ('1' <= c && c <= '9')
				state = PREFIX_0_X;
			else if (c == 'b')
			{
				if (fixed_base == 0 || fixed_base == 2)
				{
					set_base(base, fixed_base, 2);
					state = BASE_END;
				}
				else
				{
					add_digit(result, base, c);
					state = DIGITS;
				}
			}
			else if (c == 'o')
			{
				if (fixed_base == 0 || fixed_base == 8)
				{
					set_base(base, fixed_base, 8);
					state = BASE_END;
				}
				else
				{
					add_digit(result, base, c);
					state = DIGITS;
				}
			}
			else if (c == 'd')
			{
				if (fixed_base == 0 || fixed_base == 10)
				{
					set_base(base, fixed_base, 10);
					state = BASE_END;
				}
				else
				{
					add_digit(result, base, c);
					state = DIGITS;
				}
			}
			else if (c == 'x')
			{
				if (fixed_base == 0 || fixed_base == 16)
				{
					set_base(base, fixed_base, 16);
					state = BASE_END;
				}
				else
				{
					add_digit(result, base, c);
					state = DIGITS;
				}
			}
			else if (('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_12:
			if (c == '#')
			{
				set_base(base, fixed_base, text[i - 1] - '0');
				state = BASE_END;
			}
			else if ('0' <= c && c <= '9')
				state = PREFIX_12_X;
			else if (('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, text[i - 1]);
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_3:
			if (c == '#')
			{
				set_base(base, fixed_base, text[i - 1] - '0');
				state = BASE_END;
			}
			else if ('0' <= c && c <= '6')
				state = PREFIX_3_X;
			else if (('7' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, '3');
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_49:
			if (c == '#')
			{
				set_base(base, fixed_base, text[i - 1] - '0');
				state = BASE_END;
			}
			else if (('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, text[i - 1]);
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_0_X:
			if (c == '#')
			{
				set_base(base, fixed_base, text[i - 1] - '0');
				state = BASE_END;
			}
			else if (('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, text[i - 1]);
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_12_X:
			if (c == '#')
			{
				set_base(base, fixed_base,
					(text[i - 2] - '0') * 10 +
					(text[i - 1] - '0'));
				state = BASE_END;
			}
			else if (('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, text[i - 2]);
				add_digit(result, base, text[i - 1]);
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case PREFIX_3_X:
			if (c == '#')
			{
				set_base(base, fixed_base,
					30 + text[i - 1] - '0');
				state = BASE_END;
			}
			else if (('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, '3');
				add_digit(result, base, text[i - 1]);
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case BASE_END:
			if (('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		case DIGITS:
			if (('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z'))
			{
				add_digit(result, base, c);
				state = DIGITS;
			}
			else
				throw std::invalid_argument("invalid char");
			break;
		}
	}
	switch (state)
	{
	case START:
	case SIGN:
		throw std::invalid_argument("not a number");
		break;
	case PREFIX_0:
		break;
	case PREFIX_12:
	case PREFIX_3:
	case PREFIX_49:
	case PREFIX_0_X:
		add_digit(result, base, text[text.size() - 1]);
		break;
	case PREFIX_12_X:
	case PREFIX_3_X:
		add_digit(result, base, text[text.size() - 2]);
		add_digit(result, base, text[text.size() - 1]);
		break;
	case BASE_END:
		throw std::invalid_argument("not a number");
		break;
	case DIGITS:
		break;
	}
	if (! is_positive)
		result = -result;
	return result;
}

// 23
/* convert a PyLong of size 1, 0 or -1 to an sdigit */
#define MEDIUM_VALUE(x) \
	(x->ob_size < 0 ? -(sdigit)(x)->ob_digit[0] : \
	(x->ob_size == 0 ? (sdigit)0 :                \
	(sdigit)(x)->ob_digit[0]))

// 27
template < typename Ty >
inline
Ty abs(const Ty& value)
{
	return ((value < 0)? -value : value);
}

// 75
/* If a freshly-allocated long is already shared, it must
   be a small integer, so negating it must go to PyLong_FromLong */
#define NEGATE(x) \
	do if (Py_REFCNT(x) == 1) \
		x->ob_size = -x->ob_size;  \
	else { \
		assert(false); \
		PyObject* tmp = PyLong_FromLong(-MEDIUM_VALUE(x));  \
		Py_DECREF(x); \
		(x) = tmp; \
	} while(0)

// 82
/* For long multiplication, use the O(N**2) school algorithm unless
 * both operands contain more than KARATSUBA_CUTOFF digits (this
 * being an internal Python long digit, in base BASE).
 */
#define KARATSUBA_CUTOFF 70
#define KARATSUBA_SQUARE_CUTOFF (2 * KARATSUBA_CUTOFF)

// pyport.h 408
/* Py_ARITHMETIC_RIGHT_SHIFT
 * C doesn't define whether a right-shift of a signed integer sign-extends
 * or zero-fills.  Here a macro to force sign extension:
 * Py_ARITHMETIC_RIGHT_SHIFT(TYPE, I, J)
 *    Return I >> J, forcing sign extension.  Arithmetically, return the
 *    floor of I/2**J.
 * Requirements:
 *    I should have signed integer type.  In the terminology of C99, this can
 *    be either one of the five standard signed integer types (signed char,
 *    short, int, long, long long) or an extended signed integer type.
 *    J is an integer >= 0 and strictly less than the number of bits in the
 *    type of I (because C doesn't define what happens for J outside that
 *    range either).
 *    TYPE used to specify the type of I, but is now ignored.  It's been left
 *    in for backwards compatibility with versions <= 2.6 or 3.0.
 * Caution:
 *    I may be evaluated more than once.
 */
#ifdef SIGNED_RIGHT_SHIFT_ZERO_FILLS
#define Py_ARITHMETIC_RIGHT_SHIFT(TYPE, I, J) \
    ((I) < 0 ? -1-((-1-(I)) >> (J)) : (I) >> (J))
#else
#define Py_ARITHMETIC_RIGHT_SHIFT(TYPE, I, J) ((I) >> (J))
#endif

typedef ssize_t         Py_ssize_t;
#define PY_SSIZE_T_MAX (std::numeric_limits<Py_ssize_t>::max())
#define PY_SSIZE_T_MIN (std::numeric_limits<Py_ssize_t>::min())

typedef uint32_t digit;
typedef int32_t sdigit; /* signed variant of digit */
typedef uint64_t twodigits;
typedef int64_t stwodigits; /* signed variant of twodigits */
#define PyLong_SHIFT	30
#define _PyLong_DECIMAL_SHIFT	9 /* max(e) such that 10**e fits in a digit) */
#define _PyLong_DECIMAL_BASE	((digit)1000000000) /* 10 ** DECIMAL_SHIFT */
#define PyLong_BASE	((digit)1 << PyLong_SHIFT)
#define PyLong_MASK	((digit)(PyLong_BASE - 1))

} // anonymous namespace

struct bigint::Impl
{
	Py_ssize_t ob_size;
	digit ob_digit[1];
};

namespace // anonymous
{

// 106
/* Normalize (remove leading zeros from) a long int object.
   Doesn't attempt to free the storage--in most cases, due to the nature
   of the algorithms used, this could save at most be one word anyway. */
bigint::Impl* long_normalize(register bigint::Impl *v)
{
	Py_ssize_t j = abs(v->ob_size);
	Py_ssize_t i = j;

	while (i > 0 && v->ob_digit[i-1] == 0)
		--i;
	if (i != j)
		v->ob_size = (v->ob_size < 0) ? -(i) : i;
	return v;
}

void long_negate1(bigint::Impl *x)
{
	assert(x);
	x->ob_size = - x->ob_size;
}

// 126
/* Allocate a new long int object with size digits.
   Return NULL and set exception if we run out of memory. */
#define MAX_LONG_DIGITS \
	((PY_SSIZE_T_MAX - offsetof(bigint::Impl, ob_digit)) / sizeof(digit))
bigint::Impl* _PyLong_New(Py_ssize_t size)
{
	/* Number of bytes needed is: offsetof(bigint::Impl, ob_digit) +
	   sizeof(digit)*size.  Previous incarnations of this code used
	   sizeof(PyVarObject) instead of the offsetof, but this risks being
	   incorrect in the presence of padding between the PyVarObject header
	   and the digits. */
	if (size > (Py_ssize_t) MAX_LONG_DIGITS)
		throw std::overflow_error("too many digits in integer");
	bigint::Impl* result = (bigint::Impl *) operator new(
		offsetof(bigint::Impl, ob_digit) + size * sizeof(digit));
	result->ob_size = size;
	return result;
}

// 152
bigint::Impl* _PyLong_Copy(const bigint::Impl *src)
{
	assert(src);
	Py_ssize_t i = src->ob_size;
	if (i < 0)
		i = -(i);
	if (i < 2)
	{
		sdigit ival = src->ob_digit[0];
		if (src->ob_size < 0)
			ival = -ival;
	}
	bigint::Impl* result = _PyLong_New(i);
	result->ob_size = src->ob_size;
	while (--i >= 0)
		result->ob_digit[i] = src->ob_digit[i];
	return result;
}

void _PyLong_Delete(bigint::Impl* impl)
{
	assert(impl);
	free(impl);
}

/* Create a new long int object from a C long int */
bigint::Impl* PyLong_FromLong(signed long ival)
{
	int sign = (ival < 0)? -1 : ((ival == 0)? 0 : 1);
	unsigned long abs_ival = (ival < 0)?
		0U - static_cast<unsigned long>(ival) :
		static_cast<unsigned long>(ival);

	/* Fast path for single-digit ints */
	if (!(abs_ival >> PyLong_SHIFT))
	{
		bigint::Impl* v = _PyLong_New(1);
		v->ob_size = sign;
		v->ob_digit[0] = static_cast<digit>(abs_ival);
		return v;
	}

#if PyLong_SHIFT==15
	/* 2 digits */
	if (!(abs_ival >> 2 * PyLong_SHIFT))
	{
		bigint::Impl* v = _PyLong_New(2);
		v->ob_size = 2 * sign;
		v->ob_digit[0] = static_cast<digit>(abs_ival & PyLong_MASK);
		v->ob_digit[1] = static_cast<digit>(abs_ival >> PyLong_SHIFT);
		return v;
	}
#endif

	/* Larger numbers: loop to determine number of digits */
	int ndigits = 0;
	unsigned long t = abs_ival;  // unsigned so >> doesn't propagate sign bit
	while (t)
	{
		++ndigits;
		t >>= PyLong_SHIFT;
	}
	bigint::Impl* v = _PyLong_New(ndigits);
	digit *p = v->ob_digit;
	v->ob_size = ndigits * sign;
	t = abs_ival;
	while (t)
	{
		*p++ = static_cast<digit>(t & PyLong_MASK);
		t >>= PyLong_SHIFT;
	}
	return v;
}

// 246
/* Create a new long int object from a C unsigned long int */
bigint::Impl* PyLong_FromUnsignedLong(unsigned long ival)
{
	if (ival < PyLong_BASE)
		return PyLong_FromLong(ival);
	/* Count the number of Python digits. */
	int ndigits = 0;
	unsigned long t = (unsigned long)ival;
	while (t)
	{
		++ndigits;
		t >>= PyLong_SHIFT;
	}
	bigint::Impl* v = _PyLong_New(ndigits);
	digit *p = v->ob_digit;
	v->ob_size = ndigits;
	while (ival)
	{
		*p++ = (digit)(ival & PyLong_MASK);
		ival >>= PyLong_SHIFT;
	}
	return v;
}

// 275
/* Create a new long int object from a C double */
bigint::Impl* PyLong_FromDouble(double dval)
{
	if (isinf(dval))
		throw std::invalid_argument(
			"cannot convert float infinity to integer");
	if (dval != dval) // isnan(dval)
		throw std::invalid_argument(
			"cannot convert float NaN to integer");
	int neg = 0;
	if (dval < 0.0)
	{
		neg = 1;
		dval = -dval;
	}
	int expo;
	/* dval = frac*2**expo; 0.0 <= frac < 1.0 */
	double frac = frexp(dval, &expo);
	if (expo <= 0)
		return PyLong_FromLong(0L);
	/* Number of 'digits' in result */
	int ndig = (expo - 1) / PyLong_SHIFT + 1;

	bigint::Impl* v = _PyLong_New(ndig);
	frac = ldexp(frac, (expo - 1) % PyLong_SHIFT + 1);
	for (int i = ndig; --i >= 0; )
	{
		digit bits = (digit)frac;
		v->ob_digit[i] = bits;
		frac = frac - (double)bits;
		frac = ldexp(frac, PyLong_SHIFT);
	}
	if (neg)
		long_negate1(v);
	return v;
}

// 317
/* Checking for overflow in PyLong_AsLong is a PITA since C doesn't define
 * anything about what happens when a signed integer operation overflows,
 * and some compilers think they're doing you a favor by being "clever"
 * then.  The bit pattern for the largest postive signed long is
 * (unsigned long)LONG_MAX, and for the smallest negative signed long
 * it is abs(LONG_MIN), which we could write -(unsigned long)LONG_MIN.
 * However, some other compilers warn about applying unary minus to an
 * unsigned operand.  Hence the weird "0-".
 */
#define PY_ABS_LONG_MIN         (0-(unsigned long)LONG_MIN)
#define PY_ABS_SSIZE_T_MIN      (0-(size_t)PY_SSIZE_T_MIN)

// 433
/* Get a Py_ssize_t from a long int object.
   Returns -1 and sets an error condition if overflow occurs. */
Py_ssize_t PyLong_AsSsize_t(const bigint::Impl *v)
{
	assert(v);

	Py_ssize_t i = v->ob_size;
	switch (i)
	{
	case -1: return -(sdigit)v->ob_digit[0];
	case 0: return 0;
	case 1: return v->ob_digit[0];
	}
	int sign = 1;
	size_t x = 0;
	if (i < 0)
	{
		sign = -1;
		i = -(i);
	}
	while (--i >= 0)
	{
		size_t prev = x;
		x = (x << PyLong_SHIFT) | v->ob_digit[i];
		if ((x >> PyLong_SHIFT) != prev)
			throw std::overflow_error("Python int too large "
				"to convert to C ssize_t");
	}
	/* Haven't lost any bits, but casting to a signed type requires
	 * extra care (see comment above).
	 */
	if (x <= (size_t)PY_SSIZE_T_MAX)
		return (Py_ssize_t)x * sign;
	else if (sign < 0 && x == PY_ABS_SSIZE_T_MIN)
		return PY_SSIZE_T_MIN;
	/* else overflow */
	throw std::overflow_error("Python int too large "
		"to convert to C ssize_t");
}

// 1395
/* bits_in_digit(d) returns the unique integer k such that 2**(k-1) <= d <
   2**k if d is nonzero, else 0. */
static const unsigned char BitLengthTable[32] =
{
	0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};
int bits_in_digit(digit d)
{
	int d_bits = 0;
	while (d >= 32)
	{
		d_bits += 6;
		d >>= 6;
	}
	d_bits += (int) BitLengthTable[d];
	return d_bits;
}

// 1415
/* x[0:m] and y[0:n] are digit vectors, LSD first, m >= n required.  x[0:n]
 * is modified in place, by adding y to it.  Carries are propagated as far as
 * x[m-1], and the remaining carry (0 or 1) is returned.
 */
digit v_iadd(digit *x, Py_ssize_t m, const digit *y, Py_ssize_t n)
{
	assert(m >= n);

	Py_ssize_t i;
	digit carry = 0;
	for (i = 0; i < n; ++i)
	{
		carry += x[i] + y[i];
		x[i] = carry & PyLong_MASK;
		carry >>= PyLong_SHIFT;
		assert((carry & 1) == carry);
	}
	for (; carry && i < m; ++i)
	{
		carry += x[i];
		x[i] = carry & PyLong_MASK;
		carry >>= PyLong_SHIFT;
		assert((carry & 1) == carry);
	}
	return carry;
}

// 1441
/* x[0:m] and y[0:n] are digit vectors, LSD first, m >= n required.  x[0:n]
 * is modified in place, by subtracting y from it.  Borrows are propagated as
 * far as x[m-1], and the remaining borrow (0 or 1) is returned.
 */
digit v_isub(digit *x, Py_ssize_t m, const digit *y, Py_ssize_t n)
{
	assert(m >= n);

	Py_ssize_t i;
	digit borrow = 0;
	for (i = 0; i < n; ++i)
	{
		borrow = x[i] - y[i] - borrow;
		x[i] = borrow & PyLong_MASK;
		borrow >>= PyLong_SHIFT;
		borrow &= 1;            /* keep only 1 sign bit */
	}
	for (; borrow && i < m; ++i)
	{
		borrow = x[i] - borrow;
		x[i] = borrow & PyLong_MASK;
		borrow >>= PyLong_SHIFT;
		borrow &= 1;
	}
	return borrow;
}

// 1467
/* Shift digit vector a[0:m] d bits left, with 0 <= d < PyLong_SHIFT.  Put
 * result in z[0:m], and return the d bits shifted out of the top.
 */
digit v_lshift(digit *z, const digit *a, Py_ssize_t m, int d)
{
	assert(0 <= d && d < PyLong_SHIFT);

	digit carry = 0;
	for (Py_ssize_t i=0; i < m; i++)
	{
		twodigits acc = (twodigits)a[i] << d | carry;
		z[i] = (digit)acc & PyLong_MASK;
		carry = (digit)(acc >> PyLong_SHIFT);
	}
	return carry;
}

// 1485
/* Shift digit vector a[0:m] d bits right, with 0 <= d < PyLong_SHIFT.  Put
 * result in z[0:m], and return the d bits shifted out of the bottom.
 */
digit v_rshift(digit *z, const digit *a, Py_ssize_t m, int d)
{
	assert(0 <= d && d < PyLong_SHIFT);

	digit carry = 0;
	digit mask = ((digit)1 << d) - 1U;
	for (Py_ssize_t i = m; i-- > 0; )
	{
		twodigits acc = (twodigits)carry << PyLong_SHIFT | a[i];
		carry = (digit)acc & mask;
		z[i] = (digit)(acc >> d);
	}
	return carry;
}

// 1504
/* Divide long pin, w/ size digits, by non-zero digit n, storing quotient
   in pout, and returning the remainder.  pin and pout point at the LSD.
   It's OK for pin == pout on entry, which saves oodles of mallocs/frees in
   _PyLong_Format, but that should be done with great care since longs are
   immutable. */
digit inplace_divrem1(digit *pout, const digit *pin, Py_ssize_t size, digit n)
{
	assert(n > 0 && n <= PyLong_MASK);

	twodigits rem = 0;
	pin += size;
	pout += size;
	while (--size >= 0)
	{
		rem = (rem << PyLong_SHIFT) | *--pin;
		digit hi;
		*--pout = hi = (digit)(rem / n);
		rem -= (twodigits)hi * n;
	}
	return (digit)rem;
}

// 1527
/* Divide a long integer by a digit, returning both the quotient
   (as function result) and the remainder (through *prem).
   The sign of a is ignored; n should not be zero. */
bigint::Impl* divrem1(const bigint::Impl *a, digit n, digit *prem)
{
	assert(n > 0 && n <= PyLong_MASK);
	const Py_ssize_t size = abs(a->ob_size);
	bigint::Impl* z = _PyLong_New(size);
	*prem = inplace_divrem1(z->ob_digit, a->ob_digit, size, n);
	return long_normalize(z);
}

// 1545
/* Convert a long integer to a base 10 string.  Returns a new non-shared
   string.  (Return value is non-shared so that callers can modify the
   returned value if necessary.) */
std::string long_to_decimal_string(const bigint::Impl *a)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(a);

	Py_ssize_t size_a = abs(a->ob_size);
	int negative = a->ob_size < 0;

	/* quick and dirty upper bound for the number of digits
	   required to express a in base _PyLong_DECIMAL_BASE:

	     #digits = 1 + floor(log2(a) / log2(_PyLong_DECIMAL_BASE))

	   But log2(a) < size_a * PyLong_SHIFT, and
	   log2(_PyLong_DECIMAL_BASE) = log2(10) * _PyLong_DECIMAL_SHIFT
	                              > 3 * _PyLong_DECIMAL_SHIFT
	*/
	if (size_a > PY_SSIZE_T_MAX / PyLong_SHIFT)
		throw std::overflow_error("long is too large to format");
	/* the expression size_a * PyLong_SHIFT is now safe from overflow */
	Py_ssize_t size = 1 + size_a * PyLong_SHIFT / (3 * _PyLong_DECIMAL_SHIFT);
	ImplPtr scratch(_PyLong_New(size));

	/* convert array of base _PyLong_BASE digits in pin to an array of
	   base _PyLong_DECIMAL_BASE digits in pout, following Knuth (TAOCP,
	   Volume 2 (3rd edn), section 4.4, Method 1b). */
	const digit *pin = a->ob_digit;
	digit *pout = scratch->ob_digit;
	size = 0;
	for (Py_ssize_t i = size_a; --i >= 0; )
	{
		digit hi = pin[i];
		for (Py_ssize_t j = 0; j < size; j++)
		{
			twodigits z = (twodigits)pout[j] << PyLong_SHIFT | hi;
			hi = (digit)(z / _PyLong_DECIMAL_BASE);
			pout[j] = (digit)(z - (twodigits)hi *
							  _PyLong_DECIMAL_BASE);
		}
		while (hi)
		{
			pout[size++] = hi % _PyLong_DECIMAL_BASE;
			hi /= _PyLong_DECIMAL_BASE;
		}
	}
	/* pout should have at least one digit, so that the case when a = 0
	   works correctly */
	if (size == 0)
		pout[size++] = 0;

	/* calculate exact length of output string, and allocate */
	Py_ssize_t strlen = negative + 1 + (size - 1) * _PyLong_DECIMAL_SHIFT;
	digit tenpow = 10;
	digit rem = pout[size-1];
	while (rem >= tenpow)
	{
		tenpow *= 10;
		strlen++;
	}
	std::string str(strlen, '_');

	/* fill the string right-to-left */
	std::string::iterator p = str.end();
	/* pout[0] through pout[size-2] contribute exactly
	   _PyLong_DECIMAL_SHIFT digits each */
	Py_ssize_t i;
	for (i=0; i < size - 1; i++)
	{
		rem = pout[i];
		for (Py_ssize_t j = 0; j < _PyLong_DECIMAL_SHIFT; j++)
		{
			*--p = '0' + rem % 10;
			rem /= 10;
		}
	}
	/* pout[size-1]: always produce at least one decimal digit */
	rem = pout[i];
	do {
		*--p = '0' + rem % 10;
		rem /= 10;
	} while (rem != 0);

	/* and sign */
	if (negative)
		*--p = '-';

	/* check we've counted correctly */
	assert(p == str.begin());
	return str;
}

// 1659
/* Convert a long int object to a string, using a given conversion base,
   which should be one of 2, 8, 10 or 16.  Return a string object.
   If base is 2, 8 or 16, add the proper prefix '0b', '0o' or '0x'. */
std::string _PyLong_Format(const bigint::Impl *a, int base)
{
	if (base == 10)
		return long_to_decimal_string(a);
	assert(a);
	assert(base == 2 || base == 8 || base == 16);

	/* Compute a rough upper bound for the length of the string */
	int bits;
	switch (base)
	{
	case 16:
		bits = 4;
		break;
	case 8:
		bits = 3;
		break;
	case 2:
		bits = 1;
		break;
	default:
		assert(0); /* shouldn't ever get here */
	}
	/* compute length of output string: allow 2 characters for prefix and
	   1 for possible '-' sign. */
	Py_ssize_t size_a = abs(a->ob_size);
	if (size_a > (PY_SSIZE_T_MAX - 3) / PyLong_SHIFT)
		throw std::overflow_error("int is too large to format");
	/* now size_a * PyLong_SHIFT + 3 <= PY_SSIZE_T_MAX, so the RHS below
	   is safe from overflow */
	char sign = (a->ob_size < 0)? '-' : '\0';
	Py_ssize_t sz = (sign? 1 : 0) +
		(size_a * PyLong_SHIFT + (bits - 1)) / bits;
	assert(sz >= 0);
	std::string str(sz, '_');
	std::string::iterator p = str.end();

	if (a->ob_size == 0)
		*--p = '0';
	else
	{
		/* JRH: special case for power-of-2 bases */
		twodigits accum = 0;
		int accumbits = 0;              /* # of bits in accum */
		for (Py_ssize_t i = 0; i < size_a; ++i)
		{
			accum |= (twodigits)a->ob_digit[i] << accumbits;
			accumbits += PyLong_SHIFT;
			assert(accumbits >= bits);
			do {
				char cdigit = (char)(accum & (base - 1));
				cdigit += (cdigit < 10) ? '0' : 'a'-10;
				assert(p != str.begin());
				*--p = cdigit;
				accumbits -= bits;
				accum >>= bits;
			} while (i < size_a-1 ? accumbits >= bits : accum > 0);
		}
	}

	if (sign)
		*--p = sign;
	str.erase(str.begin(), p);
	return str;
}

// 2171
/* forward */
bigint::Impl* x_divrem(const bigint::Impl *v1, const bigint::Impl *w1, bigint::Impl **prem);

// 2176
/* Long division with remainder, top-level routine */
void long_divrem(const bigint::Impl *a, const bigint::Impl *b,
	bigint::Impl **pdiv, bigint::Impl **prem)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(a);
	assert(b);
	assert(pdiv);
	assert(prem);

	Py_ssize_t size_a = abs(a->ob_size);
	Py_ssize_t size_b = abs(b->ob_size);
	if (size_b == 0)
		throw std::invalid_argument(
			"integer division or modulo by zero");
	if (size_a < size_b || (size_a == size_b &&
		 a->ob_digit[size_a-1] < b->ob_digit[size_b-1]))
	{
		ImplPtr tmp_div(PyLong_FromLong(0));
		ImplPtr tmp_rem(_PyLong_Copy(a));
		*pdiv = tmp_div.release();
		*prem = tmp_rem.release();
		return;
	}
	ImplPtr z;
	if (size_b == 1)
	{
		digit rem = 0;
		z = ImplPtr(divrem1(a, b->ob_digit[0], &rem));
		*prem = PyLong_FromLong(static_cast<long>(rem));
	}
	else
		z = ImplPtr(x_divrem(a, b, prem));
	/* Set the signs.
	   The quotient z has the sign of a*b;
	   the remainder r has the sign of a,
	   so a = b*z + r. */
	if ((a->ob_size < 0) != (b->ob_size < 0))
		long_negate1(z.get());
	if (a->ob_size < 0 && (*prem)->ob_size != 0)
		long_negate1(*prem);
	*pdiv = z.release();
}

// 2229
/* Unsigned long division with remainder -- the algorithm.  The arguments v1
   and w1 should satisfy 2 <= abs(size(w1)) <= abs(size(v1)). */
bigint::Impl* x_divrem(const bigint::Impl *v1, const bigint::Impl *w1,
	bigint::Impl **prem)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	/* We follow Knuth [The Art of Computer Programming, Vol. 2 (3rd
	   edn.), section 4.3.1, Algorithm D], except that we don't explicitly
	   handle the special case when the initial estimate q for a quotient
	   digit is >= PyLong_BASE: the max value for q is PyLong_BASE+1, and
	   that won't overflow a digit. */

	/* allocate space; w will also be used to hold the final remainder */
	Py_ssize_t size_v = abs(v1->ob_size);
	Py_ssize_t size_w = abs(w1->ob_size);
	assert(size_v >= size_w && size_w >= 2); /* Assert checks by div() */
	ImplPtr v(_PyLong_New(size_v + 1));
	ImplPtr w(_PyLong_New(size_w));

	/* normalize: shift w1 left so that its top digit is >= PyLong_BASE/2.
	   shift v1 left by the same amount.  Results go into w and v. */
	digit d = PyLong_SHIFT - bits_in_digit(w1->ob_digit[size_w-1]);
	digit carry = v_lshift(w->ob_digit, w1->ob_digit, size_w, d);
	assert(carry == 0);
	carry = v_lshift(v->ob_digit, v1->ob_digit, size_v, d);
	if (carry != 0 || v->ob_digit[size_v-1] >= w->ob_digit[size_w-1])
	{
		v->ob_digit[size_v] = carry;
		size_v++;
	}

	/* Now v->ob_digit[size_v-1] < w->ob_digit[size_w-1], so quotient has
	   at most (and usually exactly) k = size_v - size_w digits. */
	Py_ssize_t k = size_v - size_w;
	assert(k >= 0);
	ImplPtr a(_PyLong_New(k));
	digit *v0 = v->ob_digit;
	digit *w0 = w->ob_digit;
	digit wm1 = w0[size_w-1];
	digit wm2 = w0[size_w-2];
	for (digit *vk = v0+k, *ak = a->ob_digit + k; vk-- > v0; )
	{
		/* inner loop: divide vk[0:size_w+1] by w0[0:size_w], giving
		   single-digit quotient q, remainder in vk[0:size_w]. */

		/* estimate quotient digit q; may overestimate by 1 (rare) */
		digit vtop = vk[size_w];
		assert(vtop <= wm1);
		twodigits vv = ((twodigits)vtop << PyLong_SHIFT) | vk[size_w-1];
		digit q = (digit)(vv / wm1);
		digit r = (digit)(vv - (twodigits)wm1 * q); /* r = vv % wm1 */
		while ((twodigits)wm2 * q > (((twodigits)r << PyLong_SHIFT)
									 | vk[size_w-2]))
		{
			--q;
			r += wm1;
			if (r >= PyLong_BASE)
				break;
		}
		assert(q <= PyLong_BASE);

		/* subtract q*w0[0:size_w] from vk[0:size_w+1] */
		sdigit zhi = 0;
		for (Py_ssize_t i = 0; i < size_w; ++i)
		{
			/* invariants: -PyLong_BASE <= -q <= zhi <= 0;
			   -PyLong_BASE * q <= z < PyLong_BASE */
			twodigits z = (sdigit)vk[i] + zhi -
				(stwodigits)q * (stwodigits)w0[i];
			vk[i] = (digit)z & PyLong_MASK;
			zhi = (sdigit)Py_ARITHMETIC_RIGHT_SHIFT(stwodigits,
				z, PyLong_SHIFT);
		}

		/* add w back if q was too large (this branch taken rarely) */
		assert((sdigit)vtop + zhi == -1 || (sdigit)vtop + zhi == 0);
		if ((sdigit)vtop + zhi < 0)
		{
			carry = 0;
			for (Py_ssize_t i = 0; i < size_w; ++i)
			{
				carry += vk[i] + w0[i];
				vk[i] = carry & PyLong_MASK;
				carry >>= PyLong_SHIFT;
			}
			--q;
		}

		/* store quotient digit */
		assert(q < PyLong_BASE);
		*--ak = q;
	}

	/* unshift remainder; we reuse w to store the result */
	carry = v_rshift(w0, v0, size_w, d);
	assert(carry==0);

	*prem = long_normalize(w.release());
	return long_normalize(a.release());
}

// 2511
int long_compare(const bigint::Impl *a, const bigint::Impl *b)
{
	assert(a);
	assert(b);

	Py_ssize_t sign;
	if (a->ob_size != b->ob_size)
		sign = a->ob_size - b->ob_size;
	else
	{
		Py_ssize_t i = abs(a->ob_size);
		while (--i >= 0 && a->ob_digit[i] == b->ob_digit[i])
			;
		if (i < 0)
			sign = 0;
		else
		{
			sign = (sdigit)a->ob_digit[i] - (sdigit)b->ob_digit[i];
			if (a->ob_size < 0)
				sign = -sign;
		}
	}
	return (sign < 0)? -1 : ((sign > 0)? 1 : 0);
}

// 2633
/* Add the absolute values of two long integers. */
bigint::Impl* x_add(const bigint::Impl *a, const bigint::Impl *b)
{
	Py_ssize_t size_a = abs(a->ob_size);
	Py_ssize_t size_b = abs(b->ob_size);

	/* Ensure a is the larger of the two: */
	if (size_a < size_b)
	{
		std::swap(a, b);
		std::swap(size_a, size_b);
	}
	bigint::Impl* z = _PyLong_New(size_a + 1);
	Py_ssize_t i;
	digit carry = 0;
	for (i = 0; i < size_b; ++i)
	{
		carry += a->ob_digit[i] + b->ob_digit[i];
		z->ob_digit[i] = carry & PyLong_MASK;
		carry >>= PyLong_SHIFT;
	}
	for (; i < size_a; ++i)
	{
		carry += a->ob_digit[i];
		z->ob_digit[i] = carry & PyLong_MASK;
		carry >>= PyLong_SHIFT;
	}
	z->ob_digit[i] = carry;
	return long_normalize(z);
}

// 2667
/* Subtract the absolute values of two integers. */
bigint::Impl* x_sub(const bigint::Impl *a, const bigint::Impl *b)
{
	Py_ssize_t size_a = abs(a->ob_size);
	Py_ssize_t size_b = abs(b->ob_size);

	/* Ensure a is the larger of the two: */
	int sign = 1;
	if (size_a < size_b)
	{
		sign = -1;
		std::swap(a, b);
		std::swap(size_a, size_b);
	}
	else if (size_a == size_b)
	{
		/* Find highest digit where a and b differ: */
		Py_ssize_t i = size_a;
		while (--i >= 0 && a->ob_digit[i] == b->ob_digit[i])
			;
		if (i < 0)
			return PyLong_FromLong(0);
		if (a->ob_digit[i] < b->ob_digit[i])
		{
			sign = -1;
			std::swap(a, b);
		}
		size_a = size_b = i+1;
	}
	bigint::Impl* z = _PyLong_New(size_a);
	Py_ssize_t i;
	digit borrow = 0;
	for (i = 0; i < size_b; ++i)
	{
		/* The following assumes unsigned arithmetic
		   works module 2**N for some N>PyLong_SHIFT. */
		borrow = a->ob_digit[i] - b->ob_digit[i] - borrow;
		z->ob_digit[i] = borrow & PyLong_MASK;
		borrow >>= PyLong_SHIFT;
		borrow &= 1; /* Keep only one sign bit */
	}
	for (; i < size_a; ++i)
	{
		borrow = a->ob_digit[i] - borrow;
		z->ob_digit[i] = borrow & PyLong_MASK;
		borrow >>= PyLong_SHIFT;
		borrow &= 1; /* Keep only one sign bit */
	}
	assert(borrow == 0);
	if (sign < 0)
		long_negate1(z);
	return long_normalize(z);
}

// 2722
bigint::Impl* long_add(const bigint::Impl *a, const bigint::Impl *b)
{
	assert(a);
	assert(b);

	if (abs(a->ob_size) <= 1 && abs(b->ob_size) <= 1)
		return PyLong_FromLong(MEDIUM_VALUE(a) + MEDIUM_VALUE(b));

	bigint::Impl *z;
	if (a->ob_size < 0)
	{
		if (b->ob_size < 0)
		{
			z = x_add(a, b);
			if (z && z->ob_size)
				long_negate1(z);
		}
		else
			z = x_sub(b, a);
	}
	else
	{
		if (b->ob_size < 0)
			z = x_sub(a, b);
		else
			z = x_add(a, b);
	}
	return z;
}

// 2752
bigint::Impl* long_sub(const bigint::Impl *a, const bigint::Impl *b)
{
	assert(a);
	assert(b);

	if (abs(a->ob_size) <= 1 && abs(b->ob_size) <= 1)
		return PyLong_FromLong(MEDIUM_VALUE(a) - MEDIUM_VALUE(b));

	bigint::Impl *z;
	if (a->ob_size < 0)
	{
		if (b->ob_size < 0)
			z = x_sub(a, b);
		else
			z = x_add(a, b);
		if (z != NULL && z->ob_size != 0)
			long_negate1(z);
	}
	else
	{
		if (b->ob_size < 0)
			z = x_add(a, b);
		else
			z = x_sub(a, b);
	}
	return z;
}

// 2781
/* Grade school multiplication, ignoring the signs.
 * Returns the absolute value of the product, or NULL if error.
 */
bigint::Impl* x_mul(const bigint::Impl *a, const bigint::Impl *b)
{
	Py_ssize_t size_a = abs(a->ob_size);
	Py_ssize_t size_b = abs(b->ob_size);

	bigint::Impl* z = _PyLong_New(size_a + size_b);
	memset(z->ob_digit, 0, z->ob_size * sizeof(digit));

	if (a == b)
	{
		/* Efficient squaring per HAC, Algorithm 14.16:
		 * http://www.cacr.math.uwaterloo.ca/hac/about/chap14.pdf
		 * Gives slightly less than a 2x speedup when a == b,
		 * via exploiting that each entry in the multiplication
		 * pyramid appears twice (except for the size_a squares).
		 */
		for (Py_ssize_t i = 0; i < size_a; ++i)
		{
			twodigits carry;
			twodigits f = a->ob_digit[i];
			digit *pz = z->ob_digit + (i << 1);
			const digit *pa = a->ob_digit + i + 1;
			const digit *paend = a->ob_digit + size_a;

			carry = *pz + f * f;
			*pz++ = (digit)(carry & PyLong_MASK);
			carry >>= PyLong_SHIFT;
			assert(carry <= PyLong_MASK);

			/* Now f is added in twice in each column of the
			 * pyramid it appears.  Same as adding f<<1 once.
			 */
			f <<= 1;
			while (pa < paend)
			{
				carry += *pz + *pa++ * f;
				*pz++ = (digit)(carry & PyLong_MASK);
				carry >>= PyLong_SHIFT;
				assert(carry <= (PyLong_MASK << 1));
			}
			if (carry)
			{
				carry += *pz;
				*pz++ = (digit)(carry & PyLong_MASK);
				carry >>= PyLong_SHIFT;
			}
			if (carry)
				*pz += (digit)(carry & PyLong_MASK);
			assert((carry >> PyLong_SHIFT) == 0);
		}
	}
	else
	{
		/* a is not the same as b -- gradeschool long mult */
		for (Py_ssize_t i = 0; i < size_a; ++i)
		{
			twodigits carry = 0;
			twodigits f = a->ob_digit[i];
			digit *pz = z->ob_digit + i;
			const digit *pb = b->ob_digit;
			const digit *pbend = b->ob_digit + size_b;

			while (pb < pbend)
			{
				carry += *pz + *pb++ * f;
				*pz++ = (digit)(carry & PyLong_MASK);
				carry >>= PyLong_SHIFT;
				assert(carry <= PyLong_MASK);
			}
			if (carry)
				*pz += (digit)(carry & PyLong_MASK);
			assert((carry >> PyLong_SHIFT) == 0);
		}
	}
	return long_normalize(z);
}

// 2868
/* A helper for Karatsuba multiplication (k_mul).
   Takes a long "n" and an integer "size" representing the place to
   split, and sets low and high such that abs(n) == (high << size) + low,
   viewing the shift as being by digits.  The sign bit is ignored, and
   the return values are >= 0.
*/
void kmul_split(const bigint::Impl *n, Py_ssize_t size,
	bigint::Impl **high, bigint::Impl **low)
{
	const Py_ssize_t size_n = abs(n->ob_size);
	Py_ssize_t size_lo = std::min(size_n, size);
	Py_ssize_t size_hi = size_n - size_lo;

	std::auto_ptr<bigint::Impl> hi(_PyLong_New(size_hi));
	bigint::Impl* lo = _PyLong_New(size_lo);

	memcpy(lo->ob_digit, n->ob_digit, size_lo * sizeof(digit));
	memcpy(hi->ob_digit, n->ob_digit + size_lo, size_hi * sizeof(digit));

	*high = long_normalize(hi.release());
	*low = long_normalize(lo);
}

// 2903
bigint::Impl* k_lopsided_mul(bigint::Impl *a, bigint::Impl *b);

// 2905
/* Karatsuba multiplication.  Ignores the input signs, and returns the
 * absolute value of the product (or NULL if error).
 * See Knuth Vol. 2 Chapter 4.3.3 (Pp. 294-295).
 */
bigint::Impl* k_mul(bigint::Impl *a, bigint::Impl *b)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	/* (ah*X+al)(bh*X+bl) = ah*bh*X*X + (ah*bl + al*bh)*X + al*bl
	 * Let k = (ah+al)*(bh+bl) = ah*bl + al*bh  + ah*bh + al*bl
	 * Then the original product is
	 *     ah*bh*X*X + (k - ah*bh - al*bl)*X + al*bl
	 * By picking X to be a power of 2, "*X" is just shifting, and it's
	 * been reduced to 3 multiplies on numbers half the size.
	 */

	/* We want to split based on the larger number; fiddle so that b
	 * is largest.
	 */
	Py_ssize_t asize = abs(a->ob_size);
	Py_ssize_t bsize = abs(b->ob_size);
	if (asize > bsize)
	{
		std::swap(a, b);
		std::swap(asize, bsize);
	}

	/* Use gradeschool math when either number is too small. */
	Py_ssize_t i = (a == b)? KARATSUBA_SQUARE_CUTOFF : KARATSUBA_CUTOFF;
	if (asize <= i)
	{
		if (asize == 0)
			return PyLong_FromLong(0);
		else
			return x_mul(a, b);
	}

	/* If a is small compared to b, splitting on b gives a degenerate
	 * case with ah==0, and Karatsuba may be (even much) less efficient
	 * than "grade school" then.  However, we can still win, by viewing
	 * b as a string of "big digits", each of width a->ob_size.  That
	 * leads to a sequence of balanced calls to k_mul.
	 */
	if (2 * asize <= bsize)
		return k_lopsided_mul(a, b);

	/* Split a & b into hi & lo pieces. */
	Py_ssize_t shift = bsize >> 1;  /* the number of digits we split off */
	bigint::Impl *tmp_h = NULL;
	bigint::Impl *tmp_l = NULL;
	kmul_split(a, shift, &tmp_h, &tmp_l);
	ImplPtr ah(tmp_h);
	ImplPtr al(tmp_l);
	assert(ah->ob_size > 0);  /* the split isn't degenerate */

	ImplPtr bh;
	ImplPtr bl;
	if (a == b)
	{
		bh = ImplPtr(_PyLong_Copy(ah.get()));
		bl = ImplPtr(_PyLong_Copy(al.get()));
	}
	else
	{
		bigint::Impl *tmp_h = NULL;
		bigint::Impl *tmp_l = NULL;
		kmul_split(b, shift, &tmp_h, &tmp_l);
		bh = ImplPtr(tmp_h);
		bl = ImplPtr(tmp_l);
	}

	/* The plan:
	 * 1. Allocate result space (asize + bsize digits:  that's always
	 *    enough).
	 * 2. Compute ah*bh, and copy into result at 2*shift.
	 * 3. Compute al*bl, and copy into result at 0.  Note that this
	 *    can't overlap with #2.
	 * 4. Subtract al*bl from the result, starting at shift.  This may
	 *    underflow (borrow out of the high digit), but we don't care:
	 *    we're effectively doing unsigned arithmetic mod
	 *    BASE**(sizea + sizeb), and so long as the *final* result fits,
	 *    borrows and carries out of the high digit can be ignored.
	 * 5. Subtract ah*bh from the result, starting at shift.
	 * 6. Compute (ah+al)*(bh+bl), and add it into the result starting
	 *    at shift.
	 */

	/* 1. Allocate result space. */
	ImplPtr ret(_PyLong_New(asize + bsize));
#ifdef Py_DEBUG
	/* Fill with trash, to catch reference to uninitialized digits. */
	memset(ret->ob_digit, 0xDF, ret->ob_size * sizeof(digit));
#endif

	/* 2. t1 <- ah*bh, and copy into high digits of result. */
	ImplPtr t1(k_mul(ah.get(), bh.get()));
	assert(t1->ob_size >= 0);
	assert(2*shift + t1->ob_size <= ret->ob_size);
	memcpy(ret->ob_digit + 2*shift, t1->ob_digit,
		   t1->ob_size * sizeof(digit));

	/* Zero-out the digits higher than the ah*bh copy. */
	i = ret->ob_size - 2*shift - t1->ob_size;
	if (i)
		memset(ret->ob_digit + 2*shift + t1->ob_size, 0,
			   i * sizeof(digit));

	/* 3. t2 <- al*bl, and copy into the low digits. */
	ImplPtr t2(k_mul(al.get(), bl.get()));
	assert(t2->ob_size >= 0);
	assert(t2->ob_size <= 2*shift); /* no overlap with high digits */
	memcpy(ret->ob_digit, t2->ob_digit, t2->ob_size * sizeof(digit));

	/* Zero out remaining digits. */
	i = 2 * shift - t2->ob_size;  /* number of uninitialized digits */
	if (i)
		memset(ret->ob_digit + t2->ob_size, 0, i * sizeof(digit));

	/* 4 & 5. Subtract ah*bh (t1) and al*bl (t2).  We do al*bl first
	 * because it's fresher in cache.
	 */
	i = ret->ob_size - shift;  /* # digits after shift */
	(void)v_isub(ret->ob_digit + shift, i, t2->ob_digit, t2->ob_size);

	(void)v_isub(ret->ob_digit + shift, i, t1->ob_digit, t1->ob_size);

	/* 6. t3 <- (ah+al)(bh+bl), and add into result. */
	t1 = ImplPtr(x_add(ah.get(), al.get()));

	if (a == b)
		t2 = ImplPtr(_PyLong_Copy(t1.get()));
	else
		t2 = ImplPtr(x_add(bh.get(), bl.get()));

	ImplPtr t3(k_mul(t1.get(), t2.get()));
	assert(t3->ob_size >= 0);

	/* Add t3.  It's not obvious why we can't run out of room here.
	 * See the (*) comment after this function.
	 */
	(void)v_iadd(ret->ob_digit + shift, i, t3->ob_digit, t3->ob_size);

	return long_normalize(ret.release());
}

// 3122
/* b has at least twice the digits of a, and a is big enough that Karatsuba
 * would pay off *if* the inputs had balanced sizes.  View b as a sequence
 * of slices, each with a->ob_size digits, and multiply the slices by a,
 * one at a time.  This gives k_mul balanced inputs to work with, and is
 * also cache-friendly (we compute one double-width slice of the result
 * at a time, then move on, never backtracking except for the helpful
 * single-width slice overlap between successive partial sums).
 */
bigint::Impl* k_lopsided_mul(bigint::Impl *a, bigint::Impl *b)
{
	const Py_ssize_t asize = abs(a->ob_size);
	Py_ssize_t bsize = abs(b->ob_size);

	assert(asize > KARATSUBA_CUTOFF);
	assert(2 * asize <= bsize);

	/* Allocate result space, and zero it out. */
	std::auto_ptr<bigint::Impl> ret(_PyLong_New(asize + bsize));
	memset(ret->ob_digit, 0, ret->ob_size * sizeof(digit));

	/* Successive slices of b are copied into bslice. */
	std::auto_ptr<bigint::Impl> bslice(_PyLong_New(asize));

	Py_ssize_t nbdone = 0;  /* # of b digits already multiplied */
	while (bsize > 0)
	{
		const Py_ssize_t nbtouse = std::min(bsize, asize);

		/* Multiply the next slice of b by a. */
		memcpy(bslice->ob_digit, b->ob_digit + nbdone,
			   nbtouse * sizeof(digit));
		bslice->ob_size = nbtouse;
		bigint::Impl *product = k_mul(a, bslice.get());

		/* Add into result. */
		(void)v_iadd(
			ret->ob_digit + nbdone, ret->ob_size - nbdone,
			product->ob_digit, product->ob_size);
		delete product;

		bsize -= nbtouse;
		nbdone += nbtouse;
	}

	return long_normalize(ret.release());
}


// 3185
bigint::Impl* long_mul(bigint::Impl* a, bigint::Impl* b)
{
	assert(a);
	assert(b);

	/* fast path for single-digit multiplication */
	if (abs(a->ob_size) <= 1 && abs(b->ob_size) <= 1)
	{
		stwodigits v = (stwodigits)(MEDIUM_VALUE(a)) * MEDIUM_VALUE(b);
#ifdef HAVE_LONG_LONG
		return PyLong_FromLongLong((PY_LONG_LONG)v);
#else
		/* if we don't have long long then we're almost certainly
		   using 15-bit digits, so v will fit in a long.  In the
		   unlikely event that we're using 30-bit digits on a platform
		   without long long, a large v will just cause us to fall
		   through to the general multiplication code below. */
		if (v >= std::numeric_limits<long>::min() &&
			v <= std::numeric_limits<long>::max())
			return PyLong_FromLong(static_cast<long>(v));
#endif
	}

	bigint::Impl *z = k_mul(a, b);
	/* Negate if exactly one of the inputs is negative. */
	if (((a->ob_size ^ b->ob_size) < 0) && z)
		long_negate1(z);
	return z;
}

// 3229
/* Compute
 *     *pdiv, *pmod = divmod(v, w)
 * NULL can be passed for pdiv or pmod, in which case that part of
 * the result is simply thrown away.  The caller owns a reference to
 * each of these it requests (does not pass NULL for).
 */
void l_divmod(const bigint::Impl *v, const bigint::Impl *w,
	bigint::Impl **pdiv, bigint::Impl **pmod)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;

	bigint::Impl* tmp_div;
	bigint::Impl* tmp_mod;
	long_divrem(v, w, &tmp_div, &tmp_mod);
	ImplPtr div(tmp_div);
	ImplPtr mod(tmp_mod);

	if ((mod->ob_size < 0 && w->ob_size > 0) ||
		(mod->ob_size > 0 && w->ob_size < 0))
	{
		mod = ImplPtr(long_add(mod.get(), w));
		ImplPtr one(PyLong_FromLong(1L));
		div = ImplPtr(long_sub(div.get(), one.get()));
	}

	if (pdiv != NULL)
		*pdiv = div.release();
	if (pmod != NULL)
		*pmod = mod.release();
}

// 3775
bigint::Impl* long_invert(const bigint::Impl *v)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(v);

	/* Implement ~x as -(x+1) */
	if (abs(v->ob_size) <= 1)
		return PyLong_FromLong(-(MEDIUM_VALUE(v) + 1));
	ImplPtr w(PyLong_FromLong(1L));
	ImplPtr x(long_add(v, w.get()));
	long_negate1(x.get());
	return x.release();
}


// 3795
bigint::Impl* long_neg(const bigint::Impl *v)
{
	assert(v);
	if (abs(v->ob_size) <= 1)
		return PyLong_FromLong(-MEDIUM_VALUE(v));
	bigint::Impl* z = _PyLong_Copy(v);
	long_negate1(z);
	return z;
}

// 3806
bigint::Impl* long_abs(const bigint::Impl *v)
{
	assert(v);
	if (v->ob_size < 0)
		return long_neg(v);
	else
		return _PyLong_Copy(v);
}

// 3821
bigint::Impl* long_rshift(const bigint::Impl *a, const bigint::Impl *b)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(a);
	assert(b);

	if (a->ob_size < 0)
	{
		/* Right shifting negative numbers is harder */
		ImplPtr a1(long_invert(a));
		ImplPtr a2(long_rshift(a1.get(), b));
		return long_invert(a2.get());
	}

	Py_ssize_t shiftby = PyLong_AsSsize_t(b);
	if (shiftby < 0)
		throw std::invalid_argument("negative shift count");
	Py_ssize_t wordshift = shiftby / PyLong_SHIFT;
	Py_ssize_t newsize = abs(a->ob_size) - wordshift;
	if (newsize <= 0)
		return PyLong_FromLong(0);
	Py_ssize_t loshift = shiftby % PyLong_SHIFT;
	Py_ssize_t hishift = PyLong_SHIFT - loshift;
	digit lomask = ((digit)1 << hishift) - 1;
	digit himask = PyLong_MASK ^ lomask;
	bigint::Impl *z = _PyLong_New(newsize);
	if (a->ob_size < 0)
		z->ob_size = -(z->ob_size);
	for (Py_ssize_t i = 0, j = wordshift; i < newsize; i++, j++)
	{
		z->ob_digit[i] = (a->ob_digit[j] >> loshift) & lomask;
		if (i+1 < newsize)
			z->ob_digit[i] |= (a->ob_digit[j+1] << hishift) & himask;
	}
	return long_normalize(z);
}

// 3877
bigint::Impl* long_lshift(const bigint::Impl *a, const bigint::Impl *b)
{
	assert(a);
	assert(b);

	/* This version due to Tim Peters */
	Py_ssize_t shiftby = PyLong_AsSsize_t(b);
	if (shiftby < 0)
		throw std::invalid_argument("negative shift count");
	/* wordshift, remshift = divmod(shiftby, PyLong_SHIFT) */
	Py_ssize_t wordshift = shiftby / PyLong_SHIFT;
	Py_ssize_t remshift  = shiftby - wordshift * PyLong_SHIFT;

	Py_ssize_t oldsize = abs(a->ob_size);
	Py_ssize_t newsize = oldsize + wordshift;
	if (remshift)
		++newsize;
	bigint::Impl *z = _PyLong_New(newsize);
	if (a->ob_size < 0)
		long_negate1(z);
	for (Py_ssize_t i = 0; i < wordshift; i++)
		z->ob_digit[i] = 0;
	twodigits accum = 0;
	for (Py_ssize_t i = wordshift, j = 0; j < oldsize; i++, j++)
	{
		accum |= (twodigits)a->ob_digit[j] << remshift;
		z->ob_digit[i] = (digit)(accum & PyLong_MASK);
		accum >>= PyLong_SHIFT;
	}
	if (remshift)
		z->ob_digit[newsize-1] = (digit)accum;
	else
		assert(!accum);
	return long_normalize(z);
}

/* Compute two's complement of digit vector a[0:m], writing result to
   z[0:m].  The digit vector a need not be normalized, but should not
   be entirely zero.  a and z may point to the same digit vector. */
void v_complement(digit *z, const digit *a, Py_ssize_t m)
{
	digit carry = 1;
	for (Py_ssize_t i = 0; i < m; ++i)
	{
		carry += a[i] ^ PyLong_MASK;
		z[i] = carry & PyLong_MASK;
		carry >>= PyLong_SHIFT;
	}
	assert(carry == 0);
}

// 3943
/* Bitwise and/xor/or operations. op is '&', '|' or '^' */
bigint::Impl* long_bitwise(const bigint::Impl *a, int op,
	const bigint::Impl *b)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(a);
	assert(b);

	/* Bitwise operations for negative numbers operate as though
	   on a two's complement representation.  So convert arguments
	   from sign-magnitude to two's complement, and convert the
	   result back to sign-magnitude at the end. */

	/* If a is negative, replace it by its two's complement. */
	Py_ssize_t size_a = abs(a->ob_size);
	int nega = a->ob_size < 0;
	ImplPtr a1;
	if (nega)
	{
		a1 = ImplPtr(_PyLong_New(size_a));
		v_complement(a1->ob_digit, a->ob_digit, size_a);
	}
	else
		/* Keep reference count consistent. */
		a1 = ImplPtr(_PyLong_Copy(a));

	/* Same for b. */
	Py_ssize_t size_b = abs(b->ob_size);
	int negb = b->ob_size < 0;
	ImplPtr b1;
	if (negb)
	{
		b1 = ImplPtr(_PyLong_New(size_b));
		v_complement(b1->ob_digit, b->ob_digit, size_b);
	}
	else
		b1 = ImplPtr(_PyLong_Copy(b));

	/* Swap a and b if necessary to ensure size_a >= size_b. */
	if (size_a < size_b)
	{
		std::swap(a1, b1);
		std::swap(size_a, size_b);
		std::swap(nega, negb);
	}

	/* JRH: The original logic here was to allocate the result value (z)
	   as the longer of the two operands.  However, there are some cases
	   where the result is guaranteed to be shorter than that: AND of two
	   positives, OR of two negatives: use the shorter number.  AND with
	   mixed signs: use the positive number.  OR with mixed signs: use the
	   negative number.
	*/
	int negz;
	Py_ssize_t size_z;
	switch (op)
	{
	case '^':
		negz = nega ^ negb;
		size_z = size_a;
		break;
	case '&':
		negz = nega & negb;
		size_z = (negb)? size_a : size_b;
		break;
	case '|':
		negz = nega | negb;
		size_z = (negb)? size_b : size_a;
		break;
	default:
		throw std::invalid_argument("unknown binary operator");
	}

	/* We allow an extra digit if z is negative, to make sure that
	   the final two's complement of z doesn't overflow. */
	bigint::Impl* z = _PyLong_New(size_z + negz);

	/* Compute digits for overlap of a and b. */
	Py_ssize_t i;
	switch (op)
	{
	case '&':
		for (i = 0; i < size_b; ++i)
			z->ob_digit[i] = a1->ob_digit[i] & b1->ob_digit[i];
		break;
	case '|':
		for (i = 0; i < size_b; ++i)
			z->ob_digit[i] = a1->ob_digit[i] | b1->ob_digit[i];
		break;
	case '^':
		for (i = 0; i < size_b; ++i)
			z->ob_digit[i] = a1->ob_digit[i] ^ b1->ob_digit[i];
		break;
	default:
		throw std::invalid_argument("unknown binary operator");
	}

	/* Copy any remaining digits of a, inverting if necessary. */
	if (op == '^' && negb)
		for (; i < size_z; ++i)
			z->ob_digit[i] = a1->ob_digit[i] ^ PyLong_MASK;
	else if (i < size_z)
		memcpy(&z->ob_digit[i], &a1->ob_digit[i],
			   (size_z-i)*sizeof(digit));

	/* Complement result if negative. */
	if (negz)
	{
		z->ob_size = -(z->ob_size);
		z->ob_digit[size_z] = PyLong_MASK;
		v_complement(z->ob_digit, z->ob_digit, size_z+1);
	}
	return long_normalize(z);
}

bigint::Impl* long_pow(const bigint::Impl* n, const bigint::Impl* exp)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(n);
	assert(exp);

	if (exp->ob_size < 0)
		throw not_implemented("pow for negative exponents");

	ImplPtr result(PyLong_FromUnsignedLong(1));
	ImplPtr tmp(_PyLong_Copy(n));
	ImplPtr exp1(_PyLong_Copy(exp));
	ImplPtr one(PyLong_FromUnsignedLong(1));
	while (exp1->ob_size)
	{
		if (exp1->ob_digit[0] & 1)
			result = ImplPtr(long_mul(result.get(), tmp.get()));
		exp1 = ImplPtr(long_rshift(exp1.get(), one.get()));
		tmp = ImplPtr(long_mul(tmp.get(), tmp.get()));
	}
	return result.release();
}

bigint::Impl* long_fac(const bigint::Impl* n)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(n);

	if (n->ob_size < 0)
		throw std::invalid_argument(
			"facultiy is not defined for negative numbers");

	ImplPtr one(PyLong_FromUnsignedLong(1U));
	if (n->ob_size == 0)
		return one.release();

	ImplPtr result(_PyLong_Copy(n));
	ImplPtr i(long_sub(result.get(), one.get()));
	while (i->ob_size)
	{
		result = ImplPtr(long_mul(result.get(), i.get()));
		i = ImplPtr(long_sub(i.get(), one.get()));
	}
	return result.release();
}

void long_sqrt(const bigint::Impl* n, bigint::Impl** root, bigint::Impl** rem)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(n);

	if (n->ob_size < 0)
		throw not_implemented("sqrt for negative integers");
	else if (n->ob_size == 0)
	{
		ImplPtr proot(PyLong_FromUnsignedLong(0UL));
		ImplPtr prem(PyLong_FromUnsignedLong(0UL));
		if (root)
			*root = proot.release();
		if (rem)
			*rem = prem.release();
		return;
	}

	ImplPtr one(PyLong_FromUnsignedLong(1UL));
	ImplPtr two(PyLong_FromUnsignedLong(2UL));
	ImplPtr x1(_PyLong_Copy(n));

	while (true)
	{
		bigint::Impl *tmp_div;
		bigint::Impl *tmp_rem;
		long_divrem(n, x1.get(), &tmp_div, &tmp_rem);
		ImplPtr x2(tmp_div);
		bool has_fraction = (tmp_rem->ob_size != 0);
		_PyLong_Delete(tmp_rem);
		x2 = ImplPtr(long_add(x2.get(), x1.get()));
		long_divrem(x2.get(), two.get(), &tmp_div, &tmp_rem);
		x2 = ImplPtr(tmp_div);
		_PyLong_Delete(tmp_rem);

		std::swap(x1, x2);

		ImplPtr x3(long_sub(x1.get(), x2.get()));
		has_fraction = has_fraction & (x3->ob_size < 0);
		if (x3->ob_size < 0)
			long_negate1(x3.get());

		int cmp_result = long_compare(x3.get(), one.get());
		if (cmp_result < 0 || (cmp_result == 0 && has_fraction))
			break;
	}
	ImplPtr prem(long_mul(x1.get(), x1.get()));
	prem = ImplPtr(long_sub(n, prem.get()));
	if (root)
		*root = x1.release();
	if (rem)
		*rem = prem.release();
}

void long_root_bsearch(const bigint::Impl* base, const bigint::Impl* exp,
	bigint::Impl** root, bigint::Impl** rem)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(base);
	assert(exp);
	assert(base->ob_size > 1);
	assert(exp->ob_size >= 0);

	ImplPtr one(PyLong_FromUnsignedLong(1UL));
	ImplPtr two(PyLong_FromUnsignedLong(2UL));

	ImplPtr lower(PyLong_FromUnsignedLong(1UL));
	ImplPtr upper(PyLong_FromUnsignedLong(2UL));
	ImplPtr power(long_pow(upper.get(), exp));
	while (long_compare(power.get(), base) <= 0)
	{
		std::swap(lower, upper);
		upper = ImplPtr(long_lshift(lower.get(), one.get()));
		power = ImplPtr(long_pow(upper.get(), exp));
	}
	ImplPtr diff(long_sub(upper.get(), lower.get()));
	while (long_compare(diff.get(), one.get()) > 0)
	{
		ImplPtr middle(long_add(lower.get(), upper.get()));
		middle = ImplPtr(long_rshift(middle.get(), one.get()));
		power = ImplPtr(long_pow(middle.get(), exp));
		if (long_compare(power.get(), base) <= 0)
			std::swap(lower, middle);
		else
			std::swap(upper, middle);
		diff = ImplPtr(long_sub(upper.get(), lower.get()));
	}
	power = ImplPtr(long_pow(lower.get(), exp));
	ImplPtr remainder(long_sub(base, power.get()));
	if (root)
		*root = lower.release();
	if (rem)
		*rem = remainder.release();
}

#define NTH_ROOT_BSEARCH_CUTOFF 50
void long_root(const bigint::Impl* base, const bigint::Impl* exp,
	bigint::Impl** root, bigint::Impl** rem)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(base);
	assert(exp);

	if (base->ob_size < 0)
		throw not_implemented("root for negative integers");
	if (exp->ob_size <= 0)
		throw std::invalid_argument("root for non-positive exponent");

	if (base->ob_size == 0 ||
		(base->ob_size == 1 && base->ob_digit[0] == 1) ||
		(exp->ob_size == 1 && exp->ob_digit[0] == 1))
	{
		ImplPtr proot(_PyLong_Copy(base));
		ImplPtr prem(PyLong_FromUnsignedLong(0UL));
		if (root)
			*root = proot.release();
		if (rem)
			*rem = prem.release();
		return;
	}
	if (base->ob_size > NTH_ROOT_BSEARCH_CUTOFF)
		return long_root_bsearch(base, exp, root, rem);

	ImplPtr one(PyLong_FromUnsignedLong(1UL));
	ImplPtr two(PyLong_FromUnsignedLong(2UL));
	ImplPtr x1(_PyLong_Copy(base));

	while (true)
	{
		ImplPtr x21(long_sub(exp, one.get()));
		ImplPtr x2(long_pow(x1.get(), x21.get()));
		bigint::Impl *tmp_div;
		bigint::Impl *tmp_rem;
		long_divrem(base, x2.get(), &tmp_div, &tmp_rem);
		x2 = ImplPtr(tmp_div);
		bool has_fraction = (tmp_rem->ob_size != 0);
		_PyLong_Delete(tmp_rem);
		x21 = ImplPtr(long_mul(x21.get(), x1.get()));
		x2 = ImplPtr(long_add(x21.get(), x2.get()));

		long_divrem(x2.get(), exp, &tmp_div, &tmp_rem);
		x2 = ImplPtr(tmp_div);
		_PyLong_Delete(tmp_rem);

		std::swap(x1, x2);

		ImplPtr x3(long_sub(x1.get(), x2.get()));
		has_fraction = has_fraction & (x3->ob_size < 0);
		if (x3->ob_size < 0)
			long_negate1(x3.get());

		int cmp_result = long_compare(x3.get(), one.get());
		if (cmp_result < 0 || (cmp_result == 0 && has_fraction))
			break;
	}
	ImplPtr prem(long_pow(x1.get(), exp));
	prem = ImplPtr(long_sub(base, prem.get()));
	if (root)
		*root = x1.release();
	if (rem)
		*rem = prem.release();
}

bigint::Impl* long_n_over_k(const bigint::Impl* n, const bigint::Impl* k)
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	assert(n);
	assert(k);

	if (n->ob_size < 0)
		throw std::invalid_argument("not defined for negative number");
	if (k->ob_size < 0)
		throw std::invalid_argument("not defined for negative number");

	if (k->ob_size == 0)
		return PyLong_FromUnsignedLong(1UL);
	else if (n->ob_size == 0)
		return PyLong_FromUnsignedLong(0UL);

	// TODO: use more efficient algorithm
	ImplPtr result(long_fac(n));
	ImplPtr tmp(long_fac(k));
	bigint::Impl* tmp_div;
	l_divmod(result.get(), tmp.get(), &tmp_div, 0);
	result = ImplPtr(tmp_div);
	ImplPtr n_k(long_sub(n, k));
	n_k = ImplPtr(long_fac(n_k.get()));
	l_divmod(result.get(), n_k.get(), &tmp_div, 0);
	return tmp_div;
}

} // anonymous namespace

void bigint::safe_bool_converter() const
{ }

bigint::bigint()
	: pimpl_(PyLong_FromUnsignedLong(0))
{ }

bigint::bigint(const bigint& other)
	: pimpl_(_PyLong_Copy(other.pimpl_))
{ }

bigint& bigint::operator=(const bigint& other)
{
	if (this == &other)
		return *this;
	reset(_PyLong_Copy(other.pimpl_));
	return *this;
}

bigint::bigint(Impl* pimpl)
	: pimpl_(pimpl)
{
	if (! pimpl)
		throw std::invalid_argument("null pointer");
}

bigint::bigint(const Impl& impl)
	: pimpl_(_PyLong_Copy(&impl))
{ }

bigint::bigint(signed char value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(unsigned char value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(signed short value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(unsigned short value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(signed int value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(unsigned int value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(signed long value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(unsigned long value)
	: pimpl_(PyLong_FromLong(value))
{ }

bigint::bigint(double value)
	: pimpl_(PyLong_FromDouble(value))
{ }

bigint::bigint(const char *text, int base)
	: pimpl_(PyLong_FromUnsignedLong(0))
{
	// TODO: raii
	*this = parse_bigint(text, base);
}

bigint::bigint(const std::string& text, int base)
	: pimpl_(PyLong_FromUnsignedLong(0))
{
	// TODO: raii
	*this = parse_bigint(text, base);
}

bigint::~bigint()
{
	_PyLong_Delete(pimpl_);
}

void bigint::reset(Impl *pimpl)
{
	assert(pimpl);
	_PyLong_Delete(pimpl_);
	pimpl_ = pimpl;
}

void bigint::swap(bigint& other) throw()
{
	std::swap(pimpl_, other.pimpl_);
}

int bigint::sign() const
{
	assert(pimpl_);
	return ((pimpl_->ob_size == 0)? 0 :
		((pimpl_->ob_size < 0)? -1 : 1));
}

const bigint bigint::abs() const
{
	std::auto_ptr<Impl> pimpl(long_abs(pimpl_));
	bigint result(pimpl.get());
	pimpl.release();
	return result;
}

int compare(const bigint& a, const bigint& b)
{
	return long_compare(a.pimpl_, b.pimpl_);
}

bigint& bigint::operator++()
{
	// TODO: use more efficient algorithm
	bigint tmp(1);
	reset(long_add(pimpl_, tmp.pimpl_));
	return *this;
}

bigint& bigint::operator--()
{
	// TODO: use more efficient algorithm
	bigint tmp(1);
	reset(long_sub(pimpl_, tmp.pimpl_));
	return *this;
}

const bigint bigint::operator-() const
{
	std::auto_ptr<Impl> pimpl(long_neg(pimpl_));
	bigint result(pimpl.get());
	pimpl.release();
	return result;
}

bigint& bigint::operator+=(const bigint& other)
{
	reset(long_add(pimpl_, other.pimpl_));
	return *this;
}

bigint& bigint::operator-=(const bigint& other)
{
	reset(long_sub(pimpl_, other.pimpl_));
	return *this;
}

bigint& bigint::operator*=(const bigint& other)
{
	reset(long_mul(pimpl_, other.pimpl_));
	return *this;
}

void divmod(const bigint& dividend, const bigint& divisor,
	bigint *div, bigint *mod)
{
	bigint::Impl* pdiv;
	bigint::Impl* pmod;
	l_divmod(dividend.pimpl_, divisor.pimpl_, &pdiv, &pmod);
	if (div)
		div->reset(pdiv);
	if (mod)
		mod->reset(pmod);
}

void divrem(const bigint& dividend, const bigint& divisor,
	bigint *div, bigint *rem)
{
	bigint::Impl* pdiv;
	bigint::Impl* prem;
	long_divrem(dividend.pimpl_, divisor.pimpl_, &pdiv, &prem);
	if (div)
		div->reset(pdiv);
	if (rem)
		rem->reset(prem);
}

const bigint bigint::operator~() const
{
	std::auto_ptr<Impl> pimpl(long_invert(pimpl_));
	bigint result(pimpl.get());
	pimpl.release();
	return result;
}

bigint& bigint::operator&=(const bigint& other)
{
	reset(long_bitwise(pimpl_, '&', other.pimpl_));
	return *this;
}

bigint& bigint::operator|=(const bigint& other)
{
	reset(long_bitwise(pimpl_, '|', other.pimpl_));
	return *this;
}

bigint& bigint::operator^=(const bigint& other)
{
	reset(long_bitwise(pimpl_, '^', other.pimpl_));
	return *this;
}

bigint& bigint::operator<<=(const bigint& count)
{
	reset(long_lshift(pimpl_, count.pimpl_));
	return *this;
}

bigint& bigint::operator>>=(const bigint& count)
{
	reset(long_rshift(pimpl_, count.pimpl_));
	return *this;
}

std::ostream& operator<<(std::ostream& out, const bigint& value)
{
	// TODO: react on ios_base::uppercase
	int base;
	switch (out.flags() & std::ios_base::basefield)
	{
	case std::ios_base::oct: base = 8; break;
	case std::ios_base::hex: base = 16; break;
	default: base = 10;
	}
	std::string str;
	if (out.flags() & std::ios_base::showpos)
	{
		if (value.sign() >= 0)
			str += "+";
	}
	if (out.flags() & std::ios_base::showbase)
	{
		if (base == 16)
			str += "0x";
		else if (base == 8)
			str += "0";
	}
	str += value.str(base);
	std::ios_base::fmtflags adjustfield = \
		out.flags() & std::ios_base::adjustfield;
	if (adjustfield && out.width() > 0)
	{
		std::string pad(out.width() - str.size(), out.fill());
		if (adjustfield == std::ios_base::left)
			str += pad;
		else
			str = pad + str;
	}
	return out << str;
}

std::istream& operator>>(std::istream&, bigint&)
{
	throw not_implemented(HERE);
}

const std::string bigint::str(int base) const
{
	if (base < 2 || 36 < base)
		throw std::invalid_argument("invalid base");
	return _PyLong_Format(pimpl_, base);
}

const bigint bigint::pow(const bigint& exp) const
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	ImplPtr pimpl(long_pow(pimpl_, exp.pimpl_));
	bigint result(pimpl.get());
	pimpl.release();
	return result;
}

const bigint bigint::over(const bigint& k) const
{
	typedef std::auto_ptr<bigint::Impl> ImplPtr;
	ImplPtr pimpl(long_n_over_k(pimpl_, k.pimpl_));
	bigint result(pimpl.get());
	pimpl.release();
	return result;
}

const bigint::Impl& bigint::data() const
{
	assert(pimpl_);
	return *pimpl_;
}

std::string bigint::debug() const
{
	assert(pimpl_);
	std::string result = '<' + to_string(pimpl_->ob_size);
	Py_ssize_t size = ::abs(pimpl_->ob_size);
	for (Py_ssize_t i = 0; i < size; ++i)
		result += ' ' + to_string(pimpl_->ob_digit[0]);
	return result + '>';
}

const bigint gcd(const bigint& a, const bigint& b)
{
	// TODO: use more efficient algorithm
	if (b > a)
		return gcd(b, a);
	bigint a1 = a;
	bigint b1 = b;
	while (b1)
	{
		bigint tmp = b1;
		b1 = a1 % b1;
		std::swap(a1, tmp);
	}
	return a1;
}

const bigint lcm(const bigint& a, const bigint& b)
{
	// TODO: use more efficient algorithm
	if (!a || !b)
		return bigint();
	return (a * b).abs() / gcd(a, b);
}

const bigint fac(const bigint& n)
{
	std::auto_ptr<bigint::Impl> pimpl(long_fac(&n.data()));
	bigint result(pimpl.get());
	pimpl.release();
	return result;
}

const bigint bigint::root(const bigint& exp) const
{
	bigint rem;
	return this->root(exp, rem);
}

const bigint bigint::root(const bigint& exp, bigint& rem) const
{
	bigint::Impl* tmp_root;
	bigint::Impl* tmp_rem;
	long_root(pimpl_, exp.pimpl_, &tmp_root, &tmp_rem);
	std::auto_ptr<Impl> proot(tmp_root);
	std::auto_ptr<Impl> prem(tmp_rem);
	bigint result(proot.get());
	proot.release();
	rem.reset(prem.release());
	return result;
}

const bigint bigint::sqrt() const
{
	bigint rem;
	return this->sqrt(rem);
}

const bigint bigint::sqrt(bigint& rem) const
{
	bigint::Impl* tmp_root;
	bigint::Impl* tmp_rem;
	long_sqrt(pimpl_, &tmp_root, &tmp_rem);
	std::auto_ptr<Impl> proot(tmp_root);
	std::auto_ptr<Impl> prem(tmp_rem);
	bigint result(proot.get());
	proot.release();
	rem.reset(prem.release());
	return result;
}

