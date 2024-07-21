#include "src/prime_sieve.h"

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace math {

/* static */
uint64_t PrimeSieve::PrimePiLB(uint64_t p) {
  switch (p) {
    case 0:
    case 1:
      return 0;
    case 2:
      return 1;
    case 3:
    case 4:
      return 2;
    case 5:
    case 6:
      return 3;
    case 7:
    case 8:
    case 9:
    case 10:
      return 4;
    default:
      break;
  }
  double x = p;
  double log_x = std::log(x);

  if (p < 59) {
    return static_cast<uint64_t>(std::floor(x / log_x));
  }

  return static_cast<uint64_t>(
      std::floor(x / log_x * (1. + 1. / (2. * log_x))));
}

/* static */
uint64_t PrimeSieve::PrimePiUB(uint64_t p) {
  switch (p) {
    case 0:
    case 1:
      return 0;
    case 2:
      return 1;
    case 3:
    case 4:
      return 2;
    default:
      break;
  }
  double x = p;
  double log_x = std::log(x);
  return static_cast<uint64_t>(
      std::floor(x / log_x * (1. + 3. / (2. * log_x))));
}

/* static */
uint64_t PrimeSieve::PrimePiInvLB(uint64_t idx) {
  uint64_t h = 1;
  while (PrimePiUB(2 * h) < idx) {
    h *= 2;
  }

  // l is inclusive, h is exclusive.
  uint64_t l = h;
  h = 2 * h;
  while (l + 1 < h) {
    uint64_t m = (l + h) / 2;
    if (PrimePiUB(m) < idx) {
      l = m;
    } else {
      h = m;
    }
  }

  return l;
}

/* static */
uint64_t PrimeSieve::PrimePiInvUB(uint64_t idx) {
  uint64_t h = 1;
  while (PrimePiLB(h) <= idx) {
    h *= 2;
  }

  // l is exclusive, h is inclusive.
  uint64_t l = h;
  h = 2 * h;
  while (l + 1 < h) {
    uint64_t m = (l + h) / 2;
    if (PrimePiLB(m) <= idx) {
      l = m;
    } else {
      h = m;
    }
  }

  return h;
}

/*static */
PrimeSieve PrimeSieve::WithMaxPrime(uint64_t max_prime) {
  return PrimeSieve(max_prime);
}

/* static */
PrimeSieve PrimeSieve::WithPrimeCount(uint64_t num_primes) {
  return PrimeSieve(PrimePiInvUB(num_primes));
}

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

uint64_t PrimeSieve::PrimeAfter(uint64_t p) const {
  return NthPrime(PrimeIdx(p) + 1);
}

uint64_t PrimeSieve::PrimeBefore(uint64_t p) const {
  return NthPrime(PrimeIdx(p) - 1);
}

uint64_t PrimeSieve::NumPrimes() const {
  return primes_.back().first + std::popcount(primes_.back().second);
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

}  // namespace math
