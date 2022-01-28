#include <benchmark/benchmark.h>

#include "../IdfFile.hpp"
#include "../../core/Filesystem.hpp"
#include "../../core/Assert.hpp"

#include <resources.hxx>

#include <OpenStudio.hxx>

using namespace openstudio;

static void BM_LoadIdfFile(benchmark::State& state, const std::string& testCase) {

  path idfPath = resourcesPath() / toPath(testCase);

  for (auto _ : state) {
    OptionalIdfFile oIdfFile = IdfFile::load(idfPath);
  }
}

BENCHMARK_CAPTURE(BM_LoadIdfFile, 5ZoneAirCooled, std::string("energyplus/5ZoneAirCooled/in.idf"));
BENCHMARK_CAPTURE(BM_LoadIdfFile, Daylighting_School, std::string("energyplus/Daylighting_School/in.idf"));
BENCHMARK_CAPTURE(BM_LoadIdfFile, SmallOffice, std::string("energyplus/SmallOffice/SmallOffice.idf"));
BENCHMARK_CAPTURE(BM_LoadIdfFile, Office_With_Many_HVAC_Types, std::string("energyplus/Office_With_Many_HVAC_Types/in.idf"));
BENCHMARK_CAPTURE(BM_LoadIdfFile, RefBldgLargeOffice, std::string("energyplus/RefLargeOffice/RefBldgLargeOfficeNew2004_Chicago.idf"));
BENCHMARK_CAPTURE(BM_LoadIdfFile, HospitalBaseline, std::string("energyplus/HospitalBaseline/in.idf"));

BENCHMARK_CAPTURE(BM_LoadIdfFile, exampleModel_osm, std::string("model/exampleModel.osm"));
