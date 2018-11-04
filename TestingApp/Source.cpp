#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MedianHolder.hpp"
#include <vector>

namespace MH
{

// All tests are testing the median of a sequence of numbers.
// The median is calculated after each insertion of a number from the sequence.

void ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(const std::vector<int>& numbers, const std::vector<float>& expectedMedians)
{
	REQUIRE(numbers.size() == expectedMedians.size());

	MedianHolder medianHolder;
	bool calculated = false;

	for (size_t i = 0, elemCount = numbers.size(); i < elemCount; ++i)
	{
		medianHolder.insert(numbers[i]);

		auto median = medianHolder.CalculateMedian(&calculated);
		REQUIRE(calculated);

		REQUIRE(median == Approx(expectedMedians[i]));
	}
}

TEST_CASE("Empty sequence")
{
	MedianHolder medianHolder;
	bool calculated = false;

	medianHolder.CalculateMedian(&calculated);

	REQUIRE_FALSE(calculated);
}

TEST_CASE("Two opposite numbers in increasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ 10, -10 },
		std::vector<float>{ 10.f, 0.f }
	);
}

TEST_CASE("Two opposite numbers in decreasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ -10, 10 },
		std::vector<float>{ -10.f, 0.f }
	);
}

TEST_CASE("Three negative numbers in increasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ -112, -110 , -10},
		std::vector<float>{ -112.f, -111.f, -110.f }
	);
}

TEST_CASE("Three negative numbers in decreasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ -10, -110 , -112},
		std::vector<float>{ -10.f, -60.f , -110.f }
	);
}

TEST_CASE("Three positive numbers in increasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ 0, 1210 , 2390},
		std::vector<float>{ 0, 605.f, 1210.f }
	);
}

TEST_CASE("Three positive numbers in decreasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ 2390, 1210, 0 },
		std::vector<float>{ 2390.f, 1800.f, 1210.f }
	);
}

TEST_CASE("Sequence of numbers with same value")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>(100, 42),
		std::vector<float>(100, 42.f)
	);
}

TEST_CASE("Sequence of numbers with only two values (successively)")
{
	std::vector<int> numbers(50, 42);
	numbers.insert(numbers.end(), 60, -42);

	std::vector<float> expectedMedians(99, 42.f);
	expectedMedians.push_back(0.f);
	expectedMedians.insert(expectedMedians.end(), 10, -42.f);

	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(numbers, expectedMedians);
}

TEST_CASE("Sequence of sequentional numbers increasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
		std::vector<float>{ 0.f, 0.5f, 1.f, 1.5f, 2.f, 2.5f, 3.f, 3.5f, 4.f, 4.5f, 5.f }
	);
}

TEST_CASE("Sequence of sequentional numbers decreasing order")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
		std::vector<float>{ 0.f, 0.5f, 1.f, 1.5f, 2.f, 2.5f, 3.f, 3.5f, 4.f, 4.5f, 5.f }
	);
}

TEST_CASE("Forcing the median holder structure's halves to reorganize #000")
{
	ExecuteSequenceOfNumberInsertionAndMedianCalculationTest(
		std::vector<int>{   1,   2,    3,   9,    9,   9,   9,   0,   -1,  -2,   -3,  -4,   -5,  -6,   -7,  -8 },
		std::vector<float>{ 1.f, 1.5f, 2.f, 2.5f, 3.f, 6.f, 9.f, 6.f, 3.f, 2.5f, 2.f, 1.5f, 1.f, 0.5f, 0.f, -0.5f }
	);
}


/*
	TODO: add fuzz tests.
	Generate an array of random numbers.
	Use sorting to calculate the median 'until' each position.
	Run the test.
	If the test fails, log the seed to be able to regenerate the problematic sequence.
*/

}