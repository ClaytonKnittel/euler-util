#include "src/prime_sieve.h"

#include <bit>
#include <cstddef>
#include <cstdint>

bool PrimeSieve::IsPrime(uint64_t n) const {
  size_t idx = n / kVSize;
  uint32_t offset = n % kVSize;
  return ((primes_[idx].second >> offset) & 1) != 0;
}

size_t PrimeSieve::PrimeIdx(uint64_t p) const {
  size_t idx = p / kVSize;
  uint32_t offset = p % kVSize;
  return primes_[idx].first +
         std::popcount(primes_[idx].second & ((UINT64_C(2) << offset) - 1)) - 1;
}

uint64_t PrimeSieve::NthPrime(size_t idx) const {
  uint64_t low = 0;
  uint64_t high = primes_.size();
  while (low + 1 < high) {
    uint64_t m = (low + high) / 2;
    if (primes_[m].first <= idx) {
      low = m;
    } else {
      high = m;
    }
  }

  uint64_t mask = primes_[low].second;
  // Take of the first N least-significant bits, where N is the count above
  // primes_[low].first idx is.
  for (uint64_t i = 0; i < idx - primes_[low].first; i++) {
    mask = mask & (mask - 1);
  }

  return low * kVSize + std::countr_zero(mask);
}

uint64_t PrimeSieve::LargestPrimeBelow(uint64_t n) const {
  return NthPrime(PrimeIdx(n));
}

size_t PrimeSieve::PrimeAfter(uint64_t p) const {
  return NthPrime(PrimeIdx(p) + 1);
}

size_t PrimeSieve::PrimeBefore(uint64_t p) const {
  return NthPrime(PrimeIdx(p) - 1);
}

void PrimeSieve::Initialize() {
  // 0 and 1 are not prime.
  primes_[0].first = 0;
  primes_[0].second = ~0x3;
  for (size_t i = 1; i < primes_.size(); i++) {
    primes_[i].first = i * kVSize - 2;
    primes_[i].second = UINT64_C(-1);
  }

  for (uint64_t p = 2; p < Range(); p++) {
    if (IsPrime(p)) {
      ClearMultiples(p);
    }
  }

  uint64_t num_primes = 0;
  for (auto& prime : primes_) {
    prime.first = num_primes;
    num_primes += std::popcount(prime.second);
  }
}

void PrimeSieve::ClearMultiples(uint64_t p) {
  for (uint64_t n = 2; n * p < Range(); n++) {
    size_t x = n * p;
    uint64_t mask = primes_[x / kVSize].second;
    primes_[x / kVSize].second = mask & ~(UINT64_C(1) << (x % kVSize));
  }
}
