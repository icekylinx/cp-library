#pragma once

#pragma GCC target("avx2")
#include <immintrin.h>

#include "lib/utils/debug.hpp"
#include "lib/utils/my_type_traits.hpp"

#ifdef __unix__
#ifndef DISABLE_MMAP
#define ENABLE_MMAP
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#endif

namespace fast_io {

template <int BufSize = 1 << 20>
struct FastInput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;
  size_t map_size;

  explicit FastInput(FILE* _file = stdin) : file(_file) {
#ifdef ENABLE_MMAP
    struct stat st;
    int fd = fileno(file);
    fstat(fd, &st);
    map_size = st.st_size;
    cur = buf = static_cast<char*>(
        mmap(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, 0));
    end = cur + map_size;
#else
    cur = buf = new char[BufSize + 64];
    end = buf + fread(buf, 1, BufSize, file);
    memset(const_cast<char*>(end), 0, 64);
#endif
  }

#ifdef ENABLE_MMAP
  ~FastInput() { munmap(buf, map_size); }
#else
  ~FastInput() { delete[] buf; }
#endif

#ifndef ENABLE_MMAP
  void ensure() {
    int rem = end - cur;
    if (rem >= 40) [[likely]] return;
    if (rem > 0 && cur != buf) memmove(buf, cur, rem);
    cur = buf;
    end = buf + rem + fread(buf + rem, 1, BufSize - rem, file);
    memset(const_cast<char*>(end), 0, 64);
  }
#else
#define ensure() void(0)
#endif

  void skip_space() {
    ensure();
    while (*cur < 33) [[unlikely]] {
      ++cur;
      ensure();
    }
  }

