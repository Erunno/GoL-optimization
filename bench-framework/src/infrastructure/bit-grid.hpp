#ifndef GOL_BIT_GRID_HPP
#define GOL_BIT_GRID_HPP

#include <cstddef>
#include <vector>

#include "grid.hpp"

namespace gol {

// Row-wise bit encoding policy
template <typename WordType>
struct RowWisePolicy {
  static constexpr std::size_t BITS_PER_WORD = sizeof(WordType) * 8;
  static constexpr std::size_t TILE_WIDTH = BITS_PER_WORD;
  static constexpr std::size_t TILE_HEIGHT = 1;

  static std::size_t index(std::size_t x, std::size_t y, std::size_t width, std::size_t height) {
    std::size_t row_words = (width + BITS_PER_WORD - 1) / BITS_PER_WORD;  // Ceiling division
    return (y * row_words) + (x / BITS_PER_WORD);
  }

  static std::size_t bit_position(std::size_t x, std::size_t y, std::size_t width,
                                  std::size_t height) {
    return x % BITS_PER_WORD;
  }
};

// Tile-based bit encoding policy
template <typename WordType, std::size_t TileWidth, std::size_t TileHeight>
struct TilePolicy {
  static constexpr std::size_t BITS_PER_WORD = sizeof(WordType) * 8;
  static constexpr std::size_t TILE_WIDTH = TileWidth;
  static constexpr std::size_t TILE_HEIGHT = TileHeight;

  static_assert(TileWidth * TileHeight == BITS_PER_WORD, "Tile size must fit within a single word");

  static std::size_t index(std::size_t x, std::size_t y, std::size_t width, std::size_t height) {
    std::size_t tile_x = x / TileWidth;
    std::size_t tile_y = y / TileHeight;
    std::size_t tiles_per_row = (width + TileWidth - 1) / TileWidth;
    return tile_y * tiles_per_row + tile_x;
  }

  static std::size_t bit_position(std::size_t x, std::size_t y, std::size_t width,
                                  std::size_t height) {
    std::size_t local_x = x % TileWidth;
    std::size_t local_y = y % TileHeight;
    return local_y * TileWidth + local_x;
  }
};

// Main BitGrid class using a policy for indexing
template <typename WordType, typename IndexPolicy>
class BitGrid {
 public:
  using Policy = IndexPolicy;

  BitGrid(std::size_t width, std::size_t height)
      : width_(width), height_(height), data(width * height / Policy::BITS_PER_WORD, 0) {}

  std::size_t cells_width() const { return width_; }
  std::size_t cells_height() const { return height_; }

  std::size_t mem_width() const { return width_ / Policy::TILE_WIDTH; }
  std::size_t mem_height() const { return height_ / Policy::TILE_HEIGHT; }

  bool get(std::size_t x, std::size_t y) const {
    if (x >= width_ || y >= height_) return false;

    std::size_t word_idx = Policy::index(x, y, width_, height_);
    std::size_t bit_pos = Policy::bit_position(x, y, width_, height_);

    return (data[word_idx] & (static_cast<WordType>(1) << bit_pos)) != 0;
  }

  void set(std::size_t x, std::size_t y, bool value) {
    if (x >= width_ || y >= height_) return;

    std::size_t word_idx = Policy::index(x, y, width_, height_);
    std::size_t bit_pos = Policy::bit_position(x, y, width_, height_);

    if (value) {
      data[word_idx] |= (static_cast<WordType>(1) << bit_pos);
    } else {
      data[word_idx] &= ~(static_cast<WordType>(1) << bit_pos);
    }
  }

  WordType* memory() { return data.data(); }
  const WordType* memory() const { return data.data(); }

  std::size_t memory_size() const { return data.size() * sizeof(WordType); }

  template <typename BitGridWordType, typename BitGridIndexPolicy, typename FromGridWordType>
  static BitGrid<BitGridWordType, BitGridIndexPolicy> from_grid(
      const Grid<FromGridWordType>& grid) {
    BitGrid<BitGridWordType, BitGridIndexPolicy> result(grid.width(), grid.height());

    for (std::size_t y = 0; y < grid.height(); ++y) {
      for (std::size_t x = 0; x < grid.width(); ++x) {
        result.set(x, y, grid.at(x, y) != 0);
      }
    }

    return result;
  }

  template <typename ResultGridWordType>
  Grid<ResultGridWordType> to_grid() const {
    Grid<ResultGridWordType> grid(width_, height_);

    for (std::size_t y = 0; y < grid.height(); ++y) {
      for (std::size_t x = 0; x < grid.width(); ++x) {
        grid.at(x, y) = get(x, y);
      }
    }

    return grid;
  }

 private:
  std::size_t width_;
  std::size_t height_;
  std::vector<WordType> data;
};

// Convenience typedefs for common cases
template <typename WordType>
using RowWiseBitGrid = BitGrid<WordType, RowWisePolicy<WordType>>;

template <typename WordType>
using Tile8x4BitGrid = BitGrid<WordType, TilePolicy<WordType, 8, 4>>;

template <typename WordType>
using Tile8x8BitGrid = BitGrid<WordType, TilePolicy<WordType, 8, 8>>;

}  // namespace gol

#endif  // GOL_BIT_GRID_HPP
