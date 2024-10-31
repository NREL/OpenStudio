/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <benchmark/benchmark.h>

#include "../FilesystemHelpers.hpp"
#include "../Path.hpp"
#include "../UnzipFile.hpp"

#include <resources.hxx>

#include <string>
#include <vector>

openstudio::path prepareOutDir(const std::string& test_case) {
  openstudio::path outpath = openstudio::tempDir() / openstudio::toPath(test_case);
  openstudio::filesystem::remove_all(outpath);
  return outpath;
}

static void BM_Unzip(benchmark::State& state) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");
  openstudio::path outpath = prepareOutDir("Current");

  openstudio::UnzipFile uf(p);
  uf.setChunksize(static_cast<unsigned long>(state.range(0)));

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    uf.extractAllFiles(outpath);
    openstudio::filesystem::remove_all(outpath);
  }
}

BENCHMARK(BM_Unzip)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(1024, 8 << 13);
