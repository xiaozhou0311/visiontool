#include <exception>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <mipp.h>
#include <catch.hpp>

template <typename T>
void test_reg_trunc()
{
	constexpr int N = mipp::N<T>();
	T inputs1[N];
	std::mt19937 g;
	std::uniform_real_distribution<T> dis((T)-10, (T)10);

	for (auto l = 0; l < 100; l++)
	{
		for (auto i = 0; i < N; i++)
			inputs1[i] = dis(g);

		mipp::reg r1 = mipp::load<T>(inputs1);
		mipp::reg r2 = mipp::trunc<T>(r1);

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			T res = std::trunc(inputs1[i]);
			REQUIRE(mipp::get<T>(r2, i) == res);
		}
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Truncate - mipp::reg", "[mipp::trunc]")
{
#if !defined(MIPP_SSE) || (defined(MIPP_SSE) && MIPP_INSTR_VERSION >= 41)
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_reg_trunc<double>(); }
#endif
	SECTION("datatype = float") { test_reg_trunc<float>(); }
#endif
}
#endif

template <typename T>
void test_Reg_trunc()
{
	constexpr int N = mipp::N<T>();
	T inputs1[N];
	std::mt19937 g;
	std::uniform_real_distribution<T> dis((T)-10, (T)10);

	for (auto l = 0; l < 100; l++)
	{
		for (auto i = 0; i < N; i++)
			inputs1[i] = dis(g);

		mipp::Reg<T> r1 = inputs1;
		mipp::Reg<T> r2 = mipp::trunc(r1);

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			T res = std::trunc(inputs1[i]);
			REQUIRE(r2[i] == res);
		}
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Truncate - mipp::Reg", "[mipp::trunc]")
{
#if !defined(MIPP_SSE) || (defined(MIPP_SSE) && MIPP_INSTR_VERSION >= 41)
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_Reg_trunc<double>(); }
#endif
	SECTION("datatype = float") { test_Reg_trunc<float>(); }
#endif
}
#endif

template <typename T>
void test_reg_maskz_trunc()
{
	constexpr int N = mipp::N<T>();
	T inputs1[N];
	std::mt19937 g;
	std::uniform_real_distribution<T> dis((T)-10, (T)10);

	bool mask[N];
	std::fill(mask,       mask + N/2, true );
	std::fill(mask + N/2, mask + N,   false);

	for (auto l = 0; l < 100; l++)
	{
		for (auto i = 0; i < N; i++)
			inputs1[i] = dis(g);

		std::shuffle(mask, mask + mipp::N<T>(), g);

		mipp::reg r1 = mipp::load<T>(inputs1);
		mipp::msk m  = mipp::set <N>(mask   );

		mipp::reg r2 = mipp::maskz<T,mipp::trunc<T>>(m, r1);

		for (auto i = 0; i < N; i++)
		{
			if (mask[i])
			{
				T res = std::trunc(inputs1[i]);
				REQUIRE(mipp::get<T>(r2, i) == res);
			}
			else
				REQUIRE(mipp::get<T>(r2, i) == (T)0);
		}
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Truncate - mipp::reg - maskz", "[mipp::trunc]")
{
#if !defined(MIPP_SSE) || (defined(MIPP_SSE) && MIPP_INSTR_VERSION >= 41)
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_reg_maskz_trunc<double>(); }
#endif
	SECTION("datatype = float") { test_reg_maskz_trunc<float>(); }
#endif
}
#endif

template <typename T>
void test_Reg_maskz_trunc()
{
	constexpr int N = mipp::N<T>();
	T inputs1[N];
	std::mt19937 g;
	std::uniform_real_distribution<T> dis((T)-10, (T)10);

	bool mask[N];
	std::fill(mask,       mask + N/2, true );
	std::fill(mask + N/2, mask + N,   false);

	for (auto l = 0; l < 100; l++)
	{
		for (auto i = 0; i < N; i++)
			inputs1[i] = dis(g);

		std::shuffle(mask, mask + mipp::N<T>(), g);

		mipp::Reg<T> r1 = inputs1;
		mipp::Msk<N> m  = mask;

		mipp::Reg<T> r2 = mipp::maskz<T,mipp::trunc>(m, r1);

		for (auto i = 0; i < N; i++)
		{
			if (mask[i])
			{
				T res = std::trunc(inputs1[i]);
				REQUIRE(r2[i] == res);
			}
			else
				REQUIRE(r2[i] == (T)0);
		}
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Truncate - mipp::Reg - maskz", "[mipp::trunc]")
{
#if !defined(MIPP_SSE) || (defined(MIPP_SSE) && MIPP_INSTR_VERSION >= 41)
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_Reg_maskz_trunc<double>(); }
#endif
	SECTION("datatype = float") { test_Reg_maskz_trunc<float>(); }
#endif
}
#endif

template <typename T>
void test_reg_mask_trunc()
{
	constexpr int N = mipp::N<T>();
	T inputs1[N], inputs2[N];
	std::mt19937 g;
	std::uniform_real_distribution<T> dis((T)-10, (T)10);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)0);

	bool mask[N];
	std::fill(mask,       mask + N/2, true );
	std::fill(mask + N/2, mask + N,   false);

	for (auto l = 0; l < 100; l++)
	{
		for (auto i = 0; i < N; i++)
			inputs1[i] = dis(g);

		std::shuffle(mask, mask + mipp::N<T>(), g);

		mipp::reg r1 = mipp::load<T>(inputs1);
		mipp::reg r2 = mipp::load<T>(inputs2);
		mipp::msk m  = mipp::set <N>(mask   );

		mipp::reg r3 = mipp::mask<T,mipp::trunc<T>>(m, r2, r1);

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			if (mask[i])
			{
				T res = std::trunc(inputs1[i]);
				REQUIRE(mipp::get<T>(r3, i) == res);
			}
			else
				REQUIRE(mipp::get<T>(r3, i) == inputs2[i]);
		}
	}
}

