#ifndef BIGINT_H
#define BIGINT_H

#include <iosfwd>

class bigint
{
private:
	typedef void (bigint::*bool_type)() const;
	void safe_bool_converter() const;
public:
	struct Impl;

	bigint();
	bigint(  signed char);
	bigint(unsigned char);
	bigint(  signed short);
	bigint(unsigned short);
	bigint(  signed int);
	bigint(unsigned int);
	bigint(  signed long);
	bigint(unsigned long);
	bigint(double);
	bigint(const char *, int base=0);
	bigint(const std::string&, int base=0);
	explicit bigint(Impl *);

	explicit bigint(const Impl&);
	bigint(const bigint&);
	~bigint();
	bigint& operator=(const bigint&);

	friend int compare(const bigint&, const bigint&);
	// divmod(a, b) == (floor(a / b), a - b * floor(a / b))
	// divrem(a, b) == (round(a / b), a - b * round(a / b))
	//                 if round() rounds towards 0
	friend void divmod(const bigint&, const bigint&, bigint *, bigint *);
	friend void divrem(const bigint&, const bigint&, bigint *, bigint *);

	operator bool_type() const;
	void swap(bigint& other) throw();
	int sign() const;
	bool operator!() const;
	const bigint abs() const;

	bigint& operator++();
	bigint& operator--();
	const bigint operator++(int);
	const bigint operator--(int);

	const bigint operator-() const;
	bigint& operator+=(const bigint&);
	bigint& operator-=(const bigint&);
	bigint& operator*=(const bigint&);
	bigint& operator/=(const bigint&);
	bigint& operator%=(const bigint&);

	const bigint operator~() const;
	bigint& operator&=(const bigint&);
	bigint& operator|=(const bigint&);
	bigint& operator^=(const bigint&);
	bigint& operator<<=(const bigint&);
	bigint& operator>>=(const bigint&);

	const bigint pow(const bigint& exp) const;
	const bigint root(const bigint& exp) const;
	const bigint root(const bigint& exp, bigint& remainder) const;
	const bigint sqrt() const;
	const bigint sqrt(bigint& remainder) const;
	const bigint over(const bigint& k) const;

	const std::string str(int base =10) const;
	friend std::ostream& operator<<(std::ostream&, const bigint&);
	friend std::istream& operator>>(std::istream&, bigint&);

	const Impl& data() const;
	std::string debug() const;

private:
	void reset(Impl *);
	Impl *pimpl_;
};

void swap(bigint&, bigint&) throw();

bool operator==(const bigint& lhs, const bigint& rhs);
bool operator!=(const bigint& lhs, const bigint& rhs);
bool operator< (const bigint& lhs, const bigint& rhs);
bool operator> (const bigint& lhs, const bigint& rhs);
bool operator<=(const bigint& lhs, const bigint& rhs);
bool operator>=(const bigint& lhs, const bigint& rhs);

const bigint& operator+(const bigint&);

const bigint operator+(const bigint& lhs, const bigint& rhs);
const bigint operator-(const bigint& lhs, const bigint& rhs);
const bigint operator*(const bigint& lhs, const bigint& rhs);
const bigint operator/(const bigint& lhs, const bigint& rhs);
const bigint operator%(const bigint& lhs, const bigint& rhs);

const bigint operator& (const bigint& lhs, const bigint& rhs);
const bigint operator| (const bigint& lhs, const bigint& rhs);
const bigint operator^ (const bigint& lhs, const bigint& rhs);
const bigint operator<<(const bigint& lhs, unsigned long rhs);
const bigint operator>>(const bigint& lhs, unsigned long rhs);

const bigint abs(const bigint&);
const bigint pow(const bigint&, const bigint&);
const bigint gcd(const bigint& a, const bigint& b);
const bigint lcm(const bigint& a, const bigint& b);
const bigint fac(const bigint&);
const bigint n_over_k(const bigint& n, const bigint& k);
const bigint root(const bigint& val, const bigint& exp);
const bigint root(const bigint& val, const bigint& exp, bigint& remainder);
const bigint sqrt(const bigint& val);
const bigint sqrt(const bigint& val, bigint& remainder);

