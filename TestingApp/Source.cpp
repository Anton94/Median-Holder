#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MedianHolder.hpp"
#include <vector>

namespace MH
{

void ExecuteSequenceOfNumbersTest(const std::vector<int>& sequence, const std::vector<float>& expectedMedians)
{
	REQUIRE(sequence.size() == expectedMedians.size());

	MedianHolder medianHolder;
	bool calculated = false;

	for (size_t i = 0, elemCount = sequence.size(); i < elemCount; ++i)
	{
		medianHolder.insert(sequence[i]);

		auto median = medianHolder.CalculateMedian(&calculated);
		REQUIRE(calculated);

		REQUIRE(median == Approx(expectedMedians[i]));
	}
}

TEST_CASE("Check median of no numbers")
{
	MedianHolder medianHolder;
	bool calculated = false;

	medianHolder.CalculateMedian(&calculated);

	REQUIRE_FALSE(calculated);
}

TEST_CASE("Check median of two opposite numbers in increasing order")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>{ 10, -10 },
		std::vector<float>{ 10.f, 0.f }
	);
}

TEST_CASE("Check median of two opposite numbers in decreasing order")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>{ -10, 10 },
		std::vector<float>{ -10.f, 0.f }
	);
}

TEST_CASE("Check median of two negative numbers in increasing order")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>{ -110, -10 },
		std::vector<float>{ -110.f, -60.f }
	);
}

TEST_CASE("Check median of two negative numbers in decreasing order")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>{ -10, -110 },
		std::vector<float>{ -10.f, -60.f }
	);
}

TEST_CASE("Check median of two positive numbers in increasing order")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>{ 1210, 0 },
		std::vector<float>{ 1210.f, 605.f }
	);
}

TEST_CASE("Check median of two positive numbers in decreasing order")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>{ 0, 1210 },
		std::vector<float>{ 0, 605.f }
	);
}

TEST_CASE("Check median of sequence of numbers with same value")
{
	ExecuteSequenceOfNumbersTest(
		std::vector<int>(100, 42),
		std::vector<float>(100, 42.f)
	);
}

}