#include "src/prime_sieve.h"

#include <cmath>

#include "gtest/gtest.h"

namespace math {

static constexpr uint64_t kMaxPrime = 1000000;

class TestPrimeSieve : public ::testing::Test {
 protected:
  static bool IsPrimeNaive(uint64_t num) {
    if (num < 2) {
      return false;
    }
    for (uint64_t d = 2; d <= static_cast<uint64_t>(std::sqrt(num)); d++) {
      if (num % d == 0) {
        return false;
      }
    }
    return true;
  }
};

TEST_F(TestPrimeSieve, TestIsPrime) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  for (uint64_t i = 0; i < kMaxPrime; i++) {
    EXPECT_EQ(sieve.IsPrime(i), IsPrimeNaive(i));
  }
}

TEST_F(TestPrimeSieve, TestWithNumPrimes) {
  for (uint64_t num_primes = 1; num_primes <= 64; num_primes++) {
    PrimeSieve sieve = PrimeSieve::WithPrimeCount(num_primes);
    EXPECT_GE(sieve.NumPrimes(), num_primes);
  }
}

TEST_F(TestPrimeSieve, TestIdx) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t num_primes = 0;
  for (uint64_t i = 0; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      num_primes++;
    }
    EXPECT_EQ(sieve.PrimeIdx(i), num_primes - 1);
  }
}

TEST_F(TestPrimeSieve, TestNthPrime) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t num_primes = 0;
  for (uint64_t i = 0; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      EXPECT_EQ(sieve.NthPrime(num_primes), i);
      num_primes++;
    }
  }

  EXPECT_EQ(sieve.NumPrimes(), num_primes);
}

TEST_F(TestPrimeSieve, TestLargestPrimeBelow) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t prev_prime = 0;
  for (uint64_t i = 0; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      prev_prime = i;
    }
    if (prev_prime != 0) {
      EXPECT_EQ(sieve.LargestPrimeBelow(i), prev_prime);
    }
  }
}

TEST_F(TestPrimeSieve, TestPrimeAfter) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t prev_prime = 0;
  for (uint64_t i = 0; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      if (prev_prime != 0) {
        EXPECT_EQ(sieve.PrimeAfter(prev_prime), i);
      }
      prev_prime = i;
    }
  }
}

TEST_F(TestPrimeSieve, TestPrimeBefore) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t prev_prime = 0;
  for (uint64_t i = 0; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      if (prev_prime != 0) {
        EXPECT_EQ(sieve.PrimeBefore(i), prev_prime);
      }
      prev_prime = i;
    }
  }
}

TEST_F(TestPrimeSieve, TestPrimePi) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t num_primes = 0;
  for (uint64_t i = 0; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      num_primes++;
    }

    EXPECT_LE(PrimeSieve::PrimePiLB(i), num_primes);
    EXPECT_GE(PrimeSieve::PrimePiUB(i), num_primes);
  }
}

TEST_F(TestPrimeSieve, TestPrimePiInv) {
  PrimeSieve sieve = PrimeSieve::WithMaxPrime(kMaxPrime);

  uint64_t num_primes = 0;
  for (uint64_t i = 1; i < kMaxPrime; i++) {
    if (sieve.IsPrime(i)) {
      num_primes++;
    }

    EXPECT_LE(PrimeSieve::PrimePiInvLB(num_primes), i);
    EXPECT_GE(PrimeSieve::PrimePiInvUB(num_primes), i);
  }
}

}  // namespace math
