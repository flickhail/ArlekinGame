#pragma once
#include <chrono>
#include <concepts>

// Creates a simple timer.
// Requires a floating-point type
template <typename T>
	requires std::is_floating_point_v<T>
class Timer
{
public:
	
	Timer() = default;

	// Resets the timer to zero seconds
	void Reset() noexcept
	{
		m_Beginning = clock_type::now();
	}

	// Returns the value of timer in seconds
	T Elapsed() const noexcept
	{
		return std::chrono::duration_cast<second_type>(clock_type::now() - m_Beginning).count();
	}

private:
	using clock_type = std::chrono::steady_clock;
	using second_type = std::chrono::duration<T, std::ratio<1>>;

	std::chrono::time_point<clock_type> m_Beginning{ clock_type::now() };
};
