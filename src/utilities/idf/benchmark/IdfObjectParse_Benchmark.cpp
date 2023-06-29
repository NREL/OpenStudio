/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <benchmark/benchmark.h>

#include "../IdfObject.hpp"

#include <string>

using namespace openstudio;

static void BM_ParseAirLoopHVAC(benchmark::State& state) {
  std::string text = R"(OS:AirLoopHVAC,
  {69a1735d-a314-4692-9729-4b7825fc62fa}, !- Handle
  Air Loop HVAC 1,                        !- Name
  ,                                       !- Controller List Name
  {f0779a92-1cb7-449a-b2b6-1218bbd0cbeb}, !- Availability Schedule
  {6a3cce5b-d2b4-4ccc-a9f6-7ad30df37d79}, !- Availability Manager List Name
  autosize,                               !- Design Supply Air Flow Rate {m3/s}
  1,                                      !- Design Return Air Flow Fraction of Supply Air Flow
  ,                                       !- Branch List Name
  ,                                       !- Connector List Name
  {ea12736a-b4e0-4c39-b4de-9d937abf305f}, !- Supply Side Inlet Node Name
  {e8181d11-ed1a-4ca5-85a2-1ebb97b7638a}, !- Demand Side Outlet Node Name
  {0c522174-c647-4b6a-8f6b-3c85b284e71c}, !- Demand Side Inlet Node A
  {34529c8e-855b-4ade-8020-240f2f51ebb8}, !- Supply Side Outlet Node A
  ,                                       !- Demand Side Inlet Node B
  ,                                       !- Supply Side Outlet Node B
  ,                                       !- Return Air Bypass Flow Temperature Setpoint Schedule Name
  {6dc9e278-bcd4-4eae-88e0-0a6854755e34}, !- Demand Mixer Name
  {b1c4ef87-39b9-4c2c-8fef-0c054aed09b9}, !- Demand Splitter A Name
  ,                                       !- Demand Splitter B Name
  ;                                       !- Supply Splitter Name)";

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto idfObject = IdfObject::load(text);
  }
};

BENCHMARK(BM_ParseAirLoopHVAC);
