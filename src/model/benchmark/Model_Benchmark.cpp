#include <benchmark/benchmark.h>

#include "../Model.hpp"

#include "../BoilerHotWater.hpp"
#include "../ChillerElectricEIR.hpp"
#include "../CoilHeatingWater.hpp"
#include "../Node.hpp"
#include "../PlantLoop.hpp"
#include "../PumpVariableSpeed.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../SetpointManagerScheduled.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

#include <fmt/format.h>

//#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

// Call exampleModel
/*
 *static void BM_ExampleModel(benchmark::State& state) {
 *  // Code inside this loop is measured repeatedly
 *  for (auto _ : state) {
 *    openstudio::model::exampleModel();
 *  }
 *
 *  state.SetComplexityN(state.range(0));
 *}
 */

// Make N boilers
// Note: 4096 takes 8s
static void BM_AddObjects(benchmark::State& state) {

  Model m;

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (auto i = 0; i <= state.range(0); ++i) {
      BoilerHotWater{m};
    }
  }

  state.SetComplexityN(state.range(0));
}

static void BM_SetUpPlantLoop(benchmark::State& state) {

  Model m;
  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (auto i = 0; i <= state.range(0); ++i) {
      PlantLoop p(m);

      PumpVariableSpeed pump(m);
      Node supplyInletNode = p.supplyInletNode();
      pump.addToNode(supplyInletNode);

      Node supplyOutletNode = p.supplyOutletNode();
      ScheduleConstant tempSch(m);
      tempSch.setValue(40);
      SetpointManagerScheduled spm(m, tempSch);
      spm.addToNode(supplyOutletNode);

      BoilerHotWater b(m);
      p.addSupplyBranchForComponent(b);

      ChillerElectricEIR ch(m);
      p.addSupplyBranchForComponent(ch);

      for (int j = 0; j < 5; ++j) {
        CoilHeatingWater coil(m, alwaysOn);
        p.addDemandBranchForComponent(coil);
      }
    }
  }

  // fmt::print("Model has {} objects", m.numObjects()); // 167 objects per loop...
  state.SetComplexityN(state.range(0));
}

// Regular run, with n=512
/*
BENCHMARK(BM_WorkspaceSetNameWithChecks)->Unit(benchmark::kMillisecond)->Arg(512);
BENCHMARK(BM_WorkspaceSetNameWithoutAnyChecks)->Unit(benchmark::kMillisecond)->Arg(512);
*/

// Simple Runs
//BENCHMARK(BM_ExampleModel);

// With Complexity
BENCHMARK(BM_AddObjects)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(8, 4096)->Complexity();

// 128 takes 14secs,  512 takes about 300 seconds, 1024 takes 20 minutes. By interpolation, 4096 would take 636 minutes, 8192 = 2567 minutes = 42 h
// 'y[ms] = 1.156580334046908*x**2 + -72.31709114930806*x + 1397.3555792110117'
BENCHMARK(BM_SetUpPlantLoop)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(1, 128)->Complexity();
