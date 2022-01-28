#include <benchmark/benchmark.h>

#include "../IddFile_Impl.hpp"
#include "../IddFile.hpp"
#include "../../core/Filesystem.hpp"
#include "../../core/Assert.hpp"

#include <resources.hxx>

#include <OpenStudio.hxx>

using namespace openstudio;

static void BM_ParseEnergyPlusIdd(benchmark::State& state, const std::string& testCase) {

  path iddPath = resourcesPath() / toPath("energyplus/ProposedEnergy+.idd");

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    openstudio::filesystem::ifstream inFile(iddPath);
    BOOST_ASSERT(inFile ? true : false);
    std::shared_ptr<openstudio::detail::IddFile_Impl> iddFileImpl_ptr;

    if (testCase == "Old") {
      iddFileImpl_ptr = openstudio::detail::IddFile_Impl::load(inFile);  //, boost::none);
      //} else if (testCase == "New") {
      //iddFileImpl_ptr = openstudio::detail::IddFile_Impl::load(inFile, false);
      //} else if (testCase == "NewParallel") {
      //iddFileImpl_ptr = openstudio::detail::IddFile_Impl::load(inFile, true);
    } else {
      BOOST_ASSERT(false);
    }

    BOOST_ASSERT(iddFileImpl_ptr);

    inFile.close();
  }
}

static void BM_ParseOpenStudioIdd(benchmark::State& state, const std::string& testCase) {

  path iddPath = resourcesPath() / toPath("model/OpenStudio.idd");

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    openstudio::filesystem::ifstream inFile(iddPath);
    BOOST_ASSERT(inFile ? true : false);
    std::shared_ptr<openstudio::detail::IddFile_Impl> iddFileImpl_ptr;

    if (testCase == "Old") {
      iddFileImpl_ptr = openstudio::detail::IddFile_Impl::load(inFile);  //, boost::none);
      //} else if (testCase == "New") {
      //iddFileImpl_ptr = openstudio::detail::IddFile_Impl::load(inFile, false);
      //} else if (testCase == "NewParallel") {
      //iddFileImpl_ptr = openstudio::detail::IddFile_Impl::load(inFile, true);
    } else {
      BOOST_ASSERT(false);
    }

    BOOST_ASSERT(iddFileImpl_ptr);

    inFile.close();
  }
}

BENCHMARK_CAPTURE(BM_ParseEnergyPlusIdd, Old, std::string("Old"))->Unit(benchmark::kMillisecond);
// BENCHMARK_CAPTURE(BM_ParseEnergyPlusIdd, New, std::string("New"));
// BENCHMARK_CAPTURE(BM_ParseEnergyPlusIdd, NewParallel, std::string("NewParallel"));

BENCHMARK_CAPTURE(BM_ParseOpenStudioIdd, Old, std::string("Old"))->Unit(benchmark::kMillisecond);
// BENCHMARK_CAPTURE(BM_ParseOpenStudioIdd, New, std::string("New"));
// BENCHMARK_CAPTURE(BM_ParseOpenStudioIdd, NewParallel, std::string("NewParallel"));