template < typename Ty >
bool operator==(const bigint& lhs, const Ty& rhs);
template < typename Ty >
bool operator==(const Ty& lhs, const bigint& rhs);
template < typename Ty >
bool operator!=(const bigint& lhs, const Ty& rhs);
template < typename Ty >
bool operator!=(const Ty& lhs, const bigint& rhs);
template < typename Ty >
bool operator<(const bigint& lhs, const Ty& rhs);
template < typename Ty >
bool operator<(const Ty& lhs, const bigint& rhs);
template < typename Ty >
bool operator<=(const bigint& lhs, const Ty& rhs);
template < typename Ty >
bool operator<=(const Ty& lhs, const bigint& rhs);
template < typename Ty >
bool operator>(const bigint& lhs, const Ty& rhs);
template < typename Ty >
bool operator>(const Ty& lhs, const bigint& rhs);
template < typename Ty >
bool operator>=(const bigint& lhs, const Ty& rhs);
template < typename Ty >
bool operator>=(const Ty& lhs, const bigint& rhs);
template < typename Ty >
const bigint operator+(const bigint& lhs, const Ty& rhs);
template < typename Ty >
const bigint operator+(const Ty& lhs, const bigint& rhs);
template < typename Ty >
const bigint operator-(const bigint& lhs, const Ty& rhs);
template < typename Ty >
const bigint operator-(const Ty& lhs, const bigint& rhs);
template < typename Ty >
const bigint operator*(const bigint& lhs, const Ty& rhs);
template < typename Ty >
const bigint operator*(const Ty& lhs, const bigint& rhs);
template < typename Ty >
const bigint operator/(const bigint& lhs, const Ty& rhs);
template < typename Ty >
const bigint operator/(const Ty& lhs, const bigint& rhs);
template < typename Ty >
const bigint operator%(const bigint& lhs, const Ty& rhs);
template < typename Ty >
const bigint operator%(const Ty& lhs, const bigint& rhs);

// ====================================================== implementation

inline
void swap(bigint& a, bigint& b) throw()
{
	a.swap(b);
}
inline
bigint::operator bool_type() const
{
	return (sign())? &bigint::safe_bool_converter : 0;
}
inline
bool bigint::operator!() const
{
	return sign() == 0;
}
inline
const bigint bigint::operator++(int)
{
	bigint result(*this);
	++*this;
	return result;
}
inline
const bigint bigint::operator--(int)
{
	bigint result(*this);
	--*this;
	return result;
}
inline
bigint& bigint::operator/=(const bigint& other)
{
	divmod(*this, other, this, 0);
	return *this;
}
inline
bigint& bigint::operator%=(const bigint& other)
{
	divmod(*this, other, 0, this);
	return *this;
}
inline
bool operator==(const bigint& lhs, const bigint& rhs)
{
	return compare(lhs, rhs) == 0;
}
inline
bool operator!=(const bigint& lhs, const bigint& rhs)
{
	return compare(lhs, rhs) != 0;
}
inline
bool operator<(const bigint& lhs, const bigint& rhs)
{
	return compare(lhs, rhs) < 0;
}
inline
bool operator>(const bigint& lhs, const bigint& rhs)
{
	return compare(lhs, rhs) > 0;
}
inline
bool operator<=(const bigint& lhs, const bigint& rhs)
{
	return compare(lhs, rhs) <= 0;
}
inline
bool operator>=(const bigint& lhs, const bigint& rhs)
{
	return compare(lhs, rhs) >= 0;
}
inline
const bigint& operator+(const bigint& val)
{
	return val;
}
inline
const bigint operator+(const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) += rhs;
}
inline
const bigint operator-(const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) -= rhs;
}
inline
const bigint operator*(const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) *= rhs;
}
inline
const bigint operator/(const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) /= rhs;
}
inline
const bigint operator%(const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) %= rhs;
}
inline
const bigint operator& (const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) &= rhs;
}
inline
const bigint operator| (const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) |= rhs;
}
inline
const bigint operator^ (const bigint& lhs, const bigint& rhs)
{
	return bigint(lhs) ^= rhs;
}
inline
const bigint operator<<(const bigint& lhs, unsigned long rhs)
{
	return bigint(lhs) <<= rhs;
}
inline
const bigint operator>>(const bigint& lhs, unsigned long rhs)
{
	return bigint(lhs) >>= rhs;
}
inline
const bigint abs(const bigint& val)
{
	return val.abs();
}
inline
const bigint pow(const bigint& val, const bigint& exp)
{
	return val.pow(exp);
}
inline
const bigint n_over_k(const bigint& n, const bigint& k)
{
	return n.over(k);
}
inline
const bigint root(const bigint& val, const bigint& exp)
{
	return val.root(exp);
}
inline
const bigint root(const bigint& val, const bigint& exp, bigint& remainder)
{
	return val.root(exp, remainder);
}
inline
const bigint sqrt(const bigint& val)
{
	return val.sqrt();
}
inline
const bigint sqrt(const bigint& val, bigint& remainder)
{
	return val.sqrt(remainder);
}

