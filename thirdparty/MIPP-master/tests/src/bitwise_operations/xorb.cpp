#include <exception>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <mipp.h>
#include <catch.hpp>

template <typename T>
void test_reg_xorb()
{

	T inputs1[mipp::N<T>()], inputs2[mipp::N<T>()];
	std::iota(inputs1, inputs1 + mipp::N<T>(), (T)0);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)0);

	std::mt19937 g;
	std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
	std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

	mipp::reg r1 = mipp::load<T>(inputs1);
	mipp::reg r2 = mipp::load<T>(inputs2);
	mipp::reg r3 = mipp::xorb<T>(r1, r2);

	for (auto i = 0; i < mipp::N<T>(); i++)
	{
		T res = inputs1[i] ^ inputs2[i];
		REQUIRE(mipp::get<T>(r3, i) == res);
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Binary xor - mipp::reg", "[mipp::xorb]")
{
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_reg_xorb<int64_t>(); }
#endif
	SECTION("datatype = int32_t") { test_reg_xorb<int32_t>(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { test_reg_xorb<int16_t>(); }
	SECTION("datatype = int8_t") { test_reg_xorb<int8_t>(); }
#endif
}
#endif

template <typename T>
void test_Reg_xorb()
{
	T inputs1[mipp::N<T>()], inputs2[mipp::N<T>()];
	std::iota(inputs1, inputs1 + mipp::N<T>(), (T)0);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)0);

	std::mt19937 g;
	std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
	std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

	mipp::Reg<T> r1 = inputs1;
	mipp::Reg<T> r2 = inputs2;
	mipp::Reg<T> r3 = r1 ^ r2;

	for (auto i = 0; i < mipp::N<T>(); i++)
	{
		T res = inputs1[i] ^ inputs2[i];
		REQUIRE(r3[i] == res);
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Binary xor - mipp::Reg", "[mipp::xorb]")
{
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_Reg_xorb<int64_t>(); }
#endif
	SECTION("datatype = int32_t") { test_Reg_xorb<int32_t>(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { test_Reg_xorb<int16_t>(); }
	SECTION("datatype = int8_t") { test_Reg_xorb<int8_t>(); }
#endif
}
#endif

template <typename T>
void test_msk_xorb()
{
	constexpr int N = mipp::N<T>();
	bool inputs1[N], inputs2[N];
	std::mt19937 g;
	std::uniform_int_distribution<uint16_t> dis(0, 1);

	for (auto t = 0; t < 100; t++)
	{
		for (auto i = 0; i < N; i++)
		{
			inputs1[i] = dis(g) ? true : false;
			inputs2[i] = dis(g) ? true : false;
		}

		std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
		std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

		mipp::msk m1 = mipp::set<N>(inputs1);
		mipp::msk m2 = mipp::set<N>(inputs2);
		mipp::msk m3 = mipp::xorb<N>(m1, m2);

		mipp::reg r = mipp::toreg<N>(m3);

		for (auto i = 0; i < N; i++)
		{
			bool res = inputs1[i] ^ inputs2[i];

			if (res)
				REQUIRE(mipp::get<T>(r, i) != (T)0);
			else
				REQUIRE(mipp::get<T>(r, i) == (T)res);
		}
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Binary xor - mipp::msk", "[mipp::xorb]")
{
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_msk_xorb<int64_t>(); }
#endif
	SECTION("datatype = int32_t") { test_msk_xorb<int32_t>(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { test_msk_xorb<int16_t>(); }
	SECTION("datatype = int8_t") { test_msk_xorb<int8_t>(); }
#endif
}
#endif

template <typename T>
void test_Msk_xorb()
{
	constexpr int N = mipp::N<T>();
	bool inputs1[N], inputs2[N];
	std::mt19937 g;
	std::uniform_int_distribution<uint16_t> dis(0, 1);

	for (auto t = 0; t < 100; t++)
	{
		for (auto i = 0; i < N; i++)
		{
			inputs1[i] = dis(g) ? true : false;
			inputs2[i] = dis(g) ? true : false;
		}

		std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
		std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

		mipp::Msk<N> m1 = inputs1;
		mipp::Msk<N> m2 = inputs2;
		mipp::Msk<N> m3 = m1 ^ m2;

		for (auto i = 0; i < N; i++)
		{
			bool res = inputs1[i] ^ inputs2[i];
			REQUIRE(m3[i] == res);
		}
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Binary xor - mipp::Msk", "[mipp::xorb]")
{
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_Msk_xorb<int64_t>(); }
#endif
	SECTION("datatype = int32_t") { test_Msk_xorb<int32_t>(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { test_Msk_xorb<int16_t>(); }
	SECTION("datatype = int8_t") { test_Msk_xorb<int8_t>(); }
#endif
}
#endif

