#include <benchmark/benchmark.h>

#include "../WorkspaceExtensibleGroup.hpp"
#include "../IdfFile.hpp"
#include "../IdfObject.hpp"
#include "../Workspace.hpp"
#include "../WorkspaceObject.hpp"
#include "../WorkspaceObject_Impl.hpp"
#include "../ValidityEnums.hpp"
#include "../../core/Enum.hpp"
#include "../../core/Optional.hpp"

#include "../../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

//#include <iostream>

using namespace openstudio;

// Create a Workspace with 2 objects of every object type that has a name + N spaces
Workspace setUpWorkspaceWithNObjectsOfEveryType(size_t n) {
  Workspace w(StrictnessLevel::Draft, IddFileType::OpenStudio);

  auto osIddFile = openstudio::IddFactory::instance().getIddFile(openstudio::IddFileType::OpenStudio);

  for (const IddObject& iddObject : osIddFile.objects()) {
    auto iddObjectType = iddObject.type();
    if (auto index_ = iddObject.nameFieldIndex()) {
      for (size_t i = 0; i < 2; ++i) {
        auto obj = w.addObject(IdfObject(iddObjectType)).get();
      }
    }
  }

  // And we add N Spaces
  for (size_t i = 0; i < n; ++i) {
    w.addObject(IdfObject(IddObjectType::OS_Space)).get();
  }

  // std::cout << "Workspace has " << w.objects().size() << " objects" << '\n';

  return w;
}

Workspace setUpMinimalWorkspace(size_t n) {

  Workspace w(StrictnessLevel::Draft, IddFileType::OpenStudio);
  for (size_t i = 0; i < n; ++i) {
    w.addObject(IdfObject(IddObjectType::OS_Space)).get();
  }

  return w;
}

static void BM_WorkspaceSetNameWithChecks(benchmark::State& state) {
  Workspace w = setUpWorkspaceWithNObjectsOfEveryType(state.range(0));

  bool checkValidity = true;

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (auto& obj : w.getObjectsByType(IddObjectType::OS_Space)) {
      obj.getImpl<openstudio::detail::WorkspaceObject_Impl>()->setName("my object", checkValidity);
    }
  }

  state.SetComplexityN(state.range(0));
}

// Not even checking Validity...
static void BM_WorkspaceSetNameWithoutAnyChecks(benchmark::State& state) {
  Workspace w = setUpWorkspaceWithNObjectsOfEveryType(state.range(0));

  bool checkValidity = false;

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (auto& obj : w.getObjectsByType(IddObjectType::OS_Space)) {
      obj.getImpl<openstudio::detail::WorkspaceObject_Impl>()->setName("my object", checkValidity);
    }
  }

  state.SetComplexityN(state.range(0));
}

// Regular run, with n=512
/*
BENCHMARK(BM_WorkspaceSetNameWithChecks)->Unit(benchmark::kMillisecond)->Arg(512);
BENCHMARK(BM_WorkspaceSetNameWithoutAnyChecks)->Unit(benchmark::kMillisecond)->Arg(512);
*/

// Calculate complexity
BENCHMARK(BM_WorkspaceSetNameWithChecks)
  //->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(8)
  ->Range(2, 2048)
  ->Complexity();

BENCHMARK(BM_WorkspaceSetNameWithoutAnyChecks)
  // ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(8)
  ->Range(2, 2048)
  ->Complexity();