template < typename Ty >
inline
bool operator==(const bigint& lhs, const Ty& rhs)
{
	return lhs == bigint(rhs);
}

template < typename Ty >
inline
bool operator==(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) == rhs;
}

template < typename Ty >
inline
bool operator!=(const bigint& lhs, const Ty& rhs)
{
	return lhs != bigint(rhs);
}

template < typename Ty >
inline
bool operator!=(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) != rhs;
}

template < typename Ty >
inline
bool operator<(const bigint& lhs, const Ty& rhs)
{
	return lhs < bigint(rhs);
}

template < typename Ty >
inline
bool operator<(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) < rhs;
}

template < typename Ty >
inline
bool operator<=(const bigint& lhs, const Ty& rhs)
{
	return lhs <= bigint(rhs);
}

template < typename Ty >
inline
bool operator<=(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) <= rhs;
}

template < typename Ty >
inline
bool operator>(const bigint& lhs, const Ty& rhs)
{
	return lhs > bigint(rhs);
}

template < typename Ty >
inline
bool operator>(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) > rhs;
}

template < typename Ty >
inline
bool operator>=(const bigint& lhs, const Ty& rhs)
{
	return lhs >= bigint(rhs);
}

template < typename Ty >
inline
bool operator>=(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) >= rhs;
}

template < typename Ty >
inline
const bigint operator+(const bigint& lhs, const Ty& rhs)
{
	return bigint(lhs) += bigint(rhs);
}

template < typename Ty >
inline
const bigint operator+(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) += rhs;
}

template < typename Ty >
inline
const bigint operator-(const bigint& lhs, const Ty& rhs)
{
	return bigint(lhs) -= bigint(rhs);
}

template < typename Ty >
inline
const bigint operator-(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) -= rhs;
}

template < typename Ty >
inline
const bigint operator*(const bigint& lhs, const Ty& rhs)
{
	return bigint(lhs) *= bigint(rhs);
}

template < typename Ty >
inline
const bigint operator*(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) *= rhs;
}

template < typename Ty >
inline
const bigint operator/(const bigint& lhs, const Ty& rhs)
{
	return bigint(lhs) /= bigint(rhs);
}

template < typename Ty >
inline
const bigint operator/(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) /= rhs;
}

template < typename Ty >
inline
const bigint operator%(const bigint& lhs, const Ty& rhs)
{
	return bigint(lhs) %= bigint(rhs);
}

template < typename Ty >
inline
const bigint operator%(const Ty& lhs, const bigint& rhs)
{
	return bigint(lhs) %= rhs;
}

#endif
