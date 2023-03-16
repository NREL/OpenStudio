/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/
#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"
#include "../../model/SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/GeneratorPhotovoltaic.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterInverterSimple.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Model.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>
#include <utilities/idd/SolarCollector_FlatPlate_PhotovoltaicThermal_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SolarCollectorPerformancePhotovoltaicThermalBIPVT) {

  ForwardTranslator ft;

  Model m;

  SolarCollectorPerformancePhotovoltaicThermalBIPVT performance(m);
  performance.setName("BIPVT Perf");
  SolarCollectorFlatPlatePhotovoltaicThermal pvt(performance);

  PlantLoop p(m);
  p.addSupplyBranchForComponent(pvt);

  // Make a ShadingSurface, and a ShadingSurfaceGroup as it's needed
  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::equivalentOneDiode(m);
  panel.setHeatTransferIntegrationMode("PhotovoltaicThermalSolarCollector");
  panel.setNumberOfModulesInParallel(12);
  panel.setNumberOfModulesInSeries(8);
  panel.setRatedElectricPowerOutput(9000);
  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  panel.setAvailabilitySchedule(alwaysOn);

  ElectricLoadCenterInverterSimple inverter(m);
  ElectricLoadCenterDistribution elcd(m);
  EXPECT_TRUE(elcd.addGenerator(panel));
  EXPECT_TRUE(elcd.setInverter(inverter));

  constexpr double width = 10.0;
  constexpr double height = 3.0;

  // Counterclockwise points
  const std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space = Space::fromFloorPrint(floorPointsSpace1, height, m).get();

  auto roof = [&space]() -> Surface {
    auto sfs = space.surfaces();
    auto it = std::find_if(sfs.cbegin(), sfs.cend(), [](const auto& s) { return s.surfaceType() == "RoofCeiling"; });
    OS_ASSERT(it != sfs.cend());
    return *it;
  }();

  EXPECT_TRUE(panel.setSurface(roof));
  EXPECT_TRUE(pvt.setSurface(roof));
  EXPECT_TRUE(pvt.setGeneratorPhotovoltaic(panel));
  EXPECT_TRUE(roof.setSurfacePropertyOtherSideConditionsModel(performance.boundaryConditionsModel()));

  ThermalZone z(m);
  EXPECT_TRUE(space.setThermalZone(z));

  EXPECT_TRUE(performance.setEffectivePlenumGapThicknessBehindPVModules(0.5));

  SurfacePropertyOtherSideConditionsModel oscm = performance.boundaryConditionsModel();
  oscm.setName("BIPVT OtherSideConditionsModel");

  ScheduleConstant availSch(m);
  availSch.setName("BIPV AvailabilitySchedule");

  EXPECT_TRUE(performance.setEffectivePlenumGapThicknessBehindPVModules(0.5));
  EXPECT_TRUE(performance.setPVCellNormalTransmittanceAbsorptanceProduct(0.81));
  EXPECT_TRUE(performance.setBackingMaterialNormalTransmittanceAbsorptanceProduct(0.82));
  EXPECT_TRUE(performance.setCladdingNormalTransmittanceAbsorptanceProduct(0.83));
  EXPECT_TRUE(performance.setFractionofCollectorGrossAreaCoveredbyPVModule(0.84));
  EXPECT_TRUE(performance.setFractionofPVCellAreatoPVModuleArea(0.75));
  EXPECT_TRUE(performance.setPVModuleTopThermalResistance(0.0054));
  EXPECT_TRUE(performance.setPVModuleBottomThermalResistance(0.0049));
  EXPECT_TRUE(performance.setPVModuleFrontLongwaveEmissivity(0.89));
  EXPECT_TRUE(performance.setPVModuleBackLongwaveEmissivity(0.92));
  EXPECT_TRUE(performance.setGlassThickness(0.009));
  EXPECT_TRUE(performance.setGlassRefractionIndex(1.586));
  EXPECT_TRUE(performance.setGlassExtinctionCoefficient(5.0));

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_BIPVT);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ("BIPVT Perf", idfObject.getString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::Name).get());
  EXPECT_EQ("BIPVT OtherSideConditionsModel",
            idfObject.getString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName).get());
  EXPECT_EQ("Always On Discrete", idfObject.getString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName).get());
  EXPECT_EQ(0.5, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules).get());
  EXPECT_EQ(0.81, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct).get());
  EXPECT_EQ(
    0.82, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct).get());
  EXPECT_EQ(0.83,
            idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct).get());
  EXPECT_EQ(0.84,
            idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule).get());
  EXPECT_EQ(0.75, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea).get());
  EXPECT_EQ(0.0054, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance).get());
  EXPECT_EQ(0.0049, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance).get());
  EXPECT_EQ(0.89, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity).get());
  EXPECT_EQ(0.92, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity).get());
  EXPECT_EQ(0.009, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness).get());
  EXPECT_EQ(1.586, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex).get());
  EXPECT_EQ(5.0, idfObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient).get());

  const auto pvtObjs = w.getObjectsByType(IddObjectType::SolarCollector_FlatPlate_PhotovoltaicThermal);
  ASSERT_EQ(1u, pvtObjs.size());
  const auto& pvtObj = pvtObjs.front();
  EXPECT_EQ("BIPVT Perf", pvtObj.getString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SolarCollectorPerformancePhotovoltaicThermalBIPVT) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto woPerf = w.addObject(IdfObject(IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_BIPVT)).get();
  woPerf.setName("BIPVT Perf");

  auto woBoundaryCond = w.addObject(IdfObject(IddObjectType::SurfaceProperty_OtherSideConditionsModel)).get();
  woBoundaryCond.setName("BIPVT OtherSideConditionsModel");
  EXPECT_TRUE(woPerf.setPointer(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName, woBoundaryCond.handle()));

  auto woAvailSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  woAvailSch.setName("BIPVT AvailabilitySchedule");
  EXPECT_TRUE(woPerf.setPointer(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName, woAvailSch.handle()));

  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules, 0.5));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct, 0.81));
  EXPECT_TRUE(
    woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct, 0.82));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct, 0.83));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule, 0.84));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea, 0.75));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance, 0.0054));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance, 0.0049));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity, 0.89));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity, 0.92));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness, 0.009));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex, 1.586));
  EXPECT_TRUE(woPerf.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient, 5.0));

  const Model m = rt.translateWorkspace(w);

  const auto perfs = m.getConcreteModelObjects<SolarCollectorPerformancePhotovoltaicThermalBIPVT>();
  ASSERT_EQ(1, perfs.size());
  const auto& perf = perfs.front();

  EXPECT_EQ("BIPVT Perf", perf.nameString());

  ASSERT_EQ(1, m.getConcreteModelObjects<SurfacePropertyOtherSideConditionsModel>().size());
  ASSERT_TRUE(perf.boundaryConditionsModel().optionalCast<SurfacePropertyOtherSideConditionsModel>());
  EXPECT_EQ("BIPVT OtherSideConditionsModel", perf.boundaryConditionsModel().nameString());

  ASSERT_TRUE(perf.availabilitySchedule().optionalCast<ScheduleConstant>());
  EXPECT_EQ("BIPVT AvailabilitySchedule", perf.availabilitySchedule().nameString());

  EXPECT_EQ(0.5, perf.effectivePlenumGapThicknessBehindPVModules());
  EXPECT_EQ(0.81, perf.pVCellNormalTransmittanceAbsorptanceProduct());
  EXPECT_EQ(0.82, perf.backingMaterialNormalTransmittanceAbsorptanceProduct());
  EXPECT_EQ(0.83, perf.claddingNormalTransmittanceAbsorptanceProduct());
  EXPECT_EQ(0.84, perf.fractionofCollectorGrossAreaCoveredbyPVModule());
  EXPECT_EQ(0.75, perf.fractionofPVCellAreatoPVModuleArea());
  EXPECT_EQ(0.0054, perf.pVModuleTopThermalResistance());
  EXPECT_EQ(0.0049, perf.pVModuleBottomThermalResistance());
  EXPECT_EQ(0.89, perf.pVModuleFrontLongwaveEmissivity());
  EXPECT_EQ(0.92, perf.pVModuleBackLongwaveEmissivity());
  EXPECT_EQ(0.009, perf.glassThickness());
  EXPECT_EQ(1.586, perf.glassRefractionIndex());
  EXPECT_EQ(5.0, perf.glassExtinctionCoefficient());
}
