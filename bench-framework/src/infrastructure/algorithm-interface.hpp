#ifndef GOL_ALGORITHM_INTERFACE_HPP
#define GOL_ALGORITHM_INTERFACE_HPP

#include <cstddef>

namespace algorithms {

class GolAlgorithm {
 public:
  virtual void initialize() = 0;
  virtual void run(std::size_t iterations) = 0;
  virtual void finalize() = 0;
};

}  // namespace algorithms

#endif