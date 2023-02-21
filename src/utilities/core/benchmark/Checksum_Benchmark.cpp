#include <benchmark/benchmark.h>

#include <sstream>
#include <string>
#include <string_view>
#include <boost/crc.hpp>
#include <fmt/format.h>

namespace openstudio::detail {
bool checksumIgnore(char c) {
  // ignore all whitespace
  //bool result = (c == ' ' || c == '\t' || c == '\n' ||  c == '\v' || c == '\f' || c == '\r');

  // ignore just line feed
  bool result = (c == '\r');

  return result;
}
}  // namespace openstudio::detail

namespace openstudio_old {
/// return 8 character hex checksum of istream
std::string checksum(std::istream& is) {
  boost::crc_32_type crc;
  do {
    const std::streamsize n = 1024;
    char buffer[n];
    is.read(buffer, n);
    std::streamsize readSize = is.gcount();
    //crc.process_bytes(buffer, readSize);

    auto stringSize = static_cast<size_t>(readSize);
    std::string str(buffer, stringSize);
    str.erase(std::remove_if(str.begin(), str.end(), openstudio::detail::checksumIgnore), str.end());
    stringSize = str.size();

    crc.process_bytes(str.c_str(), stringSize);
  } while (is);

  std::stringstream ss;
  ss << std::hex << std::uppercase << crc.checksum();
  std::string result = "00000000";
  std::string checksum = ss.str();
  result.replace(8 - checksum.size(), checksum.size(), checksum);

  return result;
}

/// return 8 character hex checksum of string
std::string checksum(const std::string& s) {
  std::stringstream ss(s);
  return checksum(ss);
}

}  // namespace openstudio_old

namespace openstudio_new {

/// return 8 character hex checksum of istream
std::string checksum(std::istream& is) {
  boost::crc_32_type crc;
  do {
    const std::streamsize n = 1024;
    char buffer[n];
    is.read(buffer, n);
    std::streamsize readSize = is.gcount();
    //crc.process_bytes(buffer, readSize);

    auto stringSize = static_cast<size_t>(readSize);
    std::string str(buffer, stringSize);
    str.erase(std::remove_if(str.begin(), str.end(), openstudio::detail::checksumIgnore), str.end());
    stringSize = str.size();

    crc.process_bytes(str.c_str(), stringSize);
  } while (is);

  return fmt::format("{:0>8X}", crc.checksum());
}

/// return 8 character hex checksum of string
std::string checksum(const std::string& s) {
  std::stringstream ss(s);
  return checksum(ss);
}

/// return 8 character hex checksum of string
std::string checksum_direct(std::string s) {
  s.erase(std::remove_if(s.begin(), s.end(), openstudio::detail::checksumIgnore), s.end());
  boost::crc_32_type crc;
  crc.process_bytes(s.data(), s.length());
  return fmt::format("{:0>8X}", crc.checksum());
}
}  // namespace openstudio_new

std::string makeTestStr(size_t len) {
  constexpr std::string_view testStr = "THIS is a quite long string, \r\nWith some newlines and co.\n";

  auto result = std::string(len, '\0');
  std::generate_n(begin(result), len, [&testStr, i = 0]() mutable { return testStr[i++ % testStr.size()]; });
  // fmt::print("{}, {}", result.size(), result);
  return result;
}

// constexpr auto testStr = "this is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \nthis is a quite long string\rwith some newlines and co \n";

static void BM_CheckSumOld(benchmark::State& state) {
  auto testStr = makeTestStr(state.range(0));
  // fmt::print("Old impl: {}", s);

  for (auto _ : state) {
    auto s = openstudio_old::checksum(testStr);
    benchmark::DoNotOptimize(s);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_CheckSumNew(benchmark::State& state) {

  auto testStr = makeTestStr(state.range(0));
  auto checkSumOld = openstudio_old::checksum(testStr);
  auto checkSumNew = openstudio_new::checksum(testStr);
  assert(checkSumNew == checkSumOld);
  // fmt::print("New impl: {}", s);

  for (auto _ : state) {
    auto s = openstudio_new::checksum(testStr);
    benchmark::DoNotOptimize(s);
  }

  state.SetComplexityN(state.range(0));
}

static void BM_CheckSumNewDirect(benchmark::State& state) {

  auto testStr = makeTestStr(state.range(0));
  auto checkSumOld = openstudio_old::checksum(testStr);
  auto checkSumNew = openstudio_new::checksum_direct(testStr);
  assert(checkSumNew == checkSumOld);
  // fmt::print("New impl: {}", s);

  for (auto _ : state) {
    auto s = openstudio_new::checksum_direct(testStr);
    benchmark::DoNotOptimize(s);
  }

  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_CheckSumOld)->RangeMultiplier(4)->Range(8, 8 << 16)->Complexity();
BENCHMARK(BM_CheckSumNew)->RangeMultiplier(4)->Range(8, 8 << 16)->Complexity();
BENCHMARK(BM_CheckSumNewDirect)->RangeMultiplier(4)->Range(8, 8 << 16)->Complexity();
