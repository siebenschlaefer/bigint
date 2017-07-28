#include "bigint.h"
#include "bigint.h"

#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#define STRINGIFYX(x) #x
#define STRINGIFY(x) STRINGIFYX(x)
#define HERE __FILE__ ":" STRINGIFY(__LINE__)
#define DEBUG std::cerr << HERE << " "
#define PING DEBUG << std::endl;
#define PRINT(x) DEBUG << (#x) << "=" << (x) << std::endl

#define EXPECT_TRUE(cond) CPPUNIT_ASSERT((cond))
#define EXPECT_EQ(a, b) CPPUNIT_ASSERT_EQUAL((a), (b))

class BigintTest
	: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(BigintTest);

	CPPUNIT_TEST(test_ctor);
	CPPUNIT_TEST(test_ctor_char);
	CPPUNIT_TEST(test_ctor_unsigned_char);
	CPPUNIT_TEST(test_ctor_short);
	CPPUNIT_TEST(test_ctor_unsigned_short);
	CPPUNIT_TEST(test_ctor_int);
	CPPUNIT_TEST(test_ctor_unsigned_int);
	CPPUNIT_TEST(test_ctor_long);
	CPPUNIT_TEST(test_ctor_unsigned_long);
	CPPUNIT_TEST(test_ctor_double);
	CPPUNIT_TEST(test_ctor_string);
	CPPUNIT_TEST(test_ctor_cstring);
	CPPUNIT_TEST(test_ctor_bigint);

	CPPUNIT_TEST(test_assign_bigint);
	CPPUNIT_TEST(test_member_swap);

	CPPUNIT_TEST(test_str);
	CPPUNIT_TEST(test_stream_out);
	CPPUNIT_TEST(test_stream_in);

	CPPUNIT_TEST(test_bool);
	CPPUNIT_TEST(test_sign);
	CPPUNIT_TEST(test_not);
	CPPUNIT_TEST(test_member_abs);

	CPPUNIT_TEST(test_pre_increment);
	CPPUNIT_TEST(test_post_increment);
	CPPUNIT_TEST(test_pre_decrement);
	CPPUNIT_TEST(test_post_decrement);

	CPPUNIT_TEST(test_add_assign);
	CPPUNIT_TEST(test_sub_assign);
	CPPUNIT_TEST(test_mul_assign);
	CPPUNIT_TEST(test_div_assign);
	CPPUNIT_TEST(test_mod_assign);

	CPPUNIT_TEST(test_and_assign);
	CPPUNIT_TEST(test_or_assign);
	CPPUNIT_TEST(test_xor_assign);
	CPPUNIT_TEST(test_lshift_assign);
	CPPUNIT_TEST(test_rshift_assign);

	CPPUNIT_TEST(test_member_over);
	CPPUNIT_TEST(test_member_pow);
	CPPUNIT_TEST(test_member_root);
	CPPUNIT_TEST(test_member_root_remainder);
	CPPUNIT_TEST(test_member_sqrt);
	CPPUNIT_TEST(test_member_sqrt_remainder);

	CPPUNIT_TEST(test_equal);
	CPPUNIT_TEST(test_unequal);
	CPPUNIT_TEST(test_less);
	CPPUNIT_TEST(test_greater);
	CPPUNIT_TEST(test_less_or_equal);
	CPPUNIT_TEST(test_greater_or_equal);

	CPPUNIT_TEST(test_unary_plus);
	CPPUNIT_TEST(test_unary_minus);
	CPPUNIT_TEST(test_add);
	CPPUNIT_TEST(test_sub);
	CPPUNIT_TEST(test_mul);
	CPPUNIT_TEST(test_div);
	CPPUNIT_TEST(test_mod);

	CPPUNIT_TEST(test_invert);
	CPPUNIT_TEST(test_and);
	CPPUNIT_TEST(test_or);
	CPPUNIT_TEST(test_xor);
	CPPUNIT_TEST(test_lshift);
	CPPUNIT_TEST(test_rshift);

	CPPUNIT_TEST(test_swap);
	CPPUNIT_TEST(test_compare);
	CPPUNIT_TEST(test_divmod);
	CPPUNIT_TEST(test_divrem);
	CPPUNIT_TEST(test_abs);

	CPPUNIT_TEST(test_n_over_k);
	CPPUNIT_TEST(test_pow);
	CPPUNIT_TEST(test_root);
	CPPUNIT_TEST(test_root_remainder);
	CPPUNIT_TEST(test_sqrt);
	CPPUNIT_TEST(test_sqrt_remainder);

	CPPUNIT_TEST(test_gcd);
	CPPUNIT_TEST(test_lcm);
	CPPUNIT_TEST(test_fac);

	//CPPUNIT_TEST(test_equal_any_right);
	//CPPUNIT_TEST(test_equal_any_left);
	//CPPUNIT_TEST(test_unequal_any_right);
	//CPPUNIT_TEST(test_unequal_any_left);
	//CPPUNIT_TEST(test_less_any_right);
	//CPPUNIT_TEST(test_less_any_left);
	//CPPUNIT_TEST(test_less_or_equal_any_right);
	//CPPUNIT_TEST(test_less_or_equal_any_left);
	//CPPUNIT_TEST(test_greater_any_right);
	//CPPUNIT_TEST(test_greater_any_left);
	//CPPUNIT_TEST(test_greater_or_equal_any_right);
	//CPPUNIT_TEST(test_greater_or_equal_any_left);

	//CPPUNIT_TEST(test_add_any_right);
	//CPPUNIT_TEST(test_add_any_left);
	//CPPUNIT_TEST(test_sub_any_right);
	//CPPUNIT_TEST(test_sub_any_left);
	//CPPUNIT_TEST(test_mul_any_right);
	//CPPUNIT_TEST(test_mul_any_left);
	//CPPUNIT_TEST(test_div_any_right);
	//CPPUNIT_TEST(test_div_any_left);
	//CPPUNIT_TEST(test_mod_any_right);
	//CPPUNIT_TEST(test_mod_any_left);

	CPPUNIT_TEST_SUITE_END();