#if !defined(MIPP_NO) && !defined(MIPP_SVE_LS)
TEST_CASE("Truncate - mipp::reg - mask", "[mipp::trunc]")
{
#if !defined(MIPP_SSE) || (defined(MIPP_SSE) && MIPP_INSTR_VERSION >= 41)
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_reg_mask_trunc<double>(); }
#endif
	SECTION("datatype = float") { test_reg_mask_trunc<float>(); }
#endif
}
#endif

template <typename T>
void test_Reg_mask_trunc()
{
	constexpr int N = mipp::N<T>();
	T inputs1[N], inputs2[N];
	std::mt19937 g;
	std::uniform_real_distribution<T> dis((T)-10, (T)10);
	std::iota(inputs2, inputs2 + mipp::N<T>(), (T)0);

	bool mask[N];
	std::fill(mask,       mask + N/2, true );
	std::fill(mask + N/2, mask + N,   false);

	for (auto l = 0; l < 100; l++)
	{
		for (auto i = 0; i < N; i++)
			inputs1[i] = dis(g);

		std::shuffle(mask, mask + mipp::N<T>(), g);

		mipp::Reg<T> r1 = inputs1;
		mipp::Reg<T> r2 = inputs2;
		mipp::Msk<N> m  = mask;

		mipp::Reg<T> r3 = mipp::mask<T,mipp::trunc>(m, r2, r1);

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			if (mask[i])
			{
				T res = std::trunc(inputs1[i]);
				REQUIRE(r3[i] == res);
			}
			else
				REQUIRE(r3[i] == inputs2[i]);
		}
	}
}

#if !defined(MIPP_SVE_LS)
TEST_CASE("Truncate - mipp::Reg - mask", "[mipp::trunc]")
{
#if !defined(MIPP_SSE) || (defined(MIPP_SSE) && MIPP_INSTR_VERSION >= 41)
#if defined(MIPP_64BIT)
	SECTION("datatype = double") { test_Reg_mask_trunc<double>(); }
#endif
	SECTION("datatype = float") { test_Reg_mask_trunc<float>(); }
#endif
}
#endif

