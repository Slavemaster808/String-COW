#pragma once

#include <algorithm>
#include <cstring>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace cowstring {
using sizeType = size_t;
const sizeType npos = -1;
class CowString {
 private:
  std::shared_ptr<char[]> data = nullptr;
  sizeType size_;
  size_t capacity_;

  char* alloc(const char* str, sizeType size) {
    const size_t doubleSize = size * 2;
    char* ptr = new char[doubleSize + 1];
    std::memcpy(ptr, str, size);
    for (sizeType i = size; i < doubleSize + 1; i++) {
      ptr[i] = '\0';
    }
    return ptr;
  }

 public:
  CowString() : data(alloc("", 0)), size_(0), capacity_(0){};

  explicit CowString(const char* str)
      : data(alloc(str, std::strlen(str))),
        size_(std::strlen(str)),
        capacity_(std::strlen(str) * 2){};

  class Iterator {
   private:
    char* currData;

   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = char*;
    using reference = char&;

    explicit Iterator(char* ptr) : currData(ptr){};

    reference operator*() const {
      return *currData;
    }

    pointer operator->() const {
      return currData;
    }

    Iterator& operator++() {
      ++currData;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++currData;
      return tmp;
    }

    Iterator& operator--() {
      --currData;
      return *this;
    }

    Iterator operator--(int) {
      Iterator tmp = *this;
      --currData;
      return tmp;
    }

    constexpr bool operator<(const Iterator& other) const noexcept {
      return currData < other.currData;
    }

    constexpr bool operator>(const Iterator& other) const noexcept {
      return other.currData < currData;
    }

    constexpr bool operator<=(const Iterator& other) const noexcept {
      return !(currData < other.currData);
    }

    constexpr bool operator>=(const Iterator& other) const noexcept {
      return !(other.currData < currData);
    }

    constexpr bool operator==(const Iterator& other) const noexcept {
      return currData == other.currData;
    }

    constexpr bool operator!=(const Iterator& other) const noexcept {
      return !(currData == other.currData);
    }

    Iterator& operator+=(difference_type n) {
      currData += n;
      return *this;
    }

    Iterator& operator-=(difference_type n) {
      currData -= n;
      return *this;
    }

    Iterator operator+(difference_type n) const {
      Iterator tmp = *this;
      tmp += n;
      return tmp;
    }

    Iterator operator-(difference_type n) const {
      Iterator tmp = *this;
      tmp -= n;
      return tmp;
    }

    difference_type operator-(const Iterator& other) const {
      return currData - other.currData;
    }

    reference operator[](difference_type n) const {
      return *(currData + n);
    }
  };

  using iterator = Iterator;
  using const_iterator = Iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() {
    return iterator(data.get());
  }

  iterator end() {
    return iterator(data.get() + size_);
  }

  const_iterator begin() const {
    return const_iterator(data.get());
  }

  const_iterator end() const {
    return const_iterator(data.get() + size_);
  }

  const_iterator cbegin() const {
    return const_iterator(data.get());
  }

  const_iterator cend() const {
    return const_iterator(data.get() + size_);
  }

  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  const char* c_str() const {
    return data.get();
  }

  size_t size() const noexcept {
    return size_;
  }

  size_t capacity() const noexcept {
    return capacity_;
  }

  bool empty() const noexcept {
    return size_ == 0;
  }

  long use_cnt() const noexcept {
    return data.use_count();
  }

  void reserve(sizeType newCap) {
    if (newCap > capacity_) {
      const std::shared_ptr<char[]> newPtr(new char[newCap + 1]);
      std::copy(data.get(), data.get() + size_, newPtr.get());
      newPtr[newCap] = '\0';
      data = newPtr;
      capacity_ = newCap;
    }
  }