  template <internal::unsigned_integral T>
  T read_small() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = *cur++ & 15;
    uint32_t v;
    memcpy(&v, cur, 4);
    v ^= 0x30303030;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ffull;
      v = (v * 100 + (v >> 16)) & 0xffffull;
      x = x * 10000 + v, cur += 4;
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <internal::signed_integral T>
  T read_small() {
    using U = internal::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v = read_small<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) < 8)
  T read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = *cur++ & 15;
    uint64_t v;
    memcpy(&v, cur, 8);
    v ^= 0x3030303030303030ull;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ffull;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffffull;
      v = (v * 10000 + (v >> 32)) & 0xffffffffull;
      x = x * 100000000 + v, cur += 8;
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) == 8)
  uint64_t read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    __m128i raw = _mm_loadu_si128(reinterpret_cast<const __m128i*>(cur));
    __m128i diff = _mm_sub_epi8(raw, zero_128);
    uint32_t mask = _mm_movemask_epi8(diff);

    if (mask == 0) {
      uint64_t x = parse_w16(diff);
      cur += 16;
      for (; *cur >= 48; ++cur) {
        x = x * 10 + (*cur & 15);
      }
      ++cur;
      return x;
    } else {
      int len = __builtin_ctz(mask);
      cur += len + 1;
      diff = _mm_shuffle_epi8(
          diff, _mm_loadu_si128(reinterpret_cast<const __m128i*>(&LUT[len])));
      return parse_w16(diff);
    }
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) > 8)
  __uint128_t read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    __m256i raw = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur));
    __m256i diff = _mm256_sub_epi8(raw, zero_256);
    uint32_t mask = _mm256_movemask_epi8(diff);

    if (mask == 0) {
      __uint128_t x = parse_w32(diff);
      cur += 32;

      uint32_t v;
      memcpy(&v, cur, 4);
      v ^= 0x30303030;
      uint32_t val = 0, pow = 1;
      if (all_digits(v)) {
        v = (v * 10 + (v >> 8)) & 0xff00ff;
        v = (v * 100 + (v >> 16)) & 0xffff;
        val = v, pow = 10000, cur += 4;
      }

      for (; *cur >= 48; ++cur, pow *= 10) {
        val = val * 10 + (*cur & 15);
      }

      ++cur;
      return x * pow + val;
    } else {
      int len = __builtin_ctz(mask);
      cur += len + 1;
      if (len <= 16) {
        __m128i low = _mm256_castsi256_si128(diff);
        low = _mm_shuffle_epi8(
            low, _mm_loadu_si128(reinterpret_cast<const __m128i*>(&LUT[len])));
        return parse_w16(low);
      } else {
        alignas(32) char aux[64]{};
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(aux + 32 - len), diff);
        diff = _mm256_load_si256(reinterpret_cast<const __m256i*>(aux));
        return parse_w32(diff);
      }
    }
  }

  template <internal::signed_integral T>
  T read() {
    using U = internal::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v = read<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <typename T>
    requires(internal::same_as<T, bool>)
  T read() {
    ensure();
    CHECK(*cur == '0' || *cur == '1');
    T x = *cur & 1;
    cur += 2;
    return x;
  }

  template <typename T>
    requires(internal::same_as<T, char>)
  T read() {
    ensure();
    T x = *cur;
    cur += 2;
    return x;
  }

  template <typename T>
    requires(internal::same_as<T, std::string>)
  T read() {
    ensure();
    CHECK(*cur > 32);

#ifdef ENABLE_MMAP
    char* first = cur;
    while (*cur > 32) ++cur;
    std::string s(first, cur);
    ++cur;
    return s;
#else
    std::string s;
    while (true) {
      char* last = cur;
      while (last < end && *last > 32) ++last;
      if (last < end) {
        s.append(cur, last);
        cur = last + 1;
        return s;
      } else {
        s.append(cur, last);
        cur = end;
        ensure();
      }
    }
#endif
  }

  template <typename T>
  FastInput& operator>>(T& x) {
    skip_space();
    x = read<T>();
    return *this;
  }

  FastInput& operator>>(char* s) {
    skip_space();
    while (*cur > 32) {
      *s++ = *cur++;
      ensure();
    }
    *s = 0, ++cur;
    return *this;
  }

 private:
  static constexpr auto E16 = 10'000'000'000'000'000ull;

  static inline const __m128i zero_128 = _mm_set1_epi8(0x30);
  static inline const __m256i zero_256 = _mm256_set1_epi8(0x30);
  static inline const __m128i w1_128 = _mm_set1_epi16(0x010a);
  static inline const __m256i w1_256 = _mm256_set1_epi16(0x010a);
  static inline const __m128i w2_128 = _mm_set1_epi32(0x00010064);
  static inline const __m256i w2_256 = _mm256_set1_epi32(0x00010064);
  static inline const __m128i w3_128 = _mm_set_epi32(1, 10000, 1, 10000);
  static inline const __m256i w3_256 =
      _mm256_set_epi32(1, 10000, 1, 10000, 1, 10000, 1, 10000);

  static constexpr auto LUT = [] {
    std::array<std::array<char, 16>, 17> res;
    for (int i = 0; i <= 16; ++i) {
      for (int j = 0; j < 16; ++j) {
        res[i][j] = (i + j < 16) ? 0x80 : i + j - 16;
      }
    }
    return res;
  }();

  static inline uint64_t parse_w16(__m128i chunk) {
    __m128i t1 = _mm_maddubs_epi16(chunk, w1_128);
    __m128i t2 = _mm_madd_epi16(t1, w2_128);
    __m128i prod = _mm_mul_epu32(t2, w3_128);
    __m128i odd = _mm_srli_epi64(t2, 32);
    __m128i t3 = _mm_add_epi64(prod, odd);
    uint64_t r0 = _mm_cvtsi128_si64(t3);
    uint64_t r1 = _mm_extract_epi64(t3, 1);
    return r0 * 100000000 + r1;
  }

  static inline __uint128_t parse_w32(__m256i chunk) {
    __m256i t1 = _mm256_maddubs_epi16(chunk, w1_256);
    __m256i t2 = _mm256_madd_epi16(t1, w2_256);
    __m256i prod = _mm256_mul_epu32(t2, w3_256);
    __m256i odd = _mm256_srli_epi64(t2, 32);
    __m256i t3 = _mm256_add_epi64(prod, odd);
    __m128i r0 = _mm256_castsi256_si128(t3);
    __m128i r1 = _mm256_extracti128_si256(t3, 1);
    uint64_t s0 = _mm_cvtsi128_si64(r0) * 100000000 + _mm_extract_epi64(r0, 1);
    uint64_t s1 = _mm_cvtsi128_si64(r1) * 100000000 + _mm_extract_epi64(r1, 1);
    return static_cast<__uint128_t>(s0) * E16 + s1;
  }

  constexpr bool all_digits(uint32_t v) { return !(v & 0xf0f0f0f0); }
  constexpr bool all_digits(uint64_t v) { return !(v & 0xf0f0f0f0f0f0f0f0ull); }
};

struct EndLine {
} endl;

