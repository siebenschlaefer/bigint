#include "bigint.h"

#include <stdexcept>
#include <sstream>

#define STRINGIFYX(x) #x
#define STRINGIFY(x) STRINGIFYX(x)
#define HERE __FILE__ ":" STRINGIFY(__LINE__)
#define DEBUG std::cerr << HERE << " "
#define PING DEBUG << std::endl;
#define PRINT(x) DEBUG << (#x) << "=" << (x) << std::endl

namespace
{

struct not_implemented
	: public std::logic_error
{
	not_implemented(const std::string& what="not implemented")
		: std::logic_error(what)
	{ }
};
#define NOT_IMPLEMENTED do { throw not_implemented(HERE); } while (false)

}

void bigint::safe_bool_converter() const
{
	NOT_IMPLEMENTED;
}

bigint::bigint()
{
	NOT_IMPLEMENTED;
}

bigint::bigint(signed int)
{
	NOT_IMPLEMENTED;
}

bigint::bigint(unsigned int)
{
	NOT_IMPLEMENTED;
}

bigint::bigint(signed long)
{
	NOT_IMPLEMENTED;
}

bigint::bigint(unsigned long)
{
	NOT_IMPLEMENTED;
}

bigint::bigint(double)
{
	NOT_IMPLEMENTED;
}

bigint::bigint(const char *, int)
{
	NOT_IMPLEMENTED;
}

bigint::bigint(const std::string&, int)
{
	NOT_IMPLEMENTED;
}

void bigint::swap(bigint&) throw()
{
	NOT_IMPLEMENTED;
}

int bigint::sign() const
{
	NOT_IMPLEMENTED;
}

const bigint bigint::abs() const
{
	NOT_IMPLEMENTED;
}

int compare(const bigint&, const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator++()
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator--()
{
	NOT_IMPLEMENTED;
}

const bigint bigint::operator-() const
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator+=(const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator-=(const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator*=(const bigint&)
{
	NOT_IMPLEMENTED;
}

void divmod(const bigint&, const bigint&, bigint *, bigint *)
{
	NOT_IMPLEMENTED;
}

void divrem(const bigint&, const bigint&, bigint *, bigint *)
{
	NOT_IMPLEMENTED;
}

const bigint bigint::operator~() const
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator&=(const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator|=(const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator^=(const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator<<=(const bigint&)
{
	NOT_IMPLEMENTED;
}

bigint& bigint::operator>>=(const bigint&)
{
	NOT_IMPLEMENTED;
}

std::ostream& operator<<(std::ostream&, const bigint&)
{
	NOT_IMPLEMENTED;
}

std::istream& operator>>(std::istream&, bigint&)
{
	NOT_IMPLEMENTED;
}

const std::string bigint::str(int) const
{
	NOT_IMPLEMENTED;
}

const bigint bigint::pow(const bigint&) const
{
	NOT_IMPLEMENTED;
}

const bigint bigint::over(const bigint&) const
{
	NOT_IMPLEMENTED;
}

std::string bigint::debug() const
{
	NOT_IMPLEMENTED;
}

const bigint gcd(const bigint&, const bigint&)
{
	NOT_IMPLEMENTED;
}

const bigint lcm(const bigint&, const bigint&)
{
	NOT_IMPLEMENTED;
}

const bigint fac(const bigint&)
{
	NOT_IMPLEMENTED;
}

const bigint bigint::root(const bigint&) const
{
	NOT_IMPLEMENTED;
}

const bigint bigint::root(const bigint&, bigint&) const
{
	NOT_IMPLEMENTED;
}

const bigint bigint::sqrt() const
{
	NOT_IMPLEMENTED;
}

const bigint bigint::sqrt(bigint&) const
{
	NOT_IMPLEMENTED;
}

std::string to_string(const bigint& bi)
{
	std::ostringstream out;
	out << bi;
	return out.str();
}

