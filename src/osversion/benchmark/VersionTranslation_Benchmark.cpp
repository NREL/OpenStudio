/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <benchmark/benchmark.h>

#include "../VersionTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../utilities/core/Filesystem.hpp"

#include <resources.hxx>

#include <OpenStudio.hxx>

using namespace openstudio;

static void BM_VT(benchmark::State& state, const std::string& testCase) {

  path modelPath = resourcesPath() / toPath(testCase);

  for (auto _ : state) {
    osversion::VersionTranslator translator;
    model::OptionalModel result = translator.loadModel(modelPath);
  }
}

BENCHMARK_CAPTURE(BM_VT, example_1_13_4, std::string("osversion/1_13_4/example.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_VT, example_1_14_0, std::string("osversion/1_14_0/example.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_VT, Windows_Complete, std::string("model/7-7_Windows_Complete.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_VT, Model12, std::string("model/15023_Model12.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_VT, floorplan_school, std::string("model/floorplan_school.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_VT, CONTAMTemplate, std::string("contam/CONTAMTemplate.osm"))->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_VT, seb, std::string("Examples/compact_osw/files/seb.osm"))->Unit(benchmark::kMillisecond);
