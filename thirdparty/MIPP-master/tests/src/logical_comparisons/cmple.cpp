#include <exception>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <mipp.h>
#include <catch.hpp>

template <typename T>
void test_reg_cmple()
{
	T inputs1[mipp::N<T>()], inputs2[mipp::N<T>()];
	std::iota(inputs1, inputs1 + mipp::N<T>(), (T)1);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)1);

	std::mt19937 g;
	for (auto t = 0; t < 100; t++)
	{
		std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
		std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

		mipp::reg r1 = mipp::load<T>(inputs1);
		mipp::reg r2 = mipp::load<T>(inputs2);
		mipp::msk m  = mipp::cmple<T>(r1, r2);

		mipp::reg r3 = mipp::toreg<mipp::N<T>()>(m);

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			if (inputs1[i] <= inputs2[i])
				REQUIRE(mipp::get<T>(r3, i) != (T)0);
			else
				REQUIRE(mipp::get<T>(r3, i) == (T)0);
		}
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Compare lower or equal - mipp::reg", "[mipp::cmple]")
{
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_reg_cmple<double>(); }
#endif
	SECTION("datatype = float") { test_reg_cmple<float>(); }

#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if !defined(MIPP_SSE)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_reg_cmple<int64_t>(); }
	SECTION("datatype = uint64_t") { test_reg_cmple<uint64_t>(); }
#endif
#endif
	SECTION("datatype = int32_t") { test_reg_cmple<int32_t>(); }
	SECTION("datatype = uint32_t") { test_reg_cmple<uint32_t>(); }
#endif
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { test_reg_cmple<int16_t>(); }
	SECTION("datatype = uint16_t") { test_reg_cmple<uint16_t>(); }
	SECTION("datatype = int8_t") { test_reg_cmple<int8_t>(); }
	SECTION("datatype = uint8_t") { test_reg_cmple<uint8_t>(); }
#endif
}
#endif

template <typename T>
void test_Reg_cmple()
{
	T inputs1[mipp::N<T>()], inputs2[mipp::N<T>()];
	std::iota(inputs1, inputs1 + mipp::N<T>(), (T)1);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)1);

	std::mt19937 g;
	for (auto t = 0; t < 100; t++)
	{
		std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);
		std::shuffle(inputs2, inputs2 + mipp::N<T>(), g);

		mipp::Reg<T> r1 = inputs1;
		mipp::Reg<T> r2 = inputs2;
		mipp::Msk<mipp::N<T>()> m = r1 <= r2;

		for (auto i = 0; i < mipp::N<T>(); i++)
			REQUIRE(m[i] == (inputs1[i] <= inputs2[i]));
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Compare lower or equal - mipp::Reg", "[mipp::cmple]")
{
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_Reg_cmple<double>(); }
#endif
	SECTION("datatype = float") { test_Reg_cmple<float>(); }

#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if !defined(MIPP_SSE)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { test_Reg_cmple<int64_t>(); }
	SECTION("datatype = uint64_t") { test_Reg_cmple<uint64_t>(); }
#endif
#endif
	SECTION("datatype = int32_t") { test_Reg_cmple<int32_t>(); }
	SECTION("datatype = uint32_t") { test_Reg_cmple<uint32_t>(); }
#endif
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { test_Reg_cmple<int16_t>(); }
	SECTION("datatype = uint16_t") { test_Reg_cmple<uint16_t>(); }
	SECTION("datatype = int8_t") { test_Reg_cmple<int8_t>(); }
	SECTION("datatype = uint8_t") { test_Reg_cmple<uint8_t>(); }
#endif
}
#endif
