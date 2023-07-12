/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <boost/algorithm/string/predicate.hpp>
#include <resources.hxx>
#include <sstream>
#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SizingSystem) {

  Model m;

  AirLoopHVAC airLoopHVAC(m);
  airLoopHVAC.setName("Air Loop HVAC");

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  WorkspaceObjectVector idf_sss(w.getObjectsByType(IddObjectType::Sizing_System));
  EXPECT_EQ(1u, idf_sss.size());
  WorkspaceObject idf_ss(idf_sss[0]);

  EXPECT_EQ(airLoopHVAC.nameString(), idf_ss.getString(Sizing_SystemFields::AirLoopName, false).get());
  EXPECT_EQ("Sensible", idf_ss.getString(Sizing_SystemFields::TypeofLoadtoSizeOn, false).get());
  EXPECT_EQ("Autosize", idf_ss.getString(Sizing_SystemFields::DesignOutdoorAirFlowRate, false).get());
  EXPECT_EQ(0.3, idf_ss.getDouble(Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, false).get());
  EXPECT_EQ(7.0, idf_ss.getDouble(Sizing_SystemFields::PreheatDesignTemperature, false).get());
  EXPECT_EQ(0.008, idf_ss.getDouble(Sizing_SystemFields::PreheatDesignHumidityRatio, false).get());
  EXPECT_EQ(12.8, idf_ss.getDouble(Sizing_SystemFields::PrecoolDesignTemperature, false).get());
  EXPECT_EQ(0.008, idf_ss.getDouble(Sizing_SystemFields::PrecoolDesignHumidityRatio, false).get());
  EXPECT_EQ(12.8, idf_ss.getDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature, false).get());
  EXPECT_EQ(16.7, idf_ss.getDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature, false).get());
  EXPECT_EQ("NonCoincident", idf_ss.getString(Sizing_SystemFields::TypeofZoneSumtoUse, false).get());
  EXPECT_EQ("Yes", idf_ss.getString(Sizing_SystemFields::AllOutdoorAirinCooling, false).get());
  EXPECT_EQ("Yes", idf_ss.getString(Sizing_SystemFields::AllOutdoorAirinHeating, false).get());
  EXPECT_EQ(0.0085, idf_ss.getDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio, false).get());
  EXPECT_EQ(0.008, idf_ss.getDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio, false).get());
  EXPECT_EQ("DesignDay", idf_ss.getString(Sizing_SystemFields::CoolingSupplyAirFlowRateMethod, false).get());
  EXPECT_EQ(0.0, idf_ss.getDouble(Sizing_SystemFields::CoolingSupplyAirFlowRate, false).get());
  EXPECT_EQ("DesignDay", idf_ss.getString(Sizing_SystemFields::HeatingSupplyAirFlowRateMethod, false).get());
  EXPECT_EQ(0.0, idf_ss.getDouble(Sizing_SystemFields::HeatingSupplyAirFlowRate, false).get());
  EXPECT_EQ("ZoneSum", idf_ss.getString(Sizing_SystemFields::SystemOutdoorAirMethod, false).get());
  EXPECT_EQ(1.0, idf_ss.getDouble(Sizing_SystemFields::ZoneMaximumOutdoorAirFraction, false).get());
  EXPECT_EQ(9.9676501E-3, idf_ss.getDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerFloorArea, false).get());
  EXPECT_EQ(1.0, idf_ss.getDouble(Sizing_SystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate, false).get());
  EXPECT_EQ(3.9475456E-5, idf_ss.getDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerUnitCoolingCapacity, false).get());
  EXPECT_EQ(9.9676501E-3, idf_ss.getDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerFloorArea, false).get());
  EXPECT_EQ(1.0, idf_ss.getDouble(Sizing_SystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate, false).get());
  EXPECT_EQ(1.0, idf_ss.getDouble(Sizing_SystemFields::HeatingFractionofAutosizedCoolingSupplyAirFlowRate, false).get());
  EXPECT_EQ(3.1588213E-5, idf_ss.getDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerUnitHeatingCapacity, false).get());
  EXPECT_EQ("CoolingDesignCapacity", idf_ss.getString(Sizing_SystemFields::CoolingDesignCapacityMethod, false).get());
  EXPECT_EQ("Autosize", idf_ss.getString(Sizing_SystemFields::CoolingDesignCapacity, false).get());
  EXPECT_EQ(234.7, idf_ss.getDouble(Sizing_SystemFields::CoolingDesignCapacityPerFloorArea, false).get());
  EXPECT_EQ(1.0, idf_ss.getDouble(Sizing_SystemFields::FractionofAutosizedCoolingDesignCapacity, false).get());
  EXPECT_EQ("HeatingDesignCapacity", idf_ss.getString(Sizing_SystemFields::HeatingDesignCapacityMethod, false).get());
  EXPECT_EQ("Autosize", idf_ss.getString(Sizing_SystemFields::HeatingDesignCapacity, false).get());
  EXPECT_EQ(157.0, idf_ss.getDouble(Sizing_SystemFields::HeatingDesignCapacityPerFloorArea, false).get());
  EXPECT_EQ(1.0, idf_ss.getDouble(Sizing_SystemFields::FractionofAutosizedHeatingDesignCapacity, false).get());
  EXPECT_EQ("OnOff", idf_ss.getString(Sizing_SystemFields::CentralCoolingCapacityControlMethod, false).get());
  EXPECT_EQ("Autosize", idf_ss.getString(Sizing_SystemFields::OccupantDiversity, false).get());
}

