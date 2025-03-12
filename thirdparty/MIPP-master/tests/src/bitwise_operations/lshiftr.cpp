#include <exception>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <mipp.h>
#include <catch.hpp>

template <typename T>
void test_reg_lshiftr()
{
	T inputs1[mipp::N<T>()];
	T inputs2[mipp::N<T>()];
	std::iota(inputs1, inputs1 + mipp::N<T>(), (T)0);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)0);

	std::mt19937 g;
	std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
	std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

	mipp::reg r1 = mipp::load<T>(inputs1);
	mipp::reg r2 = mipp::load<T>(inputs2);
	mipp::reg r3 = mipp::lshiftr<T>(r1, r2);

	for (auto i = 0; i < mipp::N<T>(); i++)
	{
		T res = inputs1[i] << inputs2[i];
		REQUIRE(mipp::get<T>(r3, i) == res);
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Binary left shift (register) - mipp::reg", "[mipp::lshiftr]")
{
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_reg_lshiftr<int64_t>(); }
#endif
	SECTION("datatype = int32_t") { test_reg_lshiftr<int32_t>(); }
#if defined(MIPP_BW)
#if !defined(MIPP_AVX)
	SECTION("datatype = int16_t") { test_reg_lshiftr<int16_t>(); }
#if !defined(MIPP_AVX512)
	SECTION("datatype = int8_t") { test_reg_lshiftr<int8_t>(); }
#endif
#endif
#endif
#endif
}
#endif

template <typename T>
void test_Reg_lshiftr()
{
	T inputs1[mipp::N<T>()];
	T inputs2[mipp::N<T>()];
	std::iota(inputs1, inputs1 + mipp::N<T>(), (T)0);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)0);

	std::mt19937 g;
	std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
	std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

	mipp::Reg<T> r1 = inputs1;
	mipp::Reg<T> r2 = inputs2;
	mipp::Reg<T> r3 = r1 << r2;

	for (auto i = 0; i < mipp::N<T>(); i++)
	{
		T res = inputs1[i] << inputs2[i];
		REQUIRE(r3[i] == res);
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Binary left shift (register) - mipp::Reg", "[mipp::lshiftr]")
{
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_Reg_lshiftr<int64_t>(); }
#endif
	SECTION("datatype = int32_t") { test_Reg_lshiftr<int32_t>(); }
#if defined(MIPP_BW)
#if !defined(MIPP_AVX)
	SECTION("datatype = int16_t") { test_Reg_lshiftr<int16_t>(); }
#if !defined(MIPP_AVX512)
	SECTION("datatype = int8_t") { test_Reg_lshiftr<int8_t >(); }
#endif
#endif
#endif
#endif
}
#endif