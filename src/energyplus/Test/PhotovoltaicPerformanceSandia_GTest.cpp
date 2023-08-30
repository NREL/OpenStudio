/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/PhotovoltaicPerformanceSandia.hpp"
#include "../../model/PhotovoltaicPerformanceSandia_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorPhotovoltaic.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterInverterSimple.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/PhotovoltaicPerformance_Sandia_FieldEnums.hxx>
#include <utilities/idd/Generator_Photovoltaic_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Generators_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PhotovoltaicPerformanceSandia) {

  ForwardTranslator ft;

  Model m;

  auto perfNames = PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames();
  ASSERT_GT(perfNames.size(), 5);
  auto perfName = perfNames[5];

  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::fromSandiaDatabase(m, perfName);
  auto sandiaPerf = panel.photovoltaicPerformance().cast<PhotovoltaicPerformanceSandia>();
  panel.setNumberOfModulesInParallel(3);
  panel.setNumberOfModulesInSeries(6);
  panel.setRatedElectricPowerOutput(20000);
  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  panel.setAvailabilitySchedule(alwaysOn);

  // Make a ShadingSurface, and a ShadingSurfaceGroup as it's needed
  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  ShadingSurface shadingSurface(points, m);

  ShadingSurfaceGroup shadingSurfaceGroup(m);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  EXPECT_TRUE(panel.setSurface(shadingSurface));
  EXPECT_TRUE(panel.surface());

  // Create an ELCD
  ElectricLoadCenterDistribution elcd(m);
  elcd.setName("PV ELCD");
  elcd.addGenerator(panel);
  elcd.setGeneratorOperationSchemeType("Baseload");
  elcd.setDemandLimitSchemePurchasedElectricDemandLimit(0.0);

  ElectricLoadCenterInverterSimple inverter(m);
  elcd.setElectricalBussType("DirectCurrentWithInverter");
  elcd.setInverter(inverter);

  inverter.setAvailabilitySchedule(alwaysOn);
  inverter.setRadiativeFraction(0.0);
  inverter.setInverterEfficiency(1.0);

  // Forward Translate
  Workspace w = ft.translateModel(m);

  // Get the ELCD
  WorkspaceObjectVector idf_elcds(w.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution));
  ASSERT_EQ(1u, idf_elcds.size());
  WorkspaceObject idf_elcd(idf_elcds[0]);

  // Get its Generator list
  WorkspaceObject idf_genlist = idf_elcd.getTarget(ElectricLoadCenter_DistributionFields::GeneratorListName).get();

  // There should be only one generator
  ASSERT_EQ(1u, idf_genlist.extensibleGroups().size());
  auto w_eg_gen = idf_genlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

  EXPECT_EQ("Generator:Photovoltaic", w_eg_gen.getString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorObjectType).get());
  ASSERT_EQ(w_eg_gen.getString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorName).get(), panel.nameString());

  // Get the Panel
  ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::Generator_Photovoltaic).size());
  WorkspaceObject idf_panel = w_eg_gen.getTarget(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorName).get();

  // Get the PV Perf object
  ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::PhotovoltaicPerformance_Sandia).size());
  EXPECT_EQ("PhotovoltaicPerformance:Sandia", idf_panel.getString(Generator_PhotovoltaicFields::PhotovoltaicPerformanceObjectType).get());
  ASSERT_TRUE(idf_panel.getTarget(Generator_PhotovoltaicFields::ModulePerformanceName));
  WorkspaceObject idf_perf = idf_panel.getTarget(Generator_PhotovoltaicFields::ModulePerformanceName).get();

  EXPECT_EQ(sandiaPerf.activeArea(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::ActiveArea).get());

  EXPECT_EQ(sandiaPerf.numberofCellsinSeries(), idf_perf.getInt(PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries).get());
  EXPECT_EQ(sandiaPerf.numberofCellsinParallel(), idf_perf.getInt(PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel).get());

  EXPECT_EQ(sandiaPerf.shortCircuitCurrent(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent).get());
  EXPECT_EQ(sandiaPerf.openCircuitVoltage(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage).get());
  EXPECT_EQ(sandiaPerf.currentatMaximumPowerPoint(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint).get());
  EXPECT_EQ(sandiaPerf.voltageatMaximumPowerPoint(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameteraIsc(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameteraImp(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc1(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterBVoc0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametermBVoc(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterBVmp0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametermBVmp(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp).get());
  EXPECT_EQ(sandiaPerf.diodeFactor(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::DiodeFactor).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc2(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc3(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametera0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametera1(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametera2(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametera3(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametera4(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb1(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb2(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb3(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb4(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb5(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterDeltaTc(),
            idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterfd(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParametera(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterb(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc4(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc5(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterIx0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterIxx0(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc6(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6).get());
  EXPECT_EQ(sandiaPerf.sandiaDatabaseParameterc7(), idf_perf.getDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7).get());
}

// The Forward Translator should not translate PerfObject if it's not linked a Generator:Photovoltaic
TEST_F(EnergyPlusFixture, ForwardTranslator_PhotovoltaicPerformanceSandia_NotUsed) {

  Model m;

  auto perf = PhotovoltaicPerformanceSandia(m);

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Not linked to a Generator:Phovoltaic => not translated

  WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::PhotovoltaicPerformance_Sandia));
  ASSERT_EQ(0u, idf_perfs.size());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_PhotovoltaicPerformanceSandia) {

  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_perf = w.addObject(IdfObject(IddObjectType::PhotovoltaicPerformance_Sandia));
  ASSERT_TRUE(_i_perf);
  _i_perf->setName("Aleo S16 165 [2007 (E)]");

  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::ActiveArea, 1.378));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries, 50.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel, 1.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent, 7.9));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage, 30.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint, 7.08));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint, 23.3));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc, 0.0008));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp, -0.0003));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0, 0.99));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1, 0.01));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0, -0.11));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc, 0.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0, -0.115));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp, 0.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::DiodeFactor, 1.35));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2, -0.12));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3, -11.08));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0, 0.924));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1, 0.06749));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2, -0.012549));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3, 0.0010049));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4, -2.8797e-05));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0, 1.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1, -0.002438));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2, 0.0003103));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3, -1.246e-05));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4, 2.11e-07));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5, -1.36e-09));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_, 3.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd, 1.0));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera, -3.56));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb, -0.075));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4, 0.995));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5, 0.005));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0, 7.8));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0, 4.92));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6, 1.15));
  EXPECT_TRUE(_i_perf->setDouble(PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7, -0.15));

  // PhotovoltaicPerformance:Sandia,
  //   Aleo S16 165 [2007 (E)],                !- Name
  //   1.378,                                  !- Active Area {m2}
  //   50,                                     !- Number of Cells in Series {dimensionless}
  //   1,                                      !- Number of Cells in Parallel {dimensionless}
  //   7.9,                                    !- Short Circuit Current {A}
  //   30,                                     !- Open Circuit Voltage {V}
  //   7.08,                                   !- Current at Maximum Power Point {A}
  //   23.3,                                   !- Voltage at Maximum Power Point {V}
  //   0.0008,                                 !- Sandia Database Parameter aIsc {1/K}
  //   -0.0003,                                !- Sandia Database Parameter aImp {1/K}
  //   0.99,                                   !- Sandia Database Parameter c0 {dimensionless}
  //   0.01,                                   !- Sandia Database Parameter c1 {dimensionless}
  //   -0.11,                                  !- Sandia Database Parameter BVoc0 {V/K}
  //   0,                                      !- Sandia Database Parameter mBVoc {V/K}
  //   -0.115,                                 !- Sandia Database Parameter BVmp0 {V/K}
  //   0,                                      !- Sandia Database Parameter mBVmp {V/K}
  //   1.35,                                   !- Diode Factor {dimensionless}
  //   -0.12,                                  !- Sandia Database Parameter c2 {dimensionless}
  //   -11.08,                                 !- Sandia Database Parameter c3 {dimensionless}
  //   0.924,                                  !- Sandia Database Parameter a0 {dimensionless}
  //   0.06749,                                !- Sandia Database Parameter a1 {dimensionless}
  //   -0.012549,                              !- Sandia Database Parameter a2 {dimensionless}
  //   0.0010049,                              !- Sandia Database Parameter a3 {dimensionless}
  //   -2.8797e-05,                            !- Sandia Database Parameter a4 {dimensionless}
  //   1,                                      !- Sandia Database Parameter b0 {dimensionless}
  //   -0.002438,                              !- Sandia Database Parameter b1 {dimensionless}
  //   0.0003103,                              !- Sandia Database Parameter b2 {dimensionless}
  //   -1.246e-05,                             !- Sandia Database Parameter b3 {dimensionless}
  //   2.11e-07,                               !- Sandia Database Parameter b4 {dimensionless}
  //   -1.36e-09,                              !- Sandia Database Parameter b5 {dimensionless}
  //   3,                                      !- Sandia Database Parameter Delta(Tc) {deltaC}
  //   1,                                      !- Sandia Database Parameter fd {dimensionless}
  //   -3.56,                                  !- Sandia Database Parameter a {dimensionless}
  //   -0.075,                                 !- Sandia Database Parameter b {dimensionless}
  //   0.995,                                  !- Sandia Database Parameter c4 {dimensionless}
  //   0.005,                                  !- Sandia Database Parameter c5 {dimensionless}
  //   7.8,                                    !- Sandia Database Parameter Ix0
  //   4.92,                                   !- Sandia Database Parameter Ixx0
  //   1.15,                                   !- Sandia Database Parameter c6 {dimensionless}
  //   -0.15;                                  !- Sandia Database Parameter c7 {dimensionless}

  // To avoid other warnings, we add required objects
  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
  Model model = reverseTranslator.translateWorkspace(w);
  EXPECT_TRUE(reverseTranslator.errors().empty());
  EXPECT_TRUE(reverseTranslator.warnings().empty());

  auto sandiaPerfs = model.getConcreteModelObjects<openstudio::model::PhotovoltaicPerformanceSandia>();
  ASSERT_EQ(1u, sandiaPerfs.size());
  PhotovoltaicPerformanceSandia sandiaPerf = sandiaPerfs[0];

  EXPECT_EQ(1.378, sandiaPerf.activeArea());
  EXPECT_EQ(50.0, sandiaPerf.numberofCellsinSeries());
  EXPECT_EQ(1.0, sandiaPerf.numberofCellsinParallel());
  EXPECT_EQ(7.9, sandiaPerf.shortCircuitCurrent());
  EXPECT_EQ(30.0, sandiaPerf.openCircuitVoltage());
  EXPECT_EQ(7.08, sandiaPerf.currentatMaximumPowerPoint());
  EXPECT_EQ(23.3, sandiaPerf.voltageatMaximumPowerPoint());
  EXPECT_EQ(0.0008, sandiaPerf.sandiaDatabaseParameteraIsc());
  EXPECT_EQ(-0.0003, sandiaPerf.sandiaDatabaseParameteraImp());
  EXPECT_EQ(0.99, sandiaPerf.sandiaDatabaseParameterc0());
  EXPECT_EQ(0.01, sandiaPerf.sandiaDatabaseParameterc1());
  EXPECT_EQ(-0.11, sandiaPerf.sandiaDatabaseParameterBVoc0());
  EXPECT_EQ(0.0, sandiaPerf.sandiaDatabaseParametermBVoc());
  EXPECT_EQ(-0.115, sandiaPerf.sandiaDatabaseParameterBVmp0());
  EXPECT_EQ(0.0, sandiaPerf.sandiaDatabaseParametermBVmp());
  EXPECT_EQ(1.35, sandiaPerf.diodeFactor());
  EXPECT_EQ(-0.12, sandiaPerf.sandiaDatabaseParameterc2());
  EXPECT_EQ(-11.08, sandiaPerf.sandiaDatabaseParameterc3());
  EXPECT_EQ(0.924, sandiaPerf.sandiaDatabaseParametera0());
  EXPECT_EQ(0.06749, sandiaPerf.sandiaDatabaseParametera1());
  EXPECT_EQ(-0.012549, sandiaPerf.sandiaDatabaseParametera2());
  EXPECT_EQ(0.0010049, sandiaPerf.sandiaDatabaseParametera3());
  EXPECT_EQ(-2.8797e-05, sandiaPerf.sandiaDatabaseParametera4());
  EXPECT_EQ(1.0, sandiaPerf.sandiaDatabaseParameterb0());
  EXPECT_EQ(-0.002438, sandiaPerf.sandiaDatabaseParameterb1());
  EXPECT_EQ(0.0003103, sandiaPerf.sandiaDatabaseParameterb2());
  EXPECT_EQ(-1.246e-05, sandiaPerf.sandiaDatabaseParameterb3());
  EXPECT_EQ(2.11e-07, sandiaPerf.sandiaDatabaseParameterb4());
  EXPECT_EQ(-1.36e-09, sandiaPerf.sandiaDatabaseParameterb5());
  EXPECT_EQ(3.0, sandiaPerf.sandiaDatabaseParameterDeltaTc());
  EXPECT_EQ(1.0, sandiaPerf.sandiaDatabaseParameterfd());
  EXPECT_EQ(-3.56, sandiaPerf.sandiaDatabaseParametera());
  EXPECT_EQ(-0.075, sandiaPerf.sandiaDatabaseParameterb());
  EXPECT_EQ(0.995, sandiaPerf.sandiaDatabaseParameterc4());
  EXPECT_EQ(0.005, sandiaPerf.sandiaDatabaseParameterc5());
  EXPECT_EQ(7.8, sandiaPerf.sandiaDatabaseParameterIx0());
  EXPECT_EQ(4.92, sandiaPerf.sandiaDatabaseParameterIxx0());
  EXPECT_EQ(1.15, sandiaPerf.sandiaDatabaseParameterc6());
  EXPECT_EQ(-0.15, sandiaPerf.sandiaDatabaseParameterc7());
}