TEST_F(EnergyPlusFixture, DISABLED_ReverseTranslator_SizingSystem) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  // air loop hvac
  openstudio::IdfObject idfObject1(openstudio::IddObjectType::AirLoopHVAC);

  openstudio::WorkspaceObject epAirLoopHVAC = workspace.addObject(idfObject1).get();
  idfObject1.setString(AirLoopHVACFields::Name, "Air Loop HVAC");

  // sizing system
  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Sizing_System);
  idfObject2.setString(Sizing_SystemFields::AirLoopName, "Air Loop HVAC");
  idfObject2.setString(Sizing_SystemFields::TypeofLoadtoSizeOn, "Sensible");
  idfObject2.setString(Sizing_SystemFields::DesignOutdoorAirFlowRate, "Autosize");
  idfObject2.setString(Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, "Autosize");
  idfObject2.setDouble(Sizing_SystemFields::PreheatDesignTemperature, 7.0);
  idfObject2.setDouble(Sizing_SystemFields::PreheatDesignHumidityRatio, 0.008);
  idfObject2.setDouble(Sizing_SystemFields::PrecoolDesignTemperature, 12.8);
  idfObject2.setDouble(Sizing_SystemFields::PrecoolDesignHumidityRatio, 0.008);
  idfObject2.setDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature, 12.8);
  idfObject2.setDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature, 16.7);
  idfObject2.setString(Sizing_SystemFields::TypeofZoneSumtoUse, "NonCoincident");
  idfObject2.setString(Sizing_SystemFields::AllOutdoorAirinCooling, "No");
  idfObject2.setString(Sizing_SystemFields::AllOutdoorAirinHeating, "No");
  idfObject2.setDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio, 0.0085);
  idfObject2.setDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio, 0.008);
  idfObject2.setString(Sizing_SystemFields::CoolingSupplyAirFlowRateMethod, "DesignDay");
  idfObject2.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRate, 0.0);
  idfObject2.setString(Sizing_SystemFields::HeatingSupplyAirFlowRateMethod, "DesignDay");
  idfObject2.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRate, 0.0);
  idfObject2.setString(Sizing_SystemFields::SystemOutdoorAirMethod, "ZoneSum");
  idfObject2.setDouble(Sizing_SystemFields::ZoneMaximumOutdoorAirFraction, 1.0);
  idfObject2.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerFloorArea, 9.9676501E-3);
  idfObject2.setDouble(Sizing_SystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate, 1.0);
  idfObject2.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerUnitCoolingCapacity, 3.9475456E-5);
  idfObject2.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerFloorArea, 9.9676501E-3);
  idfObject2.setDouble(Sizing_SystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate, 1.0);
  idfObject2.setDouble(Sizing_SystemFields::HeatingFractionofAutosizedCoolingSupplyAirFlowRate, 1.0);
  idfObject2.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerUnitHeatingCapacity, 3.1588213E-5);
  idfObject2.setString(Sizing_SystemFields::CoolingDesignCapacityMethod, "CoolingDesignCapacity");
  idfObject2.setString(Sizing_SystemFields::CoolingDesignCapacity, "Autosize");
  idfObject2.setDouble(Sizing_SystemFields::CoolingDesignCapacityPerFloorArea, 234.7);
  idfObject2.setDouble(Sizing_SystemFields::FractionofAutosizedCoolingDesignCapacity, 1.0);
  idfObject2.setString(Sizing_SystemFields::HeatingDesignCapacityMethod, "HeatingDesignCapacity");
  idfObject2.setString(Sizing_SystemFields::HeatingDesignCapacity, "Autosize");
  idfObject2.setDouble(Sizing_SystemFields::HeatingDesignCapacityPerFloorArea, 157.0);
  idfObject2.setDouble(Sizing_SystemFields::FractionofAutosizedHeatingDesignCapacity, 1.0);
  idfObject2.setString(Sizing_SystemFields::CentralCoolingCapacityControlMethod, "OnOff");
  idfObject2.setString(Sizing_SystemFields::OccupantDiversity, "Autosize");

  openstudio::WorkspaceObject epSizingSystem = workspace.addObject(idfObject2).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<SizingSystem> sizingSystems = model.getConcreteModelObjects<SizingSystem>();
  ASSERT_EQ(1u, sizingSystems.size());
  SizingSystem sizingSystem = sizingSystems[0];

  EXPECT_EQ("Sensible", sizingSystem.typeofLoadtoSizeOn());
  EXPECT_TRUE(sizingSystem.isTypeofLoadtoSizeOnDefaulted());
  EXPECT_FALSE(sizingSystem.designOutdoorAirFlowRate());
  EXPECT_TRUE(sizingSystem.isDesignOutdoorAirFlowRateDefaulted());
  EXPECT_TRUE(sizingSystem.isDesignOutdoorAirFlowRateAutosized());
  ASSERT_TRUE(sizingSystem.centralHeatingMaximumSystemAirFlowRatio());
  EXPECT_EQ(0.3, sizingSystem.centralHeatingMaximumSystemAirFlowRatio().get());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioDefaulted());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioAutosized());
  EXPECT_EQ(7.0, sizingSystem.preheatDesignTemperature());
  EXPECT_EQ(0.008, sizingSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(12.8, sizingSystem.precoolDesignTemperature());
  EXPECT_EQ(0.008, sizingSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(12.8, sizingSystem.centralCoolingDesignSupplyAirTemperature());
  EXPECT_EQ(16.7, sizingSystem.centralHeatingDesignSupplyAirTemperature());
  EXPECT_EQ("NonCoincident", sizingSystem.sizingOption());
  EXPECT_TRUE(sizingSystem.isSizingOptionDefaulted());
  EXPECT_FALSE(sizingSystem.allOutdoorAirinCooling());
  EXPECT_TRUE(sizingSystem.isAllOutdoorAirinCoolingDefaulted());
  EXPECT_FALSE(sizingSystem.allOutdoorAirinHeating());
  EXPECT_TRUE(sizingSystem.isAllOutdoorAirinHeatingDefaulted());
  EXPECT_EQ(0.0085, sizingSystem.centralCoolingDesignSupplyAirHumidityRatio());
  EXPECT_FALSE(sizingSystem.isCentralCoolingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ(0.008, sizingSystem.centralHeatingDesignSupplyAirHumidityRatio());
  EXPECT_TRUE(sizingSystem.isCentralHeatingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ("DesignDay", sizingSystem.coolingDesignAirFlowMethod());
  EXPECT_TRUE(sizingSystem.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(0.0, sizingSystem.coolingDesignAirFlowRate());
  EXPECT_TRUE(sizingSystem.isCoolingDesignAirFlowRateDefaulted());
  EXPECT_EQ("DesignDay", sizingSystem.heatingDesignAirFlowMethod());
  EXPECT_TRUE(sizingSystem.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(0.0, sizingSystem.heatingDesignAirFlowRate());
  EXPECT_TRUE(sizingSystem.isHeatingDesignAirFlowRateDefaulted());
  EXPECT_EQ("ZoneSum", sizingSystem.systemOutdoorAirMethod());
  EXPECT_TRUE(sizingSystem.isSystemOutdoorAirMethodDefaulted());
  EXPECT_EQ(1.0, sizingSystem.zoneMaximumOutdoorAirFraction());
  EXPECT_EQ(9.9676501E-3, sizingSystem.coolingSupplyAirFlowRatePerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.coolingFractionofAutosizedCoolingSupplyAirFlowRate());
  EXPECT_EQ(3.9475456E-5, sizingSystem.coolingSupplyAirFlowRatePerUnitCoolingCapacity());
  EXPECT_EQ(9.9676501E-3, sizingSystem.heatingSupplyAirFlowRatePerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.heatingFractionofAutosizedHeatingSupplyAirFlowRate());
  EXPECT_EQ(1.0, sizingSystem.heatingFractionofAutosizedCoolingSupplyAirFlowRate());
  EXPECT_EQ(3.1588213E-5, sizingSystem.heatingSupplyAirFlowRatePerUnitHeatingCapacity());
  EXPECT_EQ("CoolingDesignCapacity", sizingSystem.coolingDesignCapacityMethod());
  EXPECT_FALSE(sizingSystem.coolingDesignCapacity());
  EXPECT_TRUE(sizingSystem.isCoolingDesignCapacityAutosized());
  EXPECT_EQ(234.7, sizingSystem.coolingDesignCapacityPerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_EQ("HeatingDesignCapacity", sizingSystem.heatingDesignCapacityMethod());
  EXPECT_FALSE(sizingSystem.heatingDesignCapacity());
  EXPECT_TRUE(sizingSystem.isHeatingDesignCapacityAutosized());
  EXPECT_EQ(157.0, sizingSystem.heatingDesignCapacityPerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.fractionofAutosizedHeatingDesignCapacity());
  EXPECT_EQ("OnOff", sizingSystem.centralCoolingCapacityControlMethod());
  EXPECT_FALSE(sizingSystem.occupantDiversity());
  EXPECT_TRUE(sizingSystem.isOccupantDiversityAutosized());
}
