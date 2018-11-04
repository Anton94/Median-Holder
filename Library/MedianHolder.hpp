/*
	See: https://github.com/Anton94/Median-Holder
*/
#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

namespace MH
{

class MedianHolder
{
	// You can change the element's type with a little modifications of the following three statements:
	typedef int value_type;
	typedef float median_type;

	median_type ChooseMedianOutOfTwoElements(const value_type& left, const value_type& right) const
	{
		return (left + right) / 2.f;
	}

	/*
	The elements are divided into two collections with roughly the same size.
	The structre's invariants are the following:
		- the number of elements in both halves should not be with a difference more than one,
		i.e. | smallerHalfSize - biggerHalfSize | <= 1
		- smaller half should be a max heap
		- upper half should be a min heap
		- every element from the smaller half should be smaller than every element from the bigger half.
		If the halves are proper heaps of the desired type, then this invariant is trivial to check, i.e.
		smaller half's top element should be smaller(or equal) to the bigger half's top element.
	*/

	typedef std::greater<value_type> value_greater_type;
	typedef std::less<value_type> value_less_type;

public:
	MedianHolder() = default;
	MedianHolder(const MedianHolder&) = default;
	MedianHolder& operator=(const MedianHolder&) = default;
	~MedianHolder() = default;

	MedianHolder(MedianHolder&&) = default;
	MedianHolder& operator=(MedianHolder&&) = default;

	void insert(const value_type& element);

	median_type CalculateMedian(bool* success = nullptr) const;

private:
	const value_type& GetBiggestInSmallerHalf() const;
	const value_type& GetSmallestInBiggerHalf() const;

	void InsertFirstCoupleElements(const value_type& element, size_t smallerHalfElementsSize, size_t biggerHalfElementsSize);

	void PushInSmallerHalf(const value_type& element);
	void PushInBiggerHalf(const value_type& element);

	template <class Compare>
	void PushInHeap(std::vector<value_type>& heap, const value_type& element, Compare comp);

	void MoveSmallestFromBiggerHalfToSmallerHalf();
	void MoveBiggestFromSmallerHalfToBiggerHalf();

	template <class CompareLeft, class CompareRight>
	inline void MedianHolder::MoveTopFromLeftHeapToRightHeap(
		std::vector<value_type>& leftHeap, CompareLeft leftComp,
		std::vector<value_type>& rightHeap, CompareRight rightComp);

#ifndef NDEBUG
#define SLOW_MH_CHECK_INVARIANTS SlowCheckInvariants

	void CheckHalvesSizes() const;

