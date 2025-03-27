#include <chrono>
#include <cxxopts.hpp>
#include <iostream>
#include <random>

#include "cuda_hello.cuh"
#include "infrastructure/bit-grid.hpp"
#include "infrastructure/grid.hpp"

template <typename T>
void fill_random_grid(gol::Grid<T>& grid, float density = 0.3) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution dist(density);

  for (std::size_t y = 0; y < grid.height(); ++y) {
    for (std::size_t x = 0; x < grid.width(); ++x) {
      grid.at(x, y) = dist(gen) ? 1 : 0;
    }
  }
}

template <typename WordType, typename Policy>
void test_bit_grid_conversion() {
  std::cout << "Testing BitGrid with " << sizeof(WordType) * 8 << "-bit words" << std::endl;

  const std::size_t width = 1024 * 16;
  const std::size_t height = 2048 * 8;

  // Create and fill a random grid
  gol::Grid<uint8_t> original(width, height);
  fill_random_grid(original);
  std::cout << "Created random grid: " << width << "x" << height << std::endl;

  // Convert to BitGrid
  auto start = std::chrono::high_resolution_clock::now();
  auto bit_grid = gol::BitGrid<WordType, Policy>::template from_grid<WordType, Policy>(original);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> conversion_to_bit_ms = end - start;

  std::cout << "Converted to BitGrid in " << conversion_to_bit_ms.count() << " ms" << std::endl;
  std::cout << "BitGrid memory size: " << bit_grid.memory_size() << " bytes" << std::endl;

  // Convert back to regular Grid
  start = std::chrono::high_resolution_clock::now();
  auto converted = bit_grid.template to_grid<uint8_t>();
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> conversion_from_bit_ms = end - start;

  std::cout << "Converted back to Grid in " << conversion_from_bit_ms.count() << " ms" << std::endl;

  // Check if they are equal
  bool equal = original.equals(converted);
  std::cout << "Grids are " << (equal ? "equal" : "NOT equal") << " after double conversion"
            << std::endl;

  if (!equal) {
    // Find the first mismatch
    for (std::size_t y = 0; y < height; ++y) {
      for (std::size_t x = 0; x < width; ++x) {
        if (original.at(x, y) != converted.at(x, y)) {
          std::cout << "First mismatch at position (" << x << ", " << y
                    << "): " << "original=" << (int)original.at(x, y)
                    << ", converted=" << (int)converted.at(x, y) << std::endl;
          return;
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  // Set up command line argument parsing using cxxopts
  cxxopts::Options options("GoL_Benchmark",
                           "Benchmark application for CUDA Game of Life implementations");

  options.add_options()("h,help", "Show help")(
      "t,test", "Test name to run", cxxopts::value<std::string>()->default_value("default"))(
      "n,number", "A number to print (for testing cxxopts)", cxxopts::value<int>());

  // Parse arguments
  auto result = options.parse(argc, argv);

  // Show help if requested
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  // Get the test parameter
  std::string test_name = result["test"].as<std::string>();
  std::cout << "Running test: " << test_name << std::endl;

  // Check if number is provided and print it
  if (result.count("number")) {
    int number = result["number"].as<int>();
    std::cout << "Number provided: " << number << std::endl;
  }

  // C++ hello world
  std::cout << "Hello from C++!" << std::endl;

  // Call CUDA hello world
  cuda_hello();

  // Run the appropriate test based on command line arguments
  if (test_name == "row") {
    test_bit_grid_conversion<uint32_t, gol::RowWisePolicy<uint32_t>>();
  } else if (test_name == "tile8x4") {
    test_bit_grid_conversion<uint32_t, gol::TilePolicy<uint32_t, 8, 4>>();
  } else if (test_name == "tile8x8") {
    test_bit_grid_conversion<uint64_t, gol::TilePolicy<uint64_t, 8, 8>>();
  } else {
    std::cerr << "Unknown BitGrid type: " << test_name << std::endl;
    return 1;
  }

  return 0;
}
