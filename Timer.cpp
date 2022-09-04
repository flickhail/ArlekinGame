#include "Timer.h"

void Timer::Reset() noexcept
{
	m_beginning = clock_type::now();
}

double Timer::Elapsed() const noexcept
{
	return std::chrono::duration_cast<second_type>(clock_type::now() - m_beginning).count();
}