	void SlowCheckInvariants() const;
#else
#define SLOW_MH_CHECK_INVARIANTS
#endif
private:
	std::vector<value_type> m_SmallerHalfElements;
	std::vector<value_type> m_BiggerHalfElements;
};

inline void MedianHolder::insert(const value_type& element)
{
	auto smallerHalfElementsSize = m_SmallerHalfElements.size();
	auto biggerHalfElementsSize = m_BiggerHalfElements.size();
	if (smallerHalfElementsSize + biggerHalfElementsSize <= 1)
	{
		InsertFirstCoupleElements(element, smallerHalfElementsSize, biggerHalfElementsSize);
		return;
	}

	if (smallerHalfElementsSize < biggerHalfElementsSize)
	{
		if (element <= GetSmallestInBiggerHalf())
		{
			PushInSmallerHalf(element);
		}
		else
		{
			MoveSmallestFromBiggerHalfToSmallerHalf();
			PushInBiggerHalf(element);
		}
	}
	else if (smallerHalfElementsSize > biggerHalfElementsSize)
	{
		if (element >= GetBiggestInSmallerHalf())
		{
			PushInBiggerHalf(element);
		}
		else
		{
			MoveBiggestFromSmallerHalfToBiggerHalf();
			PushInSmallerHalf(element);
		}
	}
	else
	{
		if (element <= GetSmallestInBiggerHalf())
		{
			PushInSmallerHalf(element);
		}
		else
		{
			PushInBiggerHalf(element);
		}
	}

	SLOW_MH_CHECK_INVARIANTS();
}

inline MedianHolder::median_type MedianHolder::CalculateMedian(bool* success) const
{
	auto smallerHalfElementsSize = m_SmallerHalfElements.size();
	auto biggerHalfElementsSize = m_BiggerHalfElements.size();

	if (smallerHalfElementsSize == 0 && biggerHalfElementsSize == 0)
	{
		if (success)
		{
			*success = false;
		}
		return median_type();
	}

	if (success)
	{
		*success = true;
	}

	if (smallerHalfElementsSize > biggerHalfElementsSize)
	{
		return median_type(GetBiggestInSmallerHalf());
	}

	if (biggerHalfElementsSize > smallerHalfElementsSize)
	{
		return median_type(GetSmallestInBiggerHalf());
	}

	return ChooseMedianOutOfTwoElements(GetBiggestInSmallerHalf(), GetSmallestInBiggerHalf());
}

inline const MedianHolder::value_type& MedianHolder::GetBiggestInSmallerHalf() const
{
	return m_SmallerHalfElements.front();
}

inline const MedianHolder::value_type& MedianHolder::GetSmallestInBiggerHalf() const
{
	return m_BiggerHalfElements.front();
}

inline void MedianHolder::InsertFirstCoupleElements(const value_type& element, size_t smallerHalfElementsSize, size_t biggerHalfElementsSize)
{
	assert(smallerHalfElementsSize + biggerHalfElementsSize <= 1);
	assert(m_SmallerHalfElements.size() == smallerHalfElementsSize && m_BiggerHalfElements.size() == biggerHalfElementsSize);

	// Note: the first element has to be in m_SmallerHalfElements.
	if (smallerHalfElementsSize == 0)
	{
		assert(biggerHalfElementsSize == 0);
		m_SmallerHalfElements.push_back(element);

		SLOW_MH_CHECK_INVARIANTS();
		return;
	}

	if (element < m_SmallerHalfElements.front())
	{
		MoveBiggestFromSmallerHalfToBiggerHalf();

		PushInSmallerHalf(element);
	}
	else
	{
		PushInBiggerHalf(element);
	}

	SLOW_MH_CHECK_INVARIANTS();
}

inline void MedianHolder::PushInSmallerHalf(const value_type& element)
{
	PushInHeap(m_SmallerHalfElements, element, value_less_type());
}

inline void MedianHolder::PushInBiggerHalf(const value_type& element)
{
	PushInHeap(m_BiggerHalfElements, element, value_greater_type());
}

template <class Compare>
inline void MedianHolder::PushInHeap(std::vector<value_type>& heap, const value_type& element, Compare comp)
{
	heap.push_back(element);
	std::push_heap(heap.begin(), heap.end(), comp);

	SLOW_MH_CHECK_INVARIANTS();
}

inline void MedianHolder::MoveSmallestFromBiggerHalfToSmallerHalf()
{
	MoveTopFromLeftHeapToRightHeap(m_BiggerHalfElements, value_greater_type(), m_SmallerHalfElements, value_less_type());
}

inline void MedianHolder::MoveBiggestFromSmallerHalfToBiggerHalf()
{
	MoveTopFromLeftHeapToRightHeap(m_SmallerHalfElements, value_less_type(), m_BiggerHalfElements, value_greater_type());
}

template <class CompareLeft, class CompareRight>
inline void MedianHolder::MoveTopFromLeftHeapToRightHeap(
	std::vector<value_type>& leftHeap, CompareLeft leftComp,
	std::vector<value_type>& rightHeap, CompareRight rightComp)
{
	assert(leftHeap.size() > 0);

	rightHeap.push_back(leftHeap.front());
	std::push_heap(rightHeap.begin(), rightHeap.end(), rightComp);

	std::pop_heap(leftHeap.begin(), leftHeap.end(), leftComp);
	leftHeap.pop_back();

	SLOW_MH_CHECK_INVARIANTS();
}

#ifndef NDEBUG

inline void MedianHolder::CheckHalvesSizes() const
{
	auto smallerHalfElementsSize = m_SmallerHalfElements.size();
	auto biggerHalfElementsSize = m_BiggerHalfElements.size();

	// alternative std::abs, but an additional include of a header file.
	if (smallerHalfElementsSize < biggerHalfElementsSize)
	{
		assert(biggerHalfElementsSize - smallerHalfElementsSize <= 1);
	}
	else
	{
		assert(smallerHalfElementsSize - biggerHalfElementsSize <= 1);
	}
}

inline void MedianHolder::SlowCheckInvariants() const
{
	CheckHalvesSizes();

	assert(std::is_heap(m_SmallerHalfElements.begin(), m_SmallerHalfElements.end()));

	assert(std::is_heap(m_BiggerHalfElements.begin(), m_BiggerHalfElements.end(), value_greater_type()));

	if (m_SmallerHalfElements.size() > 0 && m_BiggerHalfElements.size() > 0)
	{
		assert(m_SmallerHalfElements.front() <= m_BiggerHalfElements.front());
	}
}

#endif

#undef SLOW_MH_CHECK_INVARIANTS
}