template <uint32_t BufSize = 1 << 20>
struct FastOutput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;

  explicit FastOutput(FILE* _file = stdout) : file(_file) {
    cur = buf = new char[BufSize];
    end = buf + BufSize;
  }

  template <int N = BufSize>
  void flush() {
    if (end - cur < N) [[unlikely]] {
      fwrite(buf, 1, cur - buf, file);
      cur = buf;
    }
  }

  ~FastOutput() {
    flush();
    delete[] buf;
  }

  template <typename T>
    requires(sizeof(T) < 8)
  void write(T x) {
    if (x > 99'999'999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(x);
    } else {
      print<0>(x);
    }
  }

  template <typename T>
    requires(sizeof(T) == 8)
  void write(T x) {
    if (x > 9'999'999'999'999'999ull) {
      print<4>(x);
    } else if (x > 999'999'999'999ull) {
      print<3>(x);
    } else if (x > 99'999'999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(static_cast<uint32_t>(x));
    } else {
      print<0>(static_cast<uint32_t>(x));
    }
  }

  template <typename T>
    requires(sizeof(T) > 8)
  void write(T x) {
    if (x < E19) {
      write(static_cast<uint64_t>(x));
    } else if (x < E38) {
      auto high = x / E19;
      auto low = x - high * E19;
      write(static_cast<uint64_t>(high));
      print_w19(static_cast<uint64_t>(low));
    } else [[unlikely]] {
      auto high = x / E38;
      x -= high * E38;
      auto mid = x / E19;
      auto low = x - mid * E19;
      write(static_cast<uint32_t>(high));
      print_w19(static_cast<uint64_t>(mid));
      print_w19(static_cast<uint64_t>(low));
    }
  }

  template <internal::unsigned_integral T>
  FastOutput& operator<<(T x) {
    flush<std::numeric_limits<T>::digits10 + 1>();
    write(x);
    return *this;
  }

  template <internal::signed_integral T>
  FastOutput& operator<<(T x) {
    using U = internal::make_unsigned_t<T>;

    flush<std::numeric_limits<T>::digits10 + 2>();
    *cur = '-';
    cur += (x < 0);
    write(x < 0 ? -static_cast<U>(x) : static_cast<U>(x));
    return *this;
  }

  FastOutput& operator<<(bool x) {
    flush<1>();
    *cur++ = x + '0';
    return *this;
  }

  FastOutput& operator<<(char x) {
    flush<1>();
    *cur++ = x;
    return *this;
  }

  FastOutput& operator<<(const char* s) {
    uint32_t len = strlen(s);
    if (len > BufSize) [[unlikely]] {
      flush();
      do {
        fwrite(s, 1, BufSize, file);
        s += BufSize;
        len -= BufSize;
      } while (len > BufSize);
    }
    if (end - cur < len) [[unlikely]] flush();
    memcpy(cur, s, len);
    cur += len;
    return *this;
  }

  FastOutput& operator<<(char* s) {
    return *this << const_cast<const char*>(s);
  }

  FastOutput& operator<<(const std::string& s) {
    return *this << s.c_str();
  }

  FastOutput& operator<<(const EndLine& end_line) {
    flush<1>();
    *cur++ = '\n';
    flush();
    return *this;
  }

 private:
  static constexpr auto LUT = [] {
    std::array<std::array<char, 4>, 10000> a, b;

    for (int i = 0; i < 10000; ++i) {
      b[i][0] = '0' + i / 1000;
      b[i][1] = '0' + i / 100 % 10;
      b[i][2] = '0' + i / 10 % 10;
      b[i][3] = '0' + i % 10;

      int j = 0;
      if (i >= 1000) a[i][j++] = b[i][0];
      if (i >= 100) a[i][j++] = b[i][1];
      if (i >= 10) a[i][j++] = b[i][2];
      a[i][j] = b[i][3];
    }

    return std::make_pair(a, b);
  }();

  static constexpr auto E16 = 10'000'000'000'000'000ull;
  static constexpr auto E19 = E16 * 1000;
  static constexpr auto E38 = static_cast<__uint128_t>(E19) * E19;

  template <int N, typename T>
  void print(T x) {
    if constexpr (N == 0) {
      memcpy(cur, &LUT.first[x], 4);
      cur += 1 + (x > 9) + (x > 99) + (x > 999);
    } else {
      print<N - 1>(x / 10000);
      memcpy(cur, &LUT.second[x % 10000], 4);
      cur += 4;
    }
  }

  template <int N, typename T>
  void print_full(T x) {
    if constexpr (N > 0) print_full<N - 1>(x / 10000);
    memcpy(cur, &LUT.second[x % 10000], 4);
    cur += 4;
  }

  void print_w19(uint64_t x) {
    auto high = static_cast<uint32_t>(x / E16);
    auto low = x - high * E16;
    memcpy(cur, &LUT.second[high][1], 3);
    cur += 3;
    print_full<3>(low);
  }
};

template <uint32_t InputBufSize = 1 << 20, uint32_t OutputBufSize = 1 << 20>
struct FastIO {
  FastInput<InputBufSize>* in;
  FastOutput<OutputBufSize>* out;

  FastIO() : in(nullptr), out(nullptr) {}

  ~FastIO() {
    if (in != nullptr) delete in;
    if (out != nullptr) {
      out->flush();
      delete out;
    }
  }

  void init(FILE* input_file = stdin, FILE* output_file = stdout) {
    in = new FastInput<InputBufSize>(input_file);
    out = new FastOutput<OutputBufSize>(output_file);
  }

  void flush() { out->flush(); }

  template <typename T>
  FastIO& operator>>(T& x) {
    *in >> x;
    return *this;
  }

  template <typename T>
  FastIO& operator<<(const T& x) {
    *out << x;
    return *this;
  }

  FastIO& operator<<(const EndLine& x) {
    *out << x;
    return *this;
  }
};

}  // namespace fast_io

using fast_io::FastIO;