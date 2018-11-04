/*
	See: https://github.com/Anton94/Median-Holder
*/
#pragma once

namespace MH
{

class MedianHolder
{
public:
	MedianHolder() = default;
	MedianHolder(const MedianHolder&) = default;
	MedianHolder& operator=(const MedianHolder&) = default;
	~MedianHolder() = default;

	MedianHolder(MedianHolder&&) = default;
	MedianHolder& operator=(MedianHolder&&) = default;
private:

};

}