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
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/core/Assert.hpp"

using namespace openstudio;
using namespace openstudio::model;

model::Model makeModelWithNSpacesInSameZone(size_t nSpaces) {

  Model m;

  ThermalZone z(m);

  constexpr double floorHeight = 2.0;

  double zOrigin = 0.0;
  for (size_t i = 0; i < nSpaces; ++i) {

    Point3dVector pts{{0, 0, zOrigin}, {0, 1, zOrigin}, {1, 1, zOrigin}, {1, 0, zOrigin}};
    auto space_ = Space::fromFloorPrint(pts, floorHeight, m);
    OS_ASSERT(space_);
    space_->setThermalZone(z);
    // Infiltration
    {
      SpaceInfiltrationDesignFlowRate infiltration(m);
      if (i % 2 == 0) {
        infiltration.setAirChangesperHour(1.0);
      } else {
        infiltration.setFlowperSpaceFloorArea(10.0);
      }
      infiltration.setSpace(space_.get());
    }
    zOrigin += floorHeight;
  }

  OS_ASSERT(m.getConcreteModelObjects<Space>().size() == nSpaces);

  return m;
}

static void BM_CombineSpaces(benchmark::State& state) {

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    // Pausing/Resuming has quite the overhead in the initial setup of the benchmark run...
    state.PauseTiming();
    Model m = makeModelWithNSpacesInSameZone(state.range(0));
    ThermalZone z = m.getConcreteModelObjects<ThermalZone>()[0];
    OS_ASSERT(m.getConcreteModelObjects<Space>().size() == (size_t)state.range(0));
    state.ResumeTiming();

    auto space_ = z.combineSpaces();
    benchmark::DoNotOptimize(space_);
  };

  state.SetComplexityN(state.range(0));
}

// (Note: Under heaver external load, 1024 takes 99 s, 2048 takes 475s (8 min))
// Normal load: 1024 takes 33s
BENCHMARK(BM_CombineSpaces)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(1, 256)->Complexity();

model::Model makeModelWithNSpacesInNZones(size_t nSpaces) {

  Model m;

  constexpr size_t nSpacesPerZone = 4;
  boost::optional<ThermalZone> z;

  constexpr double floorHeight = 2.0;

  double zOrigin = 0.0;
  for (size_t i = 0; i < nSpaces; ++i) {

    Point3dVector pts{{0, 0, zOrigin}, {0, 1, zOrigin}, {1, 1, zOrigin}, {1, 0, zOrigin}};
    auto space_ = Space::fromFloorPrint(pts, floorHeight, m);
    OS_ASSERT(space_);

    if (i % nSpacesPerZone == 0) {
      z = ThermalZone(m);
    }
    space_->setThermalZone(*z);
    // Infiltration
    {
      SpaceInfiltrationDesignFlowRate infiltration(m);
      if (i % 2 == 0) {
        infiltration.setAirChangesperHour(1.0);
      } else {
        infiltration.setFlowperSpaceFloorArea(10.0);
      }
      infiltration.setSpace(space_.get());
    }
    zOrigin += floorHeight;
  }

  OS_ASSERT(m.getConcreteModelObjects<Space>().size() == nSpaces);
  OS_ASSERT(m.getConcreteModelObjects<ThermalZone>().size() == (nSpaces / nSpacesPerZone));
  return m;
}

static void BM_CombineSpacesInNZones(benchmark::State& state) {

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    // Pausing/Resuming has quite the overhead in the initial setup of the benchmark run...
    state.PauseTiming();
    Model m = makeModelWithNSpacesInNZones(state.range(0));
    state.ResumeTiming();

    for (auto& z : m.getConcreteModelObjects<ThermalZone>()) {
      auto space_ = z.combineSpaces();
      benchmark::DoNotOptimize(space_);
    }
  };

  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_CombineSpacesInNZones)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(4, 256)->Complexity();
