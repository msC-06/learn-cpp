#include <iostream>
#include <deque>
#include <cstddef>
#include <cassert>
#include <cmath>
#include <stdexcept>

template<typename T>
class SlidingWindow{
public:
	explicit SlidingWindow(size_t size)
		:buf_size_(size)
	{
		if (size == 0) {
			throw std::invalid_argument("size can not be 0");

		}
	}
	void push(T data)
	{
		if (buffer_.size() >= buf_size_) {
			sum_ -= buffer_.front();
			buffer_.pop_front();
		}
		sum_ += data;
		buffer_.push_back(data);
	}
	double get_avg() const
	{
		if (buffer_.empty()) return 0.0;
		return sum_ / static_cast<double>(buffer_.size());
	}
	void show_raw() const
	{
		if (buffer_.empty())
		{
			std::cout << "is empty" << std::endl;
			return;
		}

		for (const auto& f : buffer_)
		{
			std::cout << f << " " << std::endl;
		}
	}
	void clear()
	{
		buffer_.clear();
		sum_ = 0.0;
	}
private:
	std::deque<T> buffer_;
	size_t buf_size_;
	double sum_ = 0.0;
};
namespace {
	bool nearly_equal(double a, double b)
	{
		return std::fabs(a - b) < 1e-9;
	}
}
int main() {

	SlidingWindow<int> win(5);

	// 空窗口：均值为 0，不抛异常
	assert(nearly_equal(win.get_avg(), 0.0));

	win.push(1);
	win.push(3);
	win.push(3);
	assert(nearly_equal(win.get_avg(), 7.0 / 3.0));
	std::cout << "win avg = " << win.get_avg() << std::endl;

	win.push(2);
	assert(nearly_equal(win.get_avg(), 9.0 / 4.0));
	std::cout << "win avg = " << win.get_avg() << std::endl;

	win.push(2);
	assert(nearly_equal(win.get_avg(), 11.0 / 5.0));
	std::cout << "win avg = " << win.get_avg() << std::endl;

	// 窗口已满，再 push 应淘汰最早的元素 1
	win.push(2);
	assert(nearly_equal(win.get_avg(), 12.0 / 5.0));
	std::cout << "win avg = " << win.get_avg() << std::endl;

	win.clear();
	assert(nearly_equal(win.get_avg(), 0.0));
	std::cout << "win avg = " << win.get_avg() << std::endl;

	win.push(10);
	assert(nearly_equal(win.get_avg(), 10.0));
	std::cout << "win avg = " << win.get_avg() << std::endl;

	// 容量为 0 时构造函数应拒绝
	bool rejected_zero = false;
	try {
		SlidingWindow<int> zero_size(0);
		(void)zero_size;
	}
	catch (const std::invalid_argument&) {
		rejected_zero = true;
	}
	assert(rejected_zero);

	std::cout << "all sliding window checks passed" << std::endl;

	return 0;
}
