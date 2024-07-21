#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class PrimeSieve {
 public:
  explicit PrimeSieve(uint64_t max_prime) : primes_(VectorSize(max_prime)) {
    Initialize();
  }

  bool IsPrime(uint64_t n) const;

  size_t PrimeIdx(uint64_t p) const;

  uint64_t NthPrime(size_t idx) const;

  // If n is prime, returns self.
  uint64_t LargestPrimeBelow(uint64_t n) const;

  uint64_t PrimeAfter(uint64_t p) const;

  uint64_t PrimeBefore(uint64_t p) const;

 private:
  static constexpr size_t kVSize = 8 * sizeof(uint64_t);

  constexpr static size_t VectorSize(uint64_t max_prime) {
    return (max_prime + kVSize - 1) / kVSize;
  }

  uint64_t Range() const {
    return primes_.size() * kVSize;
  }

  void Initialize();

  void ClearMultiples(uint64_t p);

  std::vector<std::pair<uint64_t, uint64_t>> primes_;
};
