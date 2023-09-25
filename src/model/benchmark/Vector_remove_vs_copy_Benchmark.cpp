/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <benchmark/benchmark.h>

#include "../Model.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/core/Assert.hpp"

#include <vector>
#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

model::Model makeModelWithNSurfaces(size_t nSurfaces) {
  Model m;
  double zOrigin = 0;
  Point3dVector pts{{0, 0, zOrigin}, {0, 1, zOrigin}, {1, 1, zOrigin}, {1, 0, zOrigin}};

  for (size_t i = 0; i < nSurfaces; ++i) {
    Surface s(pts, m);
    if (i % 2 == 0) {
      s.setSurfaceType("RoofCeiling");
      s.setOutsideBoundaryCondition("Outdoors");
    }
  }

  OS_ASSERT(m.getConcreteModelObjects<Surface>().size() == nSurfaces);
  return m;
}

static void Current(benchmark::State& state) {

  Model m = makeModelWithNSurfaces(state.range(0));

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    SurfaceVector result;
    SurfaceVector candidates = m.getConcreteModelObjects<Surface>();
    for (const Surface& candidate : candidates) {
      std::string surfaceType = candidate.surfaceType();
      std::string outsideBoundaryCondition = candidate.outsideBoundaryCondition();
      if (openstudio::istringEqual(surfaceType, "RoofCeiling") && openstudio::istringEqual(outsideBoundaryCondition, "Outdoors")) {
        result.push_back(candidate);
      }
    }
    benchmark::DoNotOptimize(result);
  };

  state.SetComplexityN(state.range(0));
}

static void Proposed(benchmark::State& state) {

  Model m = makeModelWithNSurfaces(state.range(0));

  auto isRoof = [](const Surface& s) -> bool {
    return openstudio::istringEqual(s.surfaceType(), "RoofCeiling") && openstudio::istringEqual(s.outsideBoundaryCondition(), "Outdoors");
  };

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    SurfaceVector result = m.getConcreteModelObjects<Surface>();
    result.erase(std::remove_if(result.begin(), result.end(), isRoof), result.end());
    benchmark::DoNotOptimize(result);
  };

  state.SetComplexityN(state.range(0));
}

// (Note: Under heaver external load, 1024 takes 99 s, 2048 takes 475s (8 min))
// Normal load: 1024 takes 33s
BENCHMARK(Current)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Range(4, 2048)
  ->Complexity();

BENCHMARK(Proposed)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Range(4, 2048)
  ->Complexity();
