#pragma once
#include <chrono>

class Timer
{
public:
	Timer() = default;
	void Reset();
	double Elapsed() const;

private:
	using clock_type = std::chrono::steady_clock;
	using second_type = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_type> m_beginning{ clock_type::now() };
};