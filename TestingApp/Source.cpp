#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MedianHolder.hpp"
#include <vector>

namespace MH
{

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

}