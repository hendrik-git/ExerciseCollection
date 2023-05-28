#pragma once
#include <algorithm>
#include <functional>
#include <ranges>
#include <shared_mutex>
#include <stdexcept>


namespace DataStructures
{
	inline namespace PriorityQueue
	{
		template<class T, class Compare = std::less<typename std::vector<T>::value_type>>
		class priority_queue
		{
			using value_type	  = typename std::vector<T>::value_type;
			using size_type		  = typename std::vector<T>::size_type;
			using reference		  = typename std::vector<T>::reference;
			using const_reference = typename std::vector<T>::const_reference;

		  public:
			[[nodiscard]] auto empty() const noexcept -> bool
			{
				return data.empty();
			}

			[[nodiscard]] auto size() const noexcept -> size_type
			{
				return data.size();
			}

			void push(const value_type& value)
			{
				data.push_back(value);
				std::ranges::push_heap(data, comparer);
			}

			void pop()
			{
				std::ranges::pop_heap(data, comparer);
				data.pop_back();
			}

			[[nodiscard]] auto top() const -> const_reference
			{
				return data.front();
			}

			void swap(priority_queue& other) noexcept
			{
				swap(data, other.data);
				swap(comparer, other.comparer);
			}

		  private:
			std::vector<T> data;
			Compare		   comparer;
		};

		template<class T, class Compare>
		void swap(priority_queue<T, Compare>& lhs,
				  priority_queue<T, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
		{
			lhs.swap(rhs);
		}
	}  // namespace PriorityQueue

	inline namespace CircularBuffer
	{
		template<class T>
		class circular_buffer
		{
			circular_buffer() = delete;

		  public:
			explicit circular_buffer(const size_t size) : data_(size) {}

			auto clear() noexcept -> bool
			{
				head_ = -1;
				size_ = 0;
			}

			auto empty() const noexcept -> bool
			{
				return size_ == 0;
			}

			auto full() const noexcept -> bool
			{
				return size_ == data_.size();
			}

			auto capacity() const noexcept -> size_t
			{
				return data_.size();
			}

			auto size() const noexcept -> size_t
			{
				return size_;
			}

			void push(const T item)
			{
				head_		 = next_pos();
				data_[head_] = item;
				if(size_ < data_.size())
				{
					size_++;
				}
			}

			[[nodiscard]] auto pop() -> T
			{
				if(empty())
				{
					throw std::runtime_error("buffer is empty");
				}
				auto pos = first_pos();
				size_--;
				return data_[pos];
			}

		  private:
			std::vector<T> data_;
			size_t		   head_ = -1;
			size_t		   size_ = 0;

			auto next_pos() const noexcept -> size_t
			{
				return size_ == 0 ? 0 : (head_ + 1) % data_.size();
			}

			auto first_pos() const noexcept -> size_t
			{
				return size_ == 0 ? 0 : (head_ + data_.size() - size_ + 1) % data_.size();
			}
		};


	}  // namespace CircularBuffer

	inline namespace DoubleBuffer
	{
		template<typename T>
		class double_buffer
		{
			using value_type	  = T;
			using reference		  = T&;
			using const_reference = const T&;
			using pointer		  = T*;

		  public:
			explicit double_buffer(const size_t size) : readbuf_(size), writebuf_(size) {}

			[[nodiscard]] auto size() const noexcept -> size_t
			{
				return readbuf_.size();
			}

			void write(const T* const ptr, const size_t size)
			{
				std::unique_lock lock(mutex_);
				auto			 length = std::min(size, writebuf_.size());
				std::copy(ptr, ptr + length, std::begin(writebuf_));
				writebuf_.swap(readbuf_);
			}

			template<class Output>
			void read(Output it) const
			{
				std::shared_lock lock(mutex_);
				std::ranges::copy(readbuf_, it);
			}

			[[nodiscard]] auto data() const -> pointer
			{
				std::shared_lock lock(mutex_);
				return readbuf_.data();
			}

			[[nodiscard]] auto operator[](const size_t pos) -> reference
			{
				std::shared_lock lock(mutex_);
				return readbuf_[pos];
			}

			[[nodiscard]] auto operator[](const size_t pos) const -> const_reference
			{
				std::shared_lock lock(mutex_);
				return readbuf_[pos];
			}

			void swap(double_buffer other)
			{
				std::swap(readbuf_, other.readbuf_);
				std::swap(writebuf_, other.writebuf_);
			}

		  private:
			std::vector<T>			  readbuf_;
			std::vector<T>			  writebuf_;
			mutable std::shared_mutex mutex_;
		};
	}  // namespace DoubleBuffer
}  // namespace DataStructures