public:
	void test_ctor()
	{
		EXPECT_EQ(bigint(), bigint(0));
	}
	void test_ctor_char()
	{
		typedef signed char Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(127)), bigint("127"));
		EXPECT_EQ(bigint(static_cast<Ty>(-56)), bigint("-56"));
	}
	void test_ctor_unsigned_char()
	{
		typedef unsigned char Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_short()
	{
		typedef short Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_unsigned_short()
	{
		typedef unsigned short Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_int()
	{
		typedef int Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_unsigned_int()
	{
		typedef unsigned int Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_long()
	{
		typedef long Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_unsigned_long()
	{
		typedef unsigned long Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_double()
	{
		typedef double Ty;
		EXPECT_EQ(bigint(static_cast<Ty>(0)), bigint("0"));
		EXPECT_EQ(bigint(static_cast<Ty>(34)), bigint("34"));
		EXPECT_EQ(bigint(static_cast<Ty>(255)), bigint("255"));
	}
	void test_ctor_string()
	{
		EXPECT_EQ(bigint(std::string(
			    std::string("0"))), bigint(    0));
		EXPECT_EQ(bigint(std::string(
			  std::string("789"))), bigint(  789));
		EXPECT_EQ(bigint(std::string(
			"-2054")), bigint(-2054));
		EXPECT_EQ(bigint(std::string(
			"629246606250322856481242943962")).str(),
			std::string("629246606250322856481242943962"));
		EXPECT_EQ(bigint(std::string(
			"-164598829468123132721592597")).str(),
			std::string("-164598829468123132721592597"));
	}
	void test_ctor_cstring()
	{
		EXPECT_EQ(bigint(    "0"), bigint(    0));
		EXPECT_EQ(bigint(  "789"), bigint(  789));
		EXPECT_EQ(bigint("-2054"), bigint(-2054));
		EXPECT_EQ(bigint(
			"979806805671209025814425").str(), std::string(
			"979806805671209025814425"));
		EXPECT_EQ(bigint(
			"-14946245150647934779039783366").str(), std::string(
			"-14946245150647934779039783366"));
	}
	void test_ctor_bigint()
	{
		EXPECT_EQ(bigint(bigint(    0)), bigint(    0));
		EXPECT_EQ(bigint(bigint(  789)), bigint(  789));
		EXPECT_EQ(bigint(bigint(-2054)), bigint(-2054));
	}
	void test_assign_bigint()
	{
		bigint big;
		EXPECT_EQ((big =     0), bigint(    0));
		EXPECT_EQ((big =   789), bigint(  789));
		EXPECT_EQ((big = -2054), bigint(-2054));
	}
	void test_member_swap()
	{
		bigint a("622762302244849521429");
		bigint b("79234696482666744418993849");
		a.swap(b);
		EXPECT_EQ(a.str(), std::string("79234696482666744418993849"));
		EXPECT_EQ(b.str(), std::string("622762302244849521429"));
		a.swap(b);
		EXPECT_EQ(a.str(), std::string("622762302244849521429"));
		EXPECT_EQ(b.str(), std::string("79234696482666744418993849"));

		a = bigint("199825213360465201576901");
		b = bigint("-17663619017345001643");
		a.swap(b);
		EXPECT_EQ(a.str(), std::string("-17663619017345001643"));
		EXPECT_EQ(b.str(), std::string("199825213360465201576901"));
		b.swap(a);
		EXPECT_EQ(a.str(), std::string("199825213360465201576901"));
		EXPECT_EQ(b.str(), std::string("-17663619017345001643"));
	}
	void test_str()
	{
		EXPECT_EQ(bigint(0).str(), std::string("0"));
		EXPECT_EQ(bigint(1).str(), std::string("1"));
		EXPECT_EQ(bigint(-1).str(), std::string("-1"));
		EXPECT_EQ(bigint("566628460167679773101744509").str(),
			std::string("566628460167679773101744509"));
		EXPECT_EQ(bigint("-9735510015156850842820864").str(),
			std::string("-9735510015156850842820864"));

		EXPECT_EQ(bigint(0).str(2), std::string("0"));
		EXPECT_EQ(bigint(1).str(2), std::string("1"));
		EXPECT_EQ(bigint(-1).str(2), std::string("-1"));
		EXPECT_EQ(bigint("655108582497").str(2), std::string(
			"1001100010000111100000111010110001100001"));
		EXPECT_EQ(bigint("-52818805542").str(2), std::string(
			"-110001001100001111110000001100100110"));

		EXPECT_EQ(bigint(0).str(16), std::string("0"));
		EXPECT_EQ(bigint(1).str(16), std::string("1"));
		EXPECT_EQ(bigint(-1).str(16), std::string("-1"));
		EXPECT_EQ(bigint("41426178640617120746904773").str(16),
			std::string("224455a4dcf41e31ef1cc5"));
		EXPECT_EQ(bigint("-173339192475832163110").str(16),
			std::string("-96590852a970a1b26"));
	}
	void test_stream_out()
	{
		std::ostringstream out;
		out << bigint(0);
		EXPECT_EQ(std::string("0"), out.str());

		out.str("");
		out << bigint(1);
		EXPECT_EQ(std::string("1"), out.str());

		out.str("");
		out << bigint(-1);
		EXPECT_EQ(std::string("-1"), out.str());

		out.str("");
		out << bigint("9025431527864940004602540");
		EXPECT_EQ(std::string("9025431527864940004602540"),
			out.str());

		out.str("");
		out << bigint("-7579151606714069884174047679");
		EXPECT_EQ(std::string("-7579151606714069884174047679"),
			out.str());

		out.str("");
		out.setf(std::ios_base::oct, std::ios_base::basefield);
		out << bigint("8720193671706061402815049710");
		EXPECT_EQ(std::string("7026454654253735406360610727756"),
			out.str());

		out.str("");
		out.setf(std::ios_base::hex, std::ios_base::basefield);
		out << bigint("-79027921896895413180829914257");
		EXPECT_EQ(std::string("-ff5a5d6661a5f70e9931d491"),
			out.str());

		out.str("");
		out << std::setbase(8) << bigint("409307427922996753487");
		EXPECT_EQ(std::string("54301100621363701372117"),
			out.str());

		out.str("");
		out << std::setbase(16) << bigint("53205412372975608733");
		EXPECT_EQ(std::string("2e25f98a8ab62639d"), out.str());

		out.str("");
		out.setf(std::ios_base::dec, std::ios_base::basefield);
		out.setf(std::ios_base::showbase);
		out << bigint("42020325302495179214");
		EXPECT_EQ(std::string("42020325302495179214"), out.str());

		out.str("");
		out.setf(std::ios_base::oct, std::ios_base::basefield);
		out.setf(std::ios_base::showbase);
		out << bigint("578577278450842698409");
		EXPECT_EQ(std::string("076565373225046657701251"),
			out.str());

		out.str("");
		out.setf(std::ios_base::hex, std::ios_base::basefield);
		out.setf(std::ios_base::showbase);
		out << bigint("315988522812560247378");
		EXPECT_EQ(std::string("0x112138bf51af120a52"),
			out.str());

		out.str("");
		out.setf(std::ios_base::dec, std::ios_base::basefield);
		out.unsetf(std::ios_base::showbase);
		out.setf(std::ios_base::showpos);
		out << bigint("46923387673191783197036512");
		EXPECT_EQ(std::string("+46923387673191783197036512"),
			out.str());

		out.str("");
		out << bigint("-528956796008749460905221379");
		EXPECT_EQ(std::string("-528956796008749460905221379"),
			out.str());

		out.str("");
		out.unsetf(std::ios_base::showpos);
		out.setf(std::ios_base::left, std::ios_base::adjustfield);
		out.width(30);
		out << bigint("288611625469533706046625617");
		EXPECT_EQ(std::string("288611625469533706046625617   "),
			out.str());

		out.str("");
		out.setf(std::ios_base::right, std::ios_base::adjustfield);
		out.width(30);
		out << bigint("-6007685836038303431096721600");
		EXPECT_EQ(std::string(" -6007685836038303431096721600"),
			out.str());

		out.str("");
		out.setf(std::ios_base::left, std::ios_base::adjustfield);
		out << bigint("60076858360383034310967216006046");
		EXPECT_EQ(std::string("60076858360383034310967216006046"),
			out.str());
	}
	void test_stream_in()
	{
		bigint a;

		std::istringstream in("0");
		in >> a;
		EXPECT_TRUE(in.eof());
		EXPECT_EQ(a.str(), std::string("0"));

		in.clear();
		in.str("671154768127448714454");
		in >> a;
		EXPECT_TRUE(in.eof());
		EXPECT_EQ(a.str(), std::string("671154768127448714454"));

		in.clear();
		in.str("-39359329322086512900113");
		in >> a;
		EXPECT_TRUE(in.eof());
		EXPECT_EQ(a.str(), std::string("-39359329322086512900113"));
	}
	void test_bool()
	{
		EXPECT_TRUE(! bool(bigint( 0)));
		EXPECT_TRUE(bigint( 1));
		EXPECT_TRUE(bigint(-1));
		EXPECT_TRUE(bigint("99783342367434583162113"));
		EXPECT_TRUE(bigint("-67350058722207668800770"));
	}
	void test_sign()
	{
		EXPECT_EQ(bigint( 0).sign(),  0);
		EXPECT_EQ(bigint(-1).sign(), -1);
		EXPECT_EQ(bigint(+1).sign(), +1);
		EXPECT_EQ(bigint("877793631480603230948925386")
			.sign(), +1);
		EXPECT_EQ(bigint("-130065723777053202460575")
			.sign(), -1);
	}
	void test_not()
	{
		EXPECT_TRUE(!!! bigint( 0));
		EXPECT_TRUE( !! bigint( 1));
		EXPECT_TRUE( !! bigint(-1));
		EXPECT_TRUE( !! bigint("455972148391194113807"));
		EXPECT_TRUE( !! bigint("-1323615506822811567529"));
	}
	void test_member_abs()
	{
		EXPECT_EQ(bigint( 0).abs().str(), std::string("0"));
		EXPECT_EQ(bigint(-1).abs().str(), std::string("1"));
		EXPECT_EQ(bigint(+1).abs().str(), std::string("1"));
		EXPECT_EQ(bigint(
			"468267650526208884680330748").abs().str(),
			std::string("468267650526208884680330748"));
		EXPECT_EQ(bigint("-"
			"556256306301807371881562130043").abs().str(),
			std::string("556256306301807371881562130043"));
	}
	void test_pre_increment()
	{
		bigint big = 4575;
		EXPECT_EQ(++big, bigint(4576));
		EXPECT_EQ(++big, bigint(4577));
		big = -64392;
		EXPECT_EQ(++big, bigint(-64391));
		EXPECT_EQ(++big, bigint(-64390));
		big = -2;
		EXPECT_EQ(++big, bigint(-1));
		EXPECT_EQ(++big, bigint( 0));
		EXPECT_EQ(++big, bigint( 1));
		big = bigint("505343580492536637241810");
		EXPECT_EQ(++big, bigint("505343580492536637241811"));
		EXPECT_EQ(++big, bigint("505343580492536637241812"));
		big = bigint("-304183563962915012079");
		EXPECT_EQ(++big, bigint("-304183563962915012078"));
		EXPECT_EQ(++big, bigint("-304183563962915012077"));
	}
	void test_post_increment()
	{
		bigint big = 4575;
		EXPECT_EQ(big++, bigint(4575));
		EXPECT_EQ(big++, bigint(4576));
		big = -64392;
		EXPECT_EQ(big++, bigint(-64392));
		EXPECT_EQ(big++, bigint(-64391));
		big = -1;
		EXPECT_EQ(big++, bigint(-1));
		EXPECT_EQ(big++, bigint( 0));
		EXPECT_EQ(big++, bigint( 1));
		big = bigint("505343580492536637241811");
		EXPECT_EQ(big++, bigint("505343580492536637241811"));
		EXPECT_EQ(big++, bigint("505343580492536637241812"));
		big = bigint("-304183563962915012078");
		EXPECT_EQ(big++, bigint("-304183563962915012078"));
		EXPECT_EQ(big++, bigint("-304183563962915012077"));
	}
	void test_pre_decrement()
	{
		bigint big = 4575;
		EXPECT_EQ(--big, bigint(4574));
		EXPECT_EQ(--big, bigint(4573));
		big = -64392;
		EXPECT_EQ(--big, bigint(-64393));
		EXPECT_EQ(--big, bigint(-64394));
		big = 2;
		EXPECT_EQ(--big, bigint( 1));
		EXPECT_EQ(--big, bigint( 0));
		EXPECT_EQ(--big, bigint(-1));
		big = bigint("505343580492536637241810");
		EXPECT_EQ(--big, bigint("505343580492536637241809"));
		EXPECT_EQ(--big, bigint("505343580492536637241808"));
		big = bigint("-304183563962915012079");
		EXPECT_EQ(--big, bigint("-304183563962915012080"));
		EXPECT_EQ(--big, bigint("-304183563962915012081"));
	}
	void test_post_decrement()
	{
		bigint big = 4575;
		EXPECT_EQ(big--, bigint(4575));
		EXPECT_EQ(big--, bigint(4574));
		big = -64392;
		EXPECT_EQ(big--, bigint(-64392));
		EXPECT_EQ(big--, bigint(-64393));
		big = 1;
		EXPECT_EQ(big--, bigint( 1));
		EXPECT_EQ(big--, bigint( 0));
		EXPECT_EQ(big--, bigint(-1));
		big = bigint("505343580492536637241809");
		EXPECT_EQ(big--, bigint("505343580492536637241809"));
		EXPECT_EQ(big--, bigint("505343580492536637241808"));
		big = bigint("-304183563962915012080");
		EXPECT_EQ(big--, bigint("-304183563962915012080"));
		EXPECT_EQ(big--, bigint("-304183563962915012081"));
	}
	void test_add_assign()
	{
		bigint big = 0;
		EXPECT_EQ((big += 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
		EXPECT_EQ((big += 865), bigint(865));
		EXPECT_EQ(big, bigint(865));
		big = -200;
		EXPECT_EQ((big += 77), bigint(-123));
		EXPECT_EQ(big, bigint(-123));
		EXPECT_EQ((big += 865),bigint( 742));
		EXPECT_EQ(big, bigint(742));
		EXPECT_EQ((big += -945), bigint(-203));
		EXPECT_EQ(big, bigint(-203));
		EXPECT_EQ((big += -0), bigint(-203));
		EXPECT_EQ(big, bigint(-203));
		big = bigint("128772692004240068257865");
		EXPECT_EQ((big += bigint("7773102511346888737945")),
			bigint("136545794515586956995810"));
		EXPECT_EQ(big, bigint("136545794515586956995810"));
	}
	void test_sub_assign()
	{
		bigint big = 0;
		EXPECT_EQ((big -= 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
		EXPECT_EQ((big -= 865), bigint(-865));
		EXPECT_EQ(big, bigint(-865));
		big = 200;
		EXPECT_EQ((big -= 77), bigint(123));
		EXPECT_EQ(big, bigint(123));
		EXPECT_EQ((big -= 865), bigint(-742));
		EXPECT_EQ(big, bigint(-742));
		EXPECT_EQ((big -= -945), bigint(203));
		EXPECT_EQ(big, bigint(203));
		EXPECT_EQ((big -= -0), bigint(203));
		EXPECT_EQ(big, bigint(203));
		big = bigint("9214933804928146287299");
		EXPECT_EQ((big -= bigint("501665113355954500173443")),
			bigint("-492450179551026353886144"));
		EXPECT_EQ(big, bigint("-492450179551026353886144"));
	}
	void test_mul_assign()
	{
		bigint big = -1;
		EXPECT_EQ((big *= 1), bigint(-1));
		EXPECT_EQ(big, bigint(-1));
		EXPECT_EQ((big *= -1), bigint(1));
		EXPECT_EQ(big, bigint(1));
		EXPECT_EQ((big *= 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 0;
		EXPECT_EQ((big *= -1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		EXPECT_EQ((big *= 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
		EXPECT_EQ((big *= 1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 1;
		EXPECT_EQ((big *= 1), bigint(1));
		EXPECT_EQ(big, bigint(1));
		EXPECT_EQ((big *= -1), bigint(-1));
		EXPECT_EQ(big, bigint(-1));
		EXPECT_EQ((big *= 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 409;
		EXPECT_EQ((big *= 2), bigint(818));
		EXPECT_EQ(big, bigint(818));
		EXPECT_EQ((big *= -56), bigint(-45808));
		EXPECT_EQ(big, bigint(-45808));
		EXPECT_EQ((big *= -1), bigint(45808));
		EXPECT_EQ(big, bigint(45808));
		EXPECT_EQ((big *= 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = bigint("5387241703157997895");
		EXPECT_EQ((big *= bigint("4234575746049986044")),
			bigint("22812683054301876392898745291731377380"));
		EXPECT_EQ(big,
			bigint("22812683054301876392898745291731377380"));
		EXPECT_EQ((big *= -1),
			bigint("-22812683054301876392898745291731377380"));
		EXPECT_EQ(big,
			bigint("-22812683054301876392898745291731377380"));
		EXPECT_EQ((big *= 1),
			bigint("-22812683054301876392898745291731377380"));
		EXPECT_EQ(big,
			bigint("-22812683054301876392898745291731377380"));
		EXPECT_EQ((big *= 0), bigint(0));
		EXPECT_EQ(big, bigint(0));
	}
	void test_div_assign()
	{
		bigint big = 0;
		EXPECT_EQ((big /= 1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		CPPUNIT_ASSERT_THROW(big /= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(0));
		EXPECT_EQ((big /= -1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 1;
		EXPECT_EQ((big /= 1), bigint(1));
		EXPECT_EQ(big, bigint(1));
		CPPUNIT_ASSERT_THROW(big /= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(1));
		EXPECT_EQ((big /= -1), bigint(-1));
		EXPECT_EQ(big, bigint(-1));
		big = -1;
		EXPECT_EQ((big /= 1), bigint(-1));
		EXPECT_EQ(big, bigint(-1));
		CPPUNIT_ASSERT_THROW(big /= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(-1));
		EXPECT_EQ((big /= -1), bigint(1));
		EXPECT_EQ(big, bigint(1));
		big = 408;
		EXPECT_EQ((big /= 2), bigint(204));
		EXPECT_EQ(big, bigint(204));
		EXPECT_EQ((big /= -12), bigint(-17));
		EXPECT_EQ(big, bigint(-17));
		EXPECT_EQ((big /= -1), bigint(17));
		EXPECT_EQ(big, bigint(17));
		CPPUNIT_ASSERT_THROW(big /= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(17));
		big = bigint("21480779547405667111739323513579958196");
		EXPECT_EQ((big /= bigint("6864646806331026057")),
			bigint("3129189330992919828"));
		EXPECT_EQ(big, bigint("3129189330992919828"));
		EXPECT_EQ((big /= -1), bigint("-3129189330992919828"));
		EXPECT_EQ(big, bigint("-3129189330992919828"));
		EXPECT_EQ((big /= 1), bigint("-3129189330992919828"));
		EXPECT_EQ(big, bigint("-3129189330992919828"));
		CPPUNIT_ASSERT_THROW(big /= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint("-3129189330992919828"));
	}
	void test_mod_assign()
	{
		bigint big = 0;
		EXPECT_EQ((big %= 1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		CPPUNIT_ASSERT_THROW(big %= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(0));
		EXPECT_EQ((big %= -1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 1;
		CPPUNIT_ASSERT_THROW(big %= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(1));
		EXPECT_EQ((big %= 1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 1;
		EXPECT_EQ((big %= -1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = -1;
		CPPUNIT_ASSERT_THROW(big %= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(-1));
		EXPECT_EQ((big %= 1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = -1;
		EXPECT_EQ((big %= -1), bigint(0));
		EXPECT_EQ(big, bigint(0));
		big = 408;
		EXPECT_EQ((big %= 312), bigint(96));
		EXPECT_EQ(big, bigint(96));
		EXPECT_EQ((big %= -47), bigint(-45));
		EXPECT_EQ(big, bigint(-45));
		EXPECT_EQ((big %= -4), bigint(-1));
		EXPECT_EQ(big, bigint(-1));
		CPPUNIT_ASSERT_THROW(big %= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint(-1));
		big = bigint("11378054977074907067072668151");
		EXPECT_EQ((big %= bigint("2161936083805573540397466")),
			bigint("1947304089979097501202059"));
		EXPECT_EQ(big, bigint("1947304089979097501202059"));
		EXPECT_EQ((big %= bigint("-450756283772789437777")),
			bigint("-413812203125659432358"));
		EXPECT_EQ(big, bigint("-413812203125659432358"));
		CPPUNIT_ASSERT_THROW(big %= 0, std::invalid_argument);
		EXPECT_EQ(big, bigint("-413812203125659432358"));
		EXPECT_EQ((big %= 10), bigint(2));
		EXPECT_EQ(big, bigint(2));
		EXPECT_EQ((big %= 1), bigint(0));
		EXPECT_EQ(big, bigint(0));
	}
	void test_and_assign()
	{
		bigint big1(0), big2(0), big3(0);
		EXPECT_EQ((big1 &= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 0; big2 = 1; big3 = 0;
		EXPECT_EQ((big1 &= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 1; big2 = 1; big3 = 1;
		EXPECT_EQ((big1 &= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 170; big2 = 85; big3 = 0;
		EXPECT_EQ((big1 &= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 255; big2 = 85; big3 = 85;
		EXPECT_EQ((big1 &= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = std::string("779879232593610263927748006161");
		big2 = std::string("82261793876695338192268955270");
		big3 = std::string("79784684709387038826197680128");
		EXPECT_EQ((big1 &= big2), big3);
		EXPECT_EQ(big1, big3);
	}
	void test_or_assign()
	{
		bigint big1(0), big2(0), big3(0);
		EXPECT_EQ((big1 |= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 0; big2 = 1; big3 = 1;
		EXPECT_EQ((big1 |= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 1; big2 = 1; big3 = 1;
		EXPECT_EQ((big1 |= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 170; big2 = 85; big3 = 255;
		EXPECT_EQ((big1 |= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 255; big2 = 85; big3 = 255;
		EXPECT_EQ((big1 |= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = std::string("779879232593610263927748006161");
		big2 = std::string("82261793876695338192268955270");
		big3 = std::string("782356341760918563293819281303");
		EXPECT_EQ((big1 |= big2), big3);
		EXPECT_EQ(big1, big3);
	}
	void test_xor_assign()
	{
		bigint big1(0), big2(0), big3(0);
		EXPECT_EQ((big1 ^= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 0; big2 = 1; big3 = 1;
		EXPECT_EQ((big1 ^= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 1; big2 = 1; big3 = 0;
		EXPECT_EQ((big1 ^= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 170; big2 = 85; big3 = 255;
		EXPECT_EQ((big1 ^= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = 255; big2 = 85; big3 = 170;
		EXPECT_EQ((big1 ^= big2), big3);
		EXPECT_EQ(big1, big3);
		big1 = std::string("779879232593610263927748006161");
		big2 = std::string("82261793876695338192268955270");
		big3 = std::string("702571657051531524467621601175");
		EXPECT_EQ((big1 ^= big2), big3);
		EXPECT_EQ(big1, big3);
	}
	void test_lshift_assign()
	{
		bigint big1 = 0; unsigned long bits = 0; bigint big2 = 0;
		EXPECT_EQ((big1 <<= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 0; bits = 1; big2 = 0;
		EXPECT_EQ((big1 <<= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 0; bits = 1; big2 = 0;
		EXPECT_EQ((big1 <<= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 1; bits = 0; big2 = 1;
		EXPECT_EQ((big1 <<= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 1; bits = 1; big2 = 2;
		EXPECT_EQ((big1 <<= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = std::string("81026021009613569113167"); bits = 3836UL; big2 =
			"4567598054677687791254410486588333618001269610"
			"1846540403343588608238769993841059669066891263"
			"5355651034003547820659105772403075752477877140"
			"8615923186438371084580327485289907135646804469"
			"1887811067661293352062906236716889052402685523"
			"2665079170301958762184369012790055375268635767"
			"1532940317594636045977620093468512408638833703"
			"5547694110022669924603646672947791899041905876"
			"1528544254052565002146190855530343633196195101"
			"4657017460903533224112765160183977670624535814"
			"6380229701576313160888155991265349291349113706"
			"8301817472208311004729731220719914816275542079"
			"8151420506940082860623762332015674804708815520"
			"0618944201991972182829661359959615860470462114"
			"9952823010513594622509874142598867079959581808"
			"0919136621981890593771382364370761476314745055"
			"1223082011835252505070390397769488852541565602"
			"2836910838677197504118750855241435945102641545"
			"6253649586356297820725943509133871495242945042"
			"1955502543007520288561892757980514233254082127"
			"3889142549410600590847466441446453597297805720"
			"5050870240919922875976920315775572861283420268"
			"5892413581091041673071654818451446569940160222"
			"3285195065455356594452065700690101487784032498"
			"7126392258559932108455546997263689692223675780"
			"1434053123534399774142758912";
		EXPECT_EQ((big1 <<= bits), big2);
		EXPECT_EQ(big1, big2);
	}
	void test_rshift_assign()
	{
		bigint big1 = 0; unsigned long bits = 0; bigint big2 = 0;
		EXPECT_EQ((big1 >>= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 0; bits = 1; big2 = 0;
		EXPECT_EQ((big1 >>= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 0; bits = 1; big2 = 0;
		EXPECT_EQ((big1 >>= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 1; bits = 0; big2 = 1;
		EXPECT_EQ((big1 >>= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = 1; bits = 1; big2 = 0;
		EXPECT_EQ((big1 >>= bits), big2);
		EXPECT_EQ(big1, big2);
		big1 = "78064821647075408717865449773074279859072021437"
			"6090049482809413992100191717861365580131853288"
			"4157145774048445836493629636254194054714756142"
			"5947919589428633248977138188898966618111906250"
			"4014572943176089554976849324409830620543957570"
			"6671810109117465133757967890919390727228581101"
			"0137315193483525674704669544647184661325582421"
			"1765847673588853393262687272401739126166284684"
			"2239475622734162318054750899625008733115250387"
			"1154278274267141171325444556409769115849346431"
			"3823873295948371878423851380486469325729158286"
			"1467461073268303478591788291186509426861895105"
			"3218969291644486565218668134704990350510122759"
			"3979487016091306876723029921849744337469326911"
			"0014840088598190038763188749823012693005962522"
			"8003350859384055825475997887308220935726596636"
			"7138041365251048475403496783268131647064536778"
			"0848115073644007061958917767041307860083877090"
			"5089030707831727840219266379078092547785096261"
			"4375847806488465645616157755725524537871814570"
			"3919239437484111627728863346550379540934841616"
			"4282128752396100396196145480261039696912224728"
			"1092632230449581634845462578490918770403658363"
			"0567247761549191591275245445802754661194575550"
			"57136748008501958447927"; bits = 3649UL; big2 =
			"271644673684907949628844818888";
		EXPECT_EQ((big1 >>= bits), big2);
		EXPECT_EQ(big1, big2);
	}
	void test_member_over()
	{
		EXPECT_EQ(bigint(0).over(0).str(), std::string("1"));
		EXPECT_EQ(bigint(1).over(0).str(), std::string("1"));
		EXPECT_EQ(bigint(0).over(1).str(), std::string("0"));
		EXPECT_EQ(bigint(1).over(1).str(), std::string("1"));
		EXPECT_EQ(bigint(5).over(2).str(), std::string("10"));
		EXPECT_EQ(bigint(49).over(6).str(), std::string("13983816"));
	}
	void test_member_pow()
	{
		EXPECT_EQ(bigint( 0).pow( 1).str(), std::string("0"));
		EXPECT_EQ(bigint( 0).pow(10).str(), std::string("0"));
		EXPECT_EQ(bigint( 1).pow( 0).str(), std::string("1"));
		EXPECT_EQ(bigint(10).pow( 0).str(), std::string("1"));
		EXPECT_EQ(bigint(5760).pow(392).str(), std::string(
			"1217905248089782879186850878040892008875582697"
			"5159979153230910492081991725820319076853824394"
			"6116374813864229749520456545773640054911991787"
			"5794831675485691578805245939477643752855969497"
			"2589941608989827467769425606235391089872731236"
			"0616486818025508126846659926040677374156821215"
			"1371620762445834353902281032132883392391036844"
			"6144221694041150965413056927075427575221377151"
			"5362962357790251655825164060794683005443607887"
			"6224678615059488738130278529832518261117850172"
			"3968487076898596222026921076495673039968082511"
			"6186930444193009710760879539886024843288286700"
			"7042505639599079103974093873559303036794632126"
			"9251466068050170068919108212705114982512569458"
			"7083109987222138032716787293780200717423683113"
			"7660657159322787977257438366299872429685933447"
			"4021358350343636445917039305825856849507596649"
			"8047895762106974581263974414796194076244373844"
			"9271721613432483741263910448544038013734435746"
			"0879487338494333039937893340843903755032037279"
			"9246602247023009445277376142815908487045763121"
			"9486243751991413639290376994173629156413631345"
			"1792346804690955595875564880984613065017003159"
			"8953024079223136532299776000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"000"));
	}
	void test_member_root()
	{
		EXPECT_EQ(std::string("4937"),
			bigint(4937).root(1).str());
		EXPECT_EQ(std::string("0"), bigint(0).root(1).str());
		EXPECT_EQ(std::string("0"), bigint(0).root(234).str());
		EXPECT_EQ(std::string("1"), bigint(1).root(3).str());
		EXPECT_EQ(std::string("2"), bigint(8).root(3).str());
		EXPECT_EQ(std::string("3"), bigint(27).root(3).str());
		EXPECT_EQ(std::string("27731"),
			bigint("12611266865568224198312917950011")
			.root(7).str());
	}
	void test_member_root_remainder()
	{
		bigint rem;
		EXPECT_EQ(std::string("3"),
			bigint(30).root(3, rem).str());
		EXPECT_EQ(std::string("3"), rem.str());
		EXPECT_EQ(std::string("2"),
			bigint(2).root(1, rem).str());
		EXPECT_EQ(std::string("0"), rem.str());
		EXPECT_EQ(std::string("728834456213"),
			bigint("387156618662520334739439670425282130")
			.root(3, rem).str());
		EXPECT_EQ(std::string("8826533"), rem.str());
	}
	void test_member_sqrt()
	{
		EXPECT_EQ(std::string("0"), bigint(0).sqrt().str());
		EXPECT_EQ(std::string("1"), bigint(1).sqrt().str());
		EXPECT_EQ(std::string("1"), bigint(2).sqrt().str());
		EXPECT_EQ(std::string("1"), bigint(3).sqrt().str());
		EXPECT_EQ(std::string("2"), bigint(4).sqrt().str());
		EXPECT_EQ(std::string("2"), bigint(5).sqrt().str());
		EXPECT_EQ(std::string("2"), bigint(6).sqrt().str());
		EXPECT_EQ(std::string("2"), bigint(7).sqrt().str());
		EXPECT_EQ(std::string("2"), bigint(8).sqrt().str());
		EXPECT_EQ(std::string("3"), bigint(9).sqrt().str());
		EXPECT_EQ(std::string("7818630"),
			bigint("61130975076900").sqrt().str());
	}
	void test_member_sqrt_remainder()
	{
		bigint rem;
		EXPECT_EQ(bigint(30).sqrt(rem).str(), std::string("5"));
		EXPECT_EQ(rem.str(), std::string("5"));
		EXPECT_EQ(bigint("4849898269614362")
			.sqrt(rem).str(), std::string("69641211"));
		EXPECT_EQ(rem.str(), std::string("67841"));
	}
	void test_equal()
	{
		EXPECT_TRUE(bigint(    0) == bigint(    0));
		EXPECT_TRUE(bigint(23896) == bigint(23896));
		EXPECT_TRUE(bigint(-2393) == bigint(-2393));
		EXPECT_TRUE(! (bigint(    0) == bigint(    1)));
		EXPECT_TRUE(! (bigint(    1) == bigint(   -1)));
		EXPECT_TRUE(! (bigint(23896) == bigint(23895)));
		EXPECT_TRUE(! (bigint(23896) == bigint(23897)));
	}
	void test_unequal()
	{
		EXPECT_TRUE(bigint(    0) != bigint(    1));
		EXPECT_TRUE(bigint(    0) != bigint(   -1));
		EXPECT_TRUE(bigint(    1) != bigint(   -1));
		EXPECT_TRUE(bigint(23896) != bigint(23895));
		EXPECT_TRUE(bigint(23896) != bigint(23897));
		EXPECT_TRUE(! (bigint(    0) != bigint(    0)));
		EXPECT_TRUE(! (bigint(23896) != bigint(23896)));
		EXPECT_TRUE(! (bigint(-2393) != bigint(-2393)));
	}
	void test_less()
	{
		EXPECT_TRUE(bigint(0) < bigint(1));
		EXPECT_TRUE(bigint(-1) < bigint(1));
		EXPECT_TRUE(bigint(-1) < bigint(0));
		EXPECT_TRUE(bigint(5678) < bigint(5678 + 1));
		EXPECT_TRUE(bigint(-5678) < bigint(-5678 + 1));
		EXPECT_TRUE(! (bigint(0) < bigint(0)));
		EXPECT_TRUE(! (bigint(0) < bigint(-1)));
		EXPECT_TRUE(! (bigint(1) < bigint(0)));
		EXPECT_TRUE(! (bigint(1) < bigint(-1)));
		EXPECT_TRUE(! (bigint(56723) < bigint(56723)));
		EXPECT_TRUE(! (bigint(-8975) < bigint(-8975)));
	}
	void test_greater()
	{
		EXPECT_TRUE(bigint(0) > bigint(-1));
		EXPECT_TRUE(bigint(1) > bigint(0));
		EXPECT_TRUE(bigint(1) > bigint(-1));
		EXPECT_TRUE(bigint(5678 + 1) > bigint(5678));
		EXPECT_TRUE(bigint(-5678 + 1) > bigint(-5678));
		EXPECT_TRUE(! (bigint(0) > bigint(0)));
		EXPECT_TRUE(! (bigint(-1) > bigint(0)));
		EXPECT_TRUE(! (bigint(0) > bigint(1)));
		EXPECT_TRUE(! (bigint(-1) > bigint(1)));
		EXPECT_TRUE(! (bigint(56723) > bigint(56723)));
		EXPECT_TRUE(! (bigint(-8975) > bigint(-8975)));
	}
	void test_less_or_equal()
	{
		EXPECT_TRUE(bigint(0) >= bigint(-1));
		EXPECT_TRUE(bigint(0) >= bigint(0));
		EXPECT_TRUE(bigint(1) >= bigint(-1));
		EXPECT_TRUE(bigint(1) >= bigint(0));
		EXPECT_TRUE(bigint(1) >= bigint(1));
		EXPECT_TRUE(bigint(5678 + 1) >= bigint(5678));
		EXPECT_TRUE(bigint(-5678 + 1) >= bigint(-5678));
		EXPECT_TRUE(! (bigint(-1) >= bigint(0)));
		EXPECT_TRUE(! (bigint(-1) >= bigint(1)));
		EXPECT_TRUE(! (bigint(0) >= bigint(1)));
		EXPECT_TRUE(bigint(56723) >= bigint(56723));
		EXPECT_TRUE(bigint(-8975) >= bigint(-8975));
	}
	void test_greater_or_equal()
	{
		EXPECT_TRUE(bigint(0) >= bigint(0));
		EXPECT_TRUE(bigint(0) >= bigint(-1));
		EXPECT_TRUE(bigint(1) >= bigint(0));
		EXPECT_TRUE(bigint(1) >= bigint(-1));
		EXPECT_TRUE(bigint(56723) >= bigint(56723));
		EXPECT_TRUE(bigint(-8975) >= bigint(-8975));
		EXPECT_TRUE(! (bigint(0) >= bigint(1)));
		EXPECT_TRUE(! (bigint(-1) >= bigint(1)));
		EXPECT_TRUE(! (bigint(5678) >= bigint(5678 + 1)));
		EXPECT_TRUE(! (bigint(-5678) >= bigint(-5678 + 1)));
	}
	void test_unary_plus()
	{
		EXPECT_EQ(+ bigint( 0), bigint( 0));
		EXPECT_EQ(+ bigint( 1), bigint( 1));
		EXPECT_EQ(+ bigint(-1), bigint(-1));
		EXPECT_EQ(+ bigint(37592), bigint(37592));
		EXPECT_EQ(+ bigint(-4619), bigint(-4619));
	}
	void test_unary_minus()
	{
		EXPECT_EQ(- bigint( 0), bigint( 0));
		EXPECT_EQ(- bigint( 1), bigint(-1));
		EXPECT_EQ(- bigint(-1), bigint( 1));
		EXPECT_EQ(- bigint(37592), bigint(-37592));
		EXPECT_EQ(- bigint(-4619), bigint(  4619));
	}
	void test_add()
	{
		EXPECT_EQ(bigint(-1) + bigint(-1), bigint(-2));
		EXPECT_EQ(bigint(-1) + bigint(0), bigint(-1));
		EXPECT_EQ(bigint(-1) + bigint(1), bigint(0));
		EXPECT_EQ(bigint(-1) + bigint(2), bigint(1));
		EXPECT_EQ(bigint(0) + bigint(-1), bigint(-1));
		EXPECT_EQ(bigint(0) + bigint(0), bigint(0));
		EXPECT_EQ(bigint(0) + bigint(1), bigint(1));
		EXPECT_EQ(bigint(1) + bigint(-2), bigint(-1));
		EXPECT_EQ(bigint(1) + bigint(-1), bigint(0));
		EXPECT_EQ(bigint(1) + bigint(0), bigint(1));
		EXPECT_EQ(bigint(1) + bigint(1), bigint(2));
		EXPECT_EQ(bigint("8840587743209014991486176890") +
			bigint("-1"),
			bigint("8840587743209014991486176889"));
		EXPECT_EQ(bigint("499680576774082292410113726") +
			bigint("0"),
			bigint("499680576774082292410113726"));
		EXPECT_EQ(bigint("1673271581108184934182") +
			bigint("1"),
			bigint("1673271581108184934183"));
		EXPECT_EQ(bigint("8271324858169862655834") +
			bigint("74287305190053403856772"),
			bigint("82558630048223266512606"));
		EXPECT_EQ(bigint("-7096499840976817344578600") +
			bigint("13416290973509623768074"),
			bigint("-7083083550003307720810526"));
	}
	void test_sub()
	{
		EXPECT_EQ(bigint(-1) - bigint(-2), bigint(1));
		EXPECT_EQ(bigint(-1) - bigint(-1), bigint(0));
		EXPECT_EQ(bigint(-1) - bigint(0), bigint(-1));
		EXPECT_EQ(bigint(-1) - bigint(1), bigint(-2));
		EXPECT_EQ(bigint(0) - bigint(-1), bigint(1));
		EXPECT_EQ(bigint(0) - bigint(0), bigint(0));
		EXPECT_EQ(bigint(0) - bigint(1), bigint(-1));
		EXPECT_EQ(bigint(1) - bigint(-1), bigint(2));
		EXPECT_EQ(bigint(1) - bigint(0), bigint(1));
		EXPECT_EQ(bigint(1) - bigint(1), bigint(0));
		EXPECT_EQ(bigint(1) - bigint(2), bigint(-1));
		EXPECT_EQ(bigint("5375300781029317375267853") +
			bigint("-1"),
			bigint("5375300781029317375267852"));
		EXPECT_EQ(bigint("746023943964118158292") +
			bigint("0"),
			bigint("746023943964118158292"));
		EXPECT_EQ(bigint("50091918650041795129111385974") +
			bigint("1"),
			bigint("50091918650041795129111385975"));
		EXPECT_EQ(bigint("49640721124093812139957021344") +
			bigint("372389724309538480315859567"),
			bigint("50013110848403350620272880911"));
		EXPECT_EQ(bigint("54204240835403498829395228") +
			bigint("-289219492033633834288511494781"),
			bigint("-289165287792798430789682099553"));
	}
	void test_mul()
	{
		EXPECT_EQ(bigint(-1) * bigint(-1), bigint( 1));
		EXPECT_EQ(bigint(-1) * bigint( 0), bigint( 0));
		EXPECT_EQ(bigint(-1) * bigint( 1), bigint(-1));
		EXPECT_EQ(bigint( 0) * bigint(-1), bigint( 0));
		EXPECT_EQ(bigint( 0) * bigint( 0), bigint( 0));
		EXPECT_EQ(bigint( 0) * bigint( 1), bigint( 0));
		EXPECT_EQ(bigint( 1) * bigint(-1), bigint(-1));
		EXPECT_EQ(bigint( 1) * bigint( 0), bigint( 0));
		EXPECT_EQ(bigint( 1) * bigint( 1), bigint( 1));
		EXPECT_EQ(bigint(409) * bigint(2), bigint(818));
		EXPECT_EQ(bigint(818) * bigint(-56), bigint(-45808));
		EXPECT_EQ(bigint(-45808) * bigint(-1), bigint(45808));
		EXPECT_EQ(bigint(45808) * bigint(0), bigint(0));
		EXPECT_EQ(bigint("5387241703157997895") *
			bigint("4234575746049986044"),
			bigint("22812683054301876392898745291731377380"));
		EXPECT_EQ(bigint("1243478184157339114435077574") *
			bigint("-1"),
			bigint("-1243478184157339114435077574"));
		EXPECT_EQ(bigint("113110558780721284166510605813") *
			bigint("1"),
			bigint("113110558780721284166510605813"));
		EXPECT_EQ(bigint("172872415652910937156") * 0, bigint(0));
	}
	void test_div()
	{
		CPPUNIT_ASSERT_THROW(bigint(-1) / 0, std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint( 0) / 0, std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint( 1) / 0, std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint("5058481184481555253642545") / 0,
			std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint("9423865037176729110640575") / 0,
			std::invalid_argument);
		EXPECT_EQ(bigint(-1) / bigint(-1), bigint( 1));
		EXPECT_EQ(bigint(-1) / bigint( 1), bigint(-1));
		EXPECT_EQ(bigint( 0) / bigint(-1), bigint( 0));
		EXPECT_EQ(bigint( 0) / bigint( 1), bigint( 0));
		EXPECT_EQ(bigint( 1) / bigint(-1), bigint(-1));
		EXPECT_EQ(bigint( 1) / bigint( 1), bigint( 1));
		EXPECT_EQ(bigint(408) / 2, bigint(204));
		EXPECT_EQ(bigint(204) / -12, bigint(-17));
		EXPECT_EQ(bigint(-17) / -1, bigint(17));
		EXPECT_EQ(bigint("214807797252670539133590073279958196") /
			bigint("686464686331026057"),
			bigint("312918933092919828"));
		EXPECT_EQ(bigint("9675282602251649031980320382") / -1,
			bigint("-9675282602251649031980320382"));
		EXPECT_EQ(bigint("35456792613054903679731105") / 1,
			bigint("35456792613054903679731105"));
	}
	void test_mod()
	{
		CPPUNIT_ASSERT_THROW(bigint(-1) % 0, std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint( 0) % 0, std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint( 1) % 0, std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint("5058481184481555253642545") % 0,
			std::invalid_argument);
		CPPUNIT_ASSERT_THROW(bigint("9423865037176729110640575") % 0,
			std::invalid_argument);
		EXPECT_EQ(bigint(-1) % bigint(-1), bigint( 0));
		EXPECT_EQ(bigint(-1) % bigint( 1), bigint( 0));
		EXPECT_EQ(bigint( 0) % bigint(-1), bigint( 0));
		EXPECT_EQ(bigint( 0) % bigint( 1), bigint( 0));
		EXPECT_EQ(bigint( 1) % bigint(-1), bigint( 0));
		EXPECT_EQ(bigint( 1) % bigint( 1), bigint( 0));
		EXPECT_EQ((bigint(408) % 312), bigint(96));
		EXPECT_EQ((bigint(96) % -47), bigint(-45));
		EXPECT_EQ((bigint(2) % 3), bigint(2));
		EXPECT_EQ(bigint("11378054977074907067072668151") %
			bigint("2161936083805573540397466"),
			bigint("1947304089979097501202059"));
		EXPECT_EQ(bigint("1947304089979097501202059") %
			bigint("-450756283772789437777"),
			bigint("-413812203125659432358"));
		EXPECT_EQ(bigint("36944080647130005419") % 10, bigint(9));
		EXPECT_EQ(bigint("3041506782351737270624283") % 1, bigint(0));
	}
	void test_invert()
	{
		EXPECT_EQ(~bigint(0), bigint(-1));
		EXPECT_EQ(~bigint(-1), bigint(0));
		EXPECT_EQ(~bigint(1), bigint(-2));
		EXPECT_EQ(~bigint("83344562038142841171344251271"),
			bigint("-83344562038142841171344251272"));
		EXPECT_EQ(~bigint("-890902013967045386446456243"),
			bigint("890902013967045386446456242"));
	}
	void test_and()
	{
		EXPECT_EQ((bigint(0) & bigint(0)), bigint(0));
		EXPECT_EQ((bigint(0) & bigint(1)), bigint(0));
		EXPECT_EQ((bigint(1) & bigint(0)), bigint(0));
		EXPECT_EQ((bigint(1) & bigint(1)), bigint(1));
		EXPECT_EQ((bigint("69087540840244653375657") &
			bigint("16212588019390587225031990")),
			bigint("38372117049930888560672"));
	}
	void test_or()
	{
		EXPECT_EQ((bigint(0) | bigint(0)), bigint(0));
		EXPECT_EQ((bigint(0) | bigint(1)), bigint(1));
		EXPECT_EQ((bigint(1) | bigint(0)), bigint(1));
		EXPECT_EQ((bigint(1) | bigint(1)), bigint(1));
		EXPECT_EQ((bigint("35682696283519908640018990321") |
			bigint("51382098653141249056266")),
			bigint("35682706325798355339853782779"));
	}
	void test_xor()
	{
		EXPECT_EQ((bigint(0) ^ bigint(0)), bigint(0));
		EXPECT_EQ((bigint(0) ^ bigint(1)), bigint(1));
		EXPECT_EQ((bigint(1) ^ bigint(0)), bigint(1));
		EXPECT_EQ((bigint(1) ^ bigint(1)), bigint(0));
		EXPECT_EQ((bigint("92590244708704389593884272") ^
			bigint("190530101625321512699891183")),
			bigint("252726568316860872329259935"));
	}
	void test_lshift()
	{
		EXPECT_EQ((bigint(0) << 0), bigint(0));
		EXPECT_EQ((bigint(0) << 1), bigint(0));
		EXPECT_EQ((bigint(1) << 0), bigint(1));
		EXPECT_EQ((bigint(1) << 1), bigint(2));
		EXPECT_EQ((bigint("32054923945035194795485") <<
			8789UL), bigint("1813537922814740447209368049"
			"2009299170755814206412306868343914300984553889"
			"1040816571447768300183914511442436018640831880"
			"4142393170704033111075782743483998759829751871"
			"8022764212948287154015532395197112701480558418"
			"4435658738458571544538404694177370664637351669"
			"6531284907704826558869193988460705001626549405"
			"9119944844575119791167912905242253248422530067"
			"5965370306925810574566256742619410386248048195"
			"3364113960499630828785138314719290424763844240"
			"4568466250016500171557677780689233748958100336"
			"7926359575172042760530675333395413401927180434"
			"4385664666394871435182859443767789484741159374"
			"5044617437930387957248177640973237612506426549"
			"0529190456983525596241937852670683496655970294"
			"0178352567596966156004010345326591868805308066"
			"3691436975104041014418872724043450738973445928"
			"2900298438851709528593686965245809755274189923"
			"7079204991071291684580093614022593982874656306"
			"9365035179958466588259498065392346472978720447"
			"0951065478655829466346198625181832869090167894"
			"0863314248615112541753428077869951796556111524"
			"9543726390724340501913388746761254927520755685"
			"9697613760290049424227594273995288778728452470"
			"0217111831338985460885691277358307067830433973"
			"8740469744784360979374469323177580656160110937"
			"6745370128448631562565843984782960477346655720"
			"3917477029925036695472664770498914933591370483"
			"8074006253550263097734689692982547367471301300"
			"2236635196839065108977373164151475824447694345"
			"2059915932778071049146608125459987377883808205"
			"0313729525150063841642665179586441971198205728"
			"0855504744255552060400093040972188453717151281"
			"7308014423973355999929935741705090191625585759"
			"7182719607804258354090706369457320961972156277"
			"6922247170394518627479851948620333280133988787"
			"5301088262824153284253731693104694290645370196"
			"0844387081248556875303073384925494140912578170"
			"9079664724397669552044688665665030813819675036"
			"2793914690831301442294503346563089808208136340"
			"7403544946356358730180729755834105926422463032"
			"1251641113700477581519734688916757615966948336"
			"3748232918844356837743081326078193325076902815"
			"9840936140316212718568449724884823803130664606"
			"7032079750073429699723802467560516244443122029"
			"3073648273879816212211431711682250383032102551"
			"2807124917634403393337877848898754418481135358"
			"0415587817099085171748095427659730054853474433"
			"4992258156499279427636400485021565900701619665"
			"0017035627741554887782490833480756579458809287"
			"8700706568892985602342413872102594691012781984"
			"7001783441750106953330551622210617685910291459"
			"9525760999697913209832237598891850489017163953"
			"0487714990169735117497063833260311187738819368"
			"1599336377047117925182983268669165705584274692"
			"0631203700240573072920333011951276389850081114"
			"2984687058150979611158527839298273197441364908"
			"7576586712393732686269824983357128823843822514"
			"4710957902405304320"));
	}
	void test_rshift()
	{
		EXPECT_EQ((bigint(0) >> 0), bigint(0));
		EXPECT_EQ((bigint(0) >> 1), bigint(0));
		EXPECT_EQ((bigint(1) >> 0), bigint(1));
		EXPECT_EQ((bigint(1) >> 1), bigint(0));
		EXPECT_EQ((bigint("7846755459404429516532876215374"
			"0590078969735738809383249631387750117346336503"
			"8997238464003150402902582005559150322192643694"
			"6540722491435088991101252500974534739076271548"
			"8399073856795387036599755944613514620648139670"
			"1646168801074436377785842234764056367888345778"
			"4461969228403718552463792757977456555723963951"
			"6227331777513054785325781744772035173942128592"
			"9340446470045956100848838362166943461415524714"
			"4800647000216583945552676194915117688787036030"
			"6686130170017328032642959748591382761475878800"
			"0688624807860978311724896802737382723155275788"
			"2740526338281040392528389108164668147641420982"
			"7453003654406213425953926877985617717697448884"
			"4653632082571800016358521006933152916967132649"
			"9507779365989641901939829186360300270899793020"
			"2511433381415576263901362283907612156529012637"
			"8570487994636541741509488006163973711199395353"
			"8685897560229387651366587047249751582422405352"
			"3203873549781573879914284161482887148458198067"
			"0273535925888247551400972740043776640834014599"
			"0606367459248117180678538032618355401019843342"
			"6914343066713461540645712152298081291887356534"
			"2928828037308081439898304271386027335546800595"
			"4648741147488482577955832734375844964946049498"
			"5628402317141927201964275007356141277659072496"
			"2547662644864637124302405274515929742429205641"
			"9056537632111793253063988248796977636141791158"
			"4119074532529812336553168939179258242030514060"
			"7697722275239051661076134318464043679860698276"
			"7213804616092677071009000633136636923369820616"
			"2796601737739338976681703851941451481657984652"
			"9436974656365357881275888715279684943672975277"
			"8203443033412774102982165088628537218896688073"
			"0890437907101370838361988247123961312360916152"
			"9102083887489967858667577436477884731162993820"
			"6673017015700309908929276134833019875077393335"
			"9663598983108130046823716228873741184320281557"
			"3664123976443148559285277950543951295006946544"
			"7913560801864271456782827053169539901269541916"
			"2347147431846519284476657637511006383780967267"
			"6254612336030554414291555809277058452831902488"
			"3919524678440602248217642648979725306074052281"
			"6753853722434130179670899967319278529738879411"
			"3040730789556304096618792443617212404250186932"
			"9357836987207606195779509779264474914035039671"
			"2010821468879163707337348918614110926619561733"
			"8245899760356654007808047560934476143699712879"
			"5493991820532135081537759305005099934985663021"
			"6745410816949809496262841853215935915510340733"
			"3782798486143461514164386362972854099713604524"
			"0229874653262447910722778513186234168292068086"
			"4133597067601129892752546554242595575258734918"
			"0707533937395609799865052514009768584443554519"
			"1601240833871525122390519344545811645173195408"
			"0283198975341736995974326320176151588864106801"
			"9566798746539300786332157353746092766819669408"
			"4418034669115151") >> 8614),
			bigint("664213148845147145845965659019"));
	}
	void test_swap()
	{
		bigint a("622762302244849521429");
		bigint b("79234696482666744418993849");
		std::swap(a, b);
		EXPECT_EQ(a.str(), std::string("79234696482666744418993849"));
		EXPECT_EQ(b.str(), std::string("622762302244849521429"));
		std::swap(a, b);
		EXPECT_EQ(a.str(), std::string("622762302244849521429"));
		EXPECT_EQ(b.str(), std::string("79234696482666744418993849"));

		a = bigint("199825213360465201576901");
		b = bigint("-17663619017345001643");
		std::swap(a, b);
		EXPECT_EQ(a.str(), std::string("-17663619017345001643"));
		EXPECT_EQ(b.str(), std::string("199825213360465201576901"));
		std::swap(b, a);
		EXPECT_EQ(a.str(), std::string("199825213360465201576901"));
		EXPECT_EQ(b.str(), std::string("-17663619017345001643"));
	}
	void test_compare()
	{
		EXPECT_EQ(compare(bigint(0), bigint(0)), 0);
		EXPECT_TRUE(compare(bigint(0), bigint(1))  < 0);
		EXPECT_TRUE(compare(bigint(1), bigint(0))  > 0);
		EXPECT_TRUE(compare(bigint(-1), bigint(1)) < 0);
		EXPECT_TRUE(compare(bigint(1), bigint(-1)) > 0);
		EXPECT_EQ(compare(bigint(67823), bigint(67823)), 0);
		EXPECT_TRUE(compare(bigint(-67823), bigint(67823)) < 0);
		EXPECT_TRUE(compare(bigint(67823), bigint(-67823)) > 0);
		EXPECT_TRUE(compare(bigint(387), bigint(388)) < 0);
		EXPECT_TRUE(compare(bigint(387), bigint(386)) > 0);
	}
	void test_divmod()
	{
		bigint a, b;
		divmod(bigint(0), bigint(1), &a, &b);
		EXPECT_EQ(a.str(), std::string("0"));
		EXPECT_EQ(b.str(), std::string("0"));
		divmod(bigint(1), bigint(1), &a, &b);
		EXPECT_EQ(a.str(), std::string("1"));
		EXPECT_EQ(b.str(), std::string("0"));
		divmod(bigint(-1), bigint(1), &a, &b);
		EXPECT_EQ(a.str(), std::string("-1"));
		EXPECT_EQ(b.str(), std::string("0"));
		divmod(bigint(1), bigint(-1), &a, &b);
		EXPECT_EQ(a.str(), std::string("-1"));
		EXPECT_EQ(b.str(), std::string("0"));
		divmod(bigint("63209104082147922191195786"),
			bigint("15809956"), &a, &b);
		EXPECT_EQ(a.str(), std::string("3998056925784481765"));
		EXPECT_EQ(b.str(), std::string("3743446"));
		divmod(bigint("-152466211251883952931"),
			bigint("848247019"), &a, &b);
		EXPECT_EQ(a.str(), std::string("-179742702111"));
		EXPECT_EQ(b.str(), std::string("776804178"));
	}
	void test_divrem()
	{
		bigint a, b;
		divrem(bigint(0), bigint(1), &a, &b);
		EXPECT_EQ(a.str(), std::string("0"));
		EXPECT_EQ(b.str(), std::string("0"));
		divrem(bigint(1), bigint(1), &a, &b);
		EXPECT_EQ(a.str(), std::string("1"));
		EXPECT_EQ(b.str(), std::string("0"));
		divrem(bigint(-1), bigint(1), &a, &b);
		EXPECT_EQ(a.str(), std::string("-1"));
		EXPECT_EQ(b.str(), std::string("0"));
		divrem(bigint(1), bigint(-1), &a, &b);
		EXPECT_EQ(a.str(), std::string("-1"));
		EXPECT_EQ(b.str(), std::string("0"));
		divrem(bigint("63209104082147922191195786"),
			bigint("15809956"), &a, &b);
		EXPECT_EQ(a.str(), std::string("3998056925784481765"));
		EXPECT_EQ(b.str(), std::string("3743446"));
		divrem(bigint("-152466211251883952931"),
			bigint("848247019"), &a, &b);
		EXPECT_EQ(a.str(), std::string("-179742702110"));
		EXPECT_EQ(b.str(), std::string("-71442841"));
	}
	void test_abs()
	{
		EXPECT_EQ(abs(bigint(-1)), bigint(1));
		EXPECT_EQ(abs(bigint( 0)), bigint(0));
		EXPECT_EQ(abs(bigint( 1)), bigint(1));
		EXPECT_EQ(abs(bigint("-129149048650216065148")),
			bigint("129149048650216065148"));
		EXPECT_EQ(abs(bigint("25616208539773509649725977")),
			bigint("25616208539773509649725977"));
	}
	void test_n_over_k()
	{
		EXPECT_EQ(n_over_k(bigint(0), 0), bigint(1));
		EXPECT_EQ(n_over_k(bigint(0), 1), bigint(0));
		EXPECT_EQ(n_over_k(bigint(1), 0), bigint(1));
		EXPECT_EQ(n_over_k(bigint(1), 1), bigint(1));
		EXPECT_EQ(n_over_k(bigint(5), 2), bigint(10));
		EXPECT_EQ(n_over_k(bigint(49), 6),
			bigint("13983816"));
	}
	void test_pow()
	{
		EXPECT_EQ(pow(bigint(-1), 0), bigint(1));
		EXPECT_EQ(pow(bigint(-1), 1), bigint(-1));
		EXPECT_EQ(pow(bigint(-1), 2), bigint(1));
		EXPECT_EQ(pow(bigint(0), 0), bigint(1));
		EXPECT_EQ(pow(bigint(0), 1), bigint(0));
		EXPECT_EQ(pow(bigint(0), 2), bigint(0));
		EXPECT_EQ(pow(bigint(1), 0), bigint(1));
		EXPECT_EQ(pow(bigint(1), 1), bigint(1));
		EXPECT_EQ(pow(bigint(1), 2), bigint(1));
		EXPECT_EQ(pow(bigint(2), 0), bigint(1));
		EXPECT_EQ(pow(bigint(2), 1), bigint(2));
		EXPECT_EQ(pow(bigint(2), 2), bigint(4));
		EXPECT_EQ(pow(bigint("403"), 272), bigint(
			"4395138350520306423605074546745167965968738981"
			"3319441416637626781334583200072635892772029811"
			"3563140105820657192577331144336961008517650963"
			"4798288218151060211042669699797254745587150618"
			"4518283884931979834374146095496537648601364542"
			"8087976213732239113890253216442561117560122503"
			"9341036570017205570050740663327881165490843281"
			"7293284004762410355083973970682472675937084033"
			"0601821254143258467823715152546325649689942907"
			"6865467365398675347610255231529358435559198322"
			"2776101459372750330393958331845504262762488462"
			"6652355938893843096407365655891654451445244017"
			"2852930044550707473780427931512606348247129633"
			"6677212431981118530635342876576386764813933018"
			"2318571860621469685677644098741567338600808128"
			"8086768630108530241"));
	}
	void test_root()
	{
		EXPECT_EQ(root(bigint(0), 1), bigint(0));
		EXPECT_EQ(root(bigint(1), 1), bigint(1));
		EXPECT_EQ(root(bigint(30), 3), bigint(3));
		EXPECT_EQ(root(bigint("120957088895388595981044060"
			"1474220002582163397089582222736674047720198599"
			"9013695252713586523435934007146630851189957398"
			"2289251743071870869204656737324424412575580947"
			"2794694425141028280948850082517081945191856827"
			"7410718682859651905227590722809829658144399809"
			"4300625333605867808936686826023817476961697440"
			"3770123665393620379451424643318432228105467290"
			"1449824934002657261265849921817779622252694028"
			"8340254117746542305245506508566290824154595212"
			"6178536816866921314084371791325887746321329262"
			"4899163825357274552751783539075573123437268415"
			"4088292095150080945806460123054351911809570086"
			"9332937986243036366873950046689615417303615715"
			"4782219314411539799607122598768015004113048155"
			"0861504759385135512172638740918506531091446744"
			"7582425303718693493045449172831086013487236810"
			"7433884542107962231722238423956604128391397249"
			"9591057107046831692843515073914116363245076755"
			"9921951340882226513607303870737336398068292244"
			"4571210930576536967681259734852698953966322117"
			"3261296882539148389707732460245077811363983674"
			"2929749502027984882079802422386192100687712583"
			"2800505534790764810857278186915494114900867866"
			"2738524498261477655557220670304842689268688982"
			"2144772742185411591266339740623350322019754359"
			"9226189093167822471460620903125136914662401778"
			"6575536475049740249910751590125200145329101653"
			"5297318103264030012168700670034079488289"),
			139), bigint("2574343009"));
	}
	void test_root_remainder()
	{
		bigint rem;
		EXPECT_EQ(root(bigint(0), 5, rem), bigint(0));
		EXPECT_EQ(rem, bigint(0));
		EXPECT_EQ(root(bigint(1), 3, rem), bigint(1));
		EXPECT_EQ(rem, bigint(0));
		EXPECT_EQ(root(bigint(30), 3, rem), bigint(3));
		EXPECT_EQ(rem, bigint(3));
		EXPECT_EQ(root(bigint("326283409086796741252264610"
			"9176666812068904834659730158634564354653800498"
			"8198484321056571025841249909544267964815815964"
			"8036920463189702194247181184576402337043683709"
			"4457000526604386271965880772803040590223504286"
			"3305944636371617987005604104852735137545342407"
			"8677118100014395275554782852119861720693827059"
			"2600215854993503825835849420157380841638362834"
			"3279898306528566677686871766862158332077134482"
			"1226235484833903799700808915567432479115083356"
			"8979281744901740134705175138985761674960770976"
			"3328"), 56, rem), bigint("574343009"));
		EXPECT_EQ(rem.str(), std::string(
			"1631091892760822239473942149312018489310561883"
			"2576734766730850982804315484539556837887"));
	}
	void test_sqrt()
	{
		EXPECT_EQ(sqrt(bigint(0)), bigint(0));
		EXPECT_EQ(sqrt(bigint(1)), bigint(1));
		EXPECT_EQ(sqrt(bigint(4)), bigint(2));
		EXPECT_EQ(sqrt(bigint("861300612541788175667593706"
			"5058312796200628401583662517936002590759006455"
			"0421101910206128874170997281883060271913938131"
			"2183013636558284887817057704226430548370392377"
			"5096868703179775626296389065369515342838784865"
			"4273947443899013809845843537872932239967031502"
			"5760486553058411708629489019175783975682263853"
			"3802622988270979892916566575788155471631091892"
			"7608222394739421493120184893105618832576539818"
			"2174765002273359326920347257434300966714691228"
			"0627307396980075648105787881384133409941166665"
			"8599933734766730850982804315484539556837005990"
			"5247285402878162088597398585134328730698566470"
			"0123035904")), bigint("293479234792137921654"
			"9217307259364901830728957393081081947239874103"
			"8921047923864892373021983021849236598172308213"
			"0921047238956923873210983210839328479237492374"
			"9237587610938014694385710391284923653271283587"
			"1973753628217309172387523487319548237143619873"
			"21973823436892317012738237647836739178921648"));
		EXPECT_EQ(sqrt(bigint("861300612541788175667593706"
			"5058312796200628401583662517936002590759006455"
			"0421101910206128874170997281883060271913938131"
			"2183013636558284887817057704226430548370392377"
			"5096868703179775626296389065369515342838784865"
			"4273947443899013809845843537872932239967031502"
			"5760486553058411708629489019175783975682263853"
			"3802622988270979892916566575788155471631091892"
			"7608222394739421493120184893105618832576539818"
			"2174765002273359326920347257434300966714691228"
			"0627307396980075648105787881384133409941166665"
			"8599933734766730850982804315484539556837005990"
			"5247285402878162088597398585134328730698566470"
			"0123035903")), bigint("293479234792137921654"
			"9217307259364901830728957393081081947239874103"
			"8921047923864892373021983021849236598172308213"
			"0921047238956923873210983210839328479237492374"
			"9237587610938014694385710391284923653271283587"
			"1973753628217309172387523487319548237143619873"
			"21973823436892317012738237647836739178921647"));
	}
	void test_sqrt_remainder()
	{
		bigint rem;
		EXPECT_EQ(sqrt(bigint(0), rem), bigint(0));
		EXPECT_EQ(rem, bigint(0));
		EXPECT_EQ(sqrt(bigint(1), rem), bigint(1));
		EXPECT_EQ(rem, bigint(0));
		EXPECT_EQ(sqrt(bigint(6), rem), bigint(2));
		EXPECT_EQ(rem, bigint(2));
		EXPECT_EQ(sqrt(bigint("861300612541788175667593706"
			"5058312796200628401583662517936002590759006455"
			"0421101910206128874170997281883060271913938131"
			"2183013636558284887817057704226430548370392377"
			"5096868703179775626296389065369515342838784865"
			"4273947443899013809845843537872932239967031502"
			"5760486553058411708629489019175783975682263853"
			"3802622988270979892916566575788155471631091892"
			"7608222394739421493120184893105618832576539818"
			"2174765002273359326920347257434300966714691228"
			"0627307396980075648105787881384133409941166665"
			"8599933734766730850982804315484539556837005990"
			"5247285402878162088597398585134328730698566470"
			"0123035904"), rem), bigint("2934792347921379"
			"2165492173072593649018307289573930810819472398"
			"7410389210479238648923730219830218492365981723"
			"0821309210472389569238732109832108393284792374"
			"9237492375876109380146943857103912849236532712"
			"8358719737536282173091723875234873195482371436"
			"1987321973823436892317012738237647836739178921"
			"648"));
		EXPECT_EQ(rem, bigint(0));
		EXPECT_EQ(sqrt(bigint("861300612541788175667593706"
			"5058312796200628401583662517936002590759006455"
			"0421101910206128874170997281883060271913938131"
			"2183013636558284887817057704226430548370392377"
			"5096868703179775626296389065369515342838784865"
			"4273947443899013809845843537872932239967031502"
			"5760486553058411708629489019175783975682263853"
			"3802622988270979892916566575788155471631091892"
			"7608222394739421493120184893105618832576539818"
			"2174765002273359326920347257434300966714691228"
			"0627307396980075648105787881384133409941166665"
			"8599933734766730850982804315484539556837005990"
			"5247285402878162088597398585134328730698566470"
			"0123035903"), rem), bigint("2934792347921379"
			"2165492173072593649018307289573930810819472398"
			"7410389210479238648923730219830218492365981723"
			"0821309210472389569238732109832108393284792374"
			"9237492375876109380146943857103912849236532712"
			"8358719737536282173091723875234873195482371436"
			"1987321973823436892317012738237647836739178921"
			"647"));
		EXPECT_EQ(rem, bigint("5869584695842758433098434"
			"6145187298036614579147861621638944797482077842"
			"0958477297847460439660436984731963446164261842"
			"0944779138477464219664216786569584749847498475"
			"1752218760293887714207825698473065425671743947"
			"5072564346183447750469746390964742872397464394"
			"7646873784634025476475295673478357843294"));
	}
	void test_gcd()
	{
		EXPECT_EQ(gcd(bigint(1), bigint(0)), bigint(1));
		EXPECT_EQ(gcd(bigint(1), bigint(1)), bigint(1));
		EXPECT_EQ(gcd(bigint(210), bigint(330)),
			bigint(30));
	}
	void test_lcm()
	{
		EXPECT_EQ(lcm(bigint(0), bigint(0)), bigint(0));
		EXPECT_EQ(lcm(bigint(1), bigint(0)), bigint(0));
		EXPECT_EQ(lcm(bigint(1), bigint(1)), bigint(1));
		EXPECT_EQ(lcm(bigint(2), bigint(3)), bigint(6));
		EXPECT_EQ(lcm(bigint(210), bigint(330)),
			bigint(2310));
	}
	void test_fac()
	{
		EXPECT_EQ(fac(0), bigint(1));
		EXPECT_EQ(fac(1), bigint(1));
		EXPECT_EQ(fac(2), bigint(2));
		EXPECT_EQ(fac(3), bigint(6));
		EXPECT_EQ(fac(4), bigint(24));
		EXPECT_EQ(fac(5), bigint(120));
		EXPECT_EQ(fac(100), bigint("93326215443944152681"
			"6992388562667004907159682643816214685929638952"
			"1759999322991560894146397615651828625369792082"
			"7223758251185210916864000000000000000000000000"
			));
		EXPECT_EQ(fac(999), bigint("40238726007709377354"
			"3702433923003985719374864210714632543799910429"
			"9385123986290205920442084869694048004799886101"
			"9719605863166687299480855890132382966994459099"
			"7424504087073759918823627727188732519779505950"
			"9952761208749754624970436014182780946464962910"
			"5639388743788648733711918104582578364784997701"
			"2476632889835955735432513185323958463075557409"
			"1142624174743493475534286465766116677973966688"
			"2029120737914385371958824980812686783837455973"
			"1746136085379534524221586593201928090878297308"
			"4313928444032812315586110369768013573042161687"
			"4760967587134831202547858932076716913244842623"
			"6131412508780208000261683151027341827977704784"
			"6358681701643650241536913982812648102130927612"
			"4489635992870511496497541990934222156683257208"
			"0821333186116811553615836546984046708975602900"
			"9505376164758477284218896796462449451607653534"
			"0819890138544248798495995331910172335555660213"
			"9450399736280750137837615307127761926849034352"
			"6252000158885351473316117021039681759215109077"
			"8801939317811419454525722386554146106289218796"
			"0223838971476088506276862967146674697562911234"
			"0824392081601537808898939645182632436716167621"
			"7916890977991190375403127462228998800519544441"
			"4282012187361745992642956581746628302955570299"
			"0243241531816172104658320367869061172601587835"
			"2075151628422554026517048330422614397428693306"
			"1690897968482590125458327168226458066526769958"
			"6526822728070757813918581788896522081643483448"
			"2599326604336766017699961283186078838615027946"
			"5955131156552036093988180612138558600301435694"
			"5272242063446317974605946825731037900840244324"
			"3846565724501440282188525247093519062092902313"
			"6493273497565513958720559654228749774011413346"
			"9627154228458623773875382304838656889764619273"
			"8381490014076731044664025989949022222176590433"
			"9901886018566526485061799702356193897017860040"
			"8118897299183110211712298459016419210688843871"
			"2185564612496079872290851929681937238864261483"
			"9657382291123125024186649353143970137428531926"
			"6498753372189406942814341185201580141233448280"
			"1505139969429015348307764456909907315243327828"
			"8269864602789864321139083506217095002597389863"
			"5542771967428222487575867657523442202075736305"
			"6949882508796892816275384886339690995982628095"
			"6121450994871701244516461260379029309120889086"
			"9420285106401821543994571568059418727489980942"
			"5474217358240106367740459574178516082923013535"
			"8081840096996372524230560855903700624271243416"
			"9090041536901059339838357779394109700277534720"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000000000000000000"
			"000000000000000"));

	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(BigintTest);

