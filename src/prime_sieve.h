#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "gtest/gtest_prod.h"

namespace math {

class PrimeSieve {
  FRIEND_TEST(TestPrimeSieve, TestWithNumPrimes);

 public:
  // Lower bound for the ordinal of prime `p`, i.e. the count of primes `<= p`.
  //
  // PrimePi(2) <= 1
  // PrimePi(3) <= 2
  static uint64_t PrimePiLB(uint64_t p);

  // Upper bound for the ordinal of prime `p`, i.e. the count of primes `<= p`.
  //
  // PrimePi(2) >= 1
  // PrimePi(3) >= 2
  static uint64_t PrimePiUB(uint64_t p);

  // Inverse of `PrimePiUB()`, i.e. a lower bound on the prime with ordinal
  // `idx`. The returned number is not guaranteed to be prime.
  static uint64_t PrimePiInvLB(uint64_t idx);

  // Inverse of `PrimePiLB()`, i.e. an upper bound on the prime with ordinal
  // `idx`. The returned number is not guaranteed to be prime.
  static uint64_t PrimePiInvUB(uint64_t idx);

  // Constructs a prime sieve up to `max_prime`, i.e. all primes in the sieve
  // will be `<= max_prime`.
  static PrimeSieve WithMaxPrime(uint64_t max_prime);

  // Constructs a prime sieve with capacity for at least the first `num_primes`
  // primes. I.e. `NthPrime(num_primes)` will always exist.
  static PrimeSieve WithPrimeCount(uint64_t num_primes);

  bool IsPrime(uint64_t n) const;

  size_t PrimeIdx(uint64_t p) const;

  uint64_t NthPrime(size_t idx) const;

  // If n is prime, returns self.
  uint64_t LargestPrimeBelow(uint64_t n) const;

  uint64_t PrimeAfter(uint64_t p) const;

  uint64_t PrimeBefore(uint64_t p) const;

  // Returns the number of primes in this sieve.
  uint64_t NumPrimes() const;

 private:
  static constexpr size_t kVSize = 8 * sizeof(uint64_t);

  explicit PrimeSieve(uint64_t max_prime) : primes_(VectorSize(max_prime)) {
    Initialize();
  }

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

}  // namespace math
