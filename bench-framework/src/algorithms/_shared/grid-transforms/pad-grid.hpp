namespace gol {

#include "grid.hpp"

template <typename WordType>
class PaddGrid {
 public:
  static void transform(const Grid<WordType>& grid, std::size_t height_padding,
                        std::size_t width_padding) {
    std::size_t new_width = grid.width() + 2 * width_padding;
    std::size_t new_height = grid.height() + 2 * height_padding;

    Grid<WordType> new_grid(new_width, new_height);

    for (std::size_t y = height_padding; y < new_height - height_padding; ++y) {
      for (std::size_t x = width_padding; x < new_width - width_padding; ++x) {
        new_grid.at(x, y) = grid.at(x - width_padding, y - height_padding);
      }
    }

    grid = new_grid;
  }
}

}  // namespace gol