  void resize(sizeType newSize, char ch = '\0') {
    if (newSize > size_) {
      if (newSize > capacity_) {
        reserve(newSize);
      }
      std::fill(data.get() + size_, data.get() + newSize, ch);
      data[newSize] = '\0';
    } else if (newSize < size_) {
      data[newSize] = '\0';
    }
    size_ = newSize;
  }

  void shrinkToFit() {
    if (size_ < capacity_) {
      const std::shared_ptr<char[]> newPtr(new char[size_ + 1]);
      std::copy(data.get(), data.get() + size_, newPtr.get());
      newPtr[size_] = '\0';
      data = newPtr;
      capacity_ = size_;
    }
  }

  void pushBack(char ch) {
    if (size_ + 1 >= capacity_) {
      reserve(size_ * 2);
    }
    data[size_] = ch;
    size_++;
    data[size_] = '\0';
  }

  CowString& erase(sizeType index = 0, sizeType count = npos) {
    if (index > size_) {
      throw std::out_of_range("Index out of range.");
    }
    count = std::min(count, size_ - index);
    std::copy(
        data.get() + index + count, data.get() + size_, data.get() + index);
    size_ -= count;
    data[size_] = '\0';
    return *this;
  }

  iterator erase(iterator position) {
    if (position.operator->() < data.get() ||
        position.operator->() >= data.get() + size_) {
      throw std::out_of_range("Iterator out of bounds.");
    }
    auto posIndex = std::distance(begin().operator->(), position.operator->());
    std::copy(
        position.operator->() + 1, data.get() + size_, position.operator->());
    --size_;
    data[size_] = '\0';
    return iterator(data.get() + posIndex);
  }

  iterator erase(iterator first, iterator last) {
    if (first.operator->() < data.get() ||
        last.operator->() > data.get() + size_ ||
        first.operator->() > last.operator->()) {
      throw std::out_of_range("Iterator out of bounds or invalid range.");
    }

    auto firstIndex = std::distance(begin().operator->(), first.operator->());
    auto count = std::distance(first.operator->(), last.operator->());

    if (count > 0) {
      std::copy(last.operator->(), data.get() + size_, first.operator->());
      size_ -= count;
      data[size_] = '\0';
    }
    return iterator(data.get() + firstIndex);
  }

  CowString& insert(sizeType index, sizeType count, char ch) {
    if (index > size_) {
      throw std::out_of_range("Index out of range.");
    }
    const sizeType newSize = size_ + count;
    std::unique_ptr<char[]> newData(new char[newSize + 1]);

    std::copy(data.get(), data.get() + index, newData.get());

    std::fill(newData.get() + index, newData.get() + index + count, ch);

    std::copy(
        data.get() + index, data.get() + size_, newData.get() + index + count);

    data = std::move(newData);
    size_ = newSize;
    data[size_] = '\0';

    return *this;
  }

  iterator insert(iterator pos, char ch) {
    const sizeType index =
        std::distance(begin().operator->(), pos.operator->());
    if (index > size_) {
      throw std::out_of_range("Iterator out of range.");
    }

    const sizeType newSize = size_ + 1;
    std::unique_ptr<char[]> newData(new char[newSize + 1]);

    std::copy(data.get(), data.get() + index, newData.get());

    newData[index] = ch;

    std::copy(
        data.get() + index, data.get() + size_, newData.get() + index + 1);

    data = std::move(newData);
    size_ = newSize;
    data[size_] = '\0';

    return iterator(data.get() + index);
  }

  char& at(sizeType i) {
    if (i >= size_) {
      throw std::out_of_range("Index out of range.");
    }
    return (*this)[i];
  }

  const char& at(sizeType i) const {
    if (i >= size_) {
      throw std::out_of_range("Index out of range.");
    }
    return data[i];
  }

  char& operator[](sizeType i) {
    if (!data.unique()) {
      data = std::shared_ptr<char[]>(alloc(data.get(), size_));
    }
    return data[i];
  }

  const char& operator[](sizeType i) const {
    return data[i];
  }
};
}  // namespace cowstring