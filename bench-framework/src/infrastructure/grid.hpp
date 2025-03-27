#ifndef GOL_GRID_HPP
#define GOL_GRID_HPP

#include <cstddef>
#include <random>
#include <vector>

namespace gol {

template <typename WordType>
class Grid {
 public:
  Grid(std::size_t width, std::size_t height)
      : width_(width), height_(height), data(width * height, 0) {}

  std::size_t width() const { return width_; }
  std::size_t height() const { return height_; }
  std::size_t size() const { return data.size(); }

  WordType& at(std::size_t x, std::size_t y) { return data[y * width_ + x]; }
  WordType at(std::size_t x, std::size_t y) const { return data[y * width_ + x]; }
  WordType* memory() { return data.data(); }
  const WordType* memory() const { return data.data(); }

  bool equals(const Grid<WordType>& other) const {
    if (width_ != other.width_ || height_ != other.height_) {
      return false;
    }
    auto self_mem = this->memory();
    auto other_mem = other.memory();

    for (std::size_t i = 0; i < data.size(); ++i) {
      if (self_mem[i] != other_mem[i]) {
        return false;
      }
    }

    return true;
  }

 private:
  std::size_t width_;
  std::size_t height_;
  std::vector<WordType> data;
};

}  // namespace gol

#endif
