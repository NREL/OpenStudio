/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/PortList.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include <utilities/idd/ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACWaterToAirHeatPump) {

  openstudio::energyplus::ForwardTranslator ft;

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  sch.setName("HP AvailSch");
  CoilHeatingWaterToAirHeatPumpEquationFit htg_coil(m);
  htg_coil.setName("HP HC");
  CoilCoolingWaterToAirHeatPumpEquationFit clg_coil(m);
  clg_coil.setName("HP CC");
  FanOnOff fan(m);
  fan.setName("HP FanOnOff");
  CoilHeatingElectric supp_htg_coil(m);
  supp_htg_coil.setName("HP SupHC");

  ZoneHVACWaterToAirHeatPump hp(m, sch, fan, htg_coil, clg_coil, supp_htg_coil);

  EXPECT_TRUE(hp.setSupplyAirFlowRateDuringCoolingOperation(1.1));
  EXPECT_TRUE(hp.setSupplyAirFlowRateDuringHeatingOperation(1.2));
  EXPECT_TRUE(hp.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(0.5));

  EXPECT_TRUE(hp.setOutdoorAirFlowRateDuringCoolingOperation(0.3));
  EXPECT_TRUE(hp.setOutdoorAirFlowRateDuringHeatingOperation(0.4));
  EXPECT_TRUE(hp.setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.2));

  EXPECT_TRUE(hp.setMaximumSupplyAirTemperaturefromSupplementalHeater(30.0));

  EXPECT_TRUE(hp.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(15.0));

  ScheduleConstant supplyAirFanSch(m);
  supplyAirFanSch.setName("supplyAirFanSch");
  EXPECT_TRUE(hp.setSupplyAirFanOperatingModeSchedule(supplyAirFanSch));
  EXPECT_TRUE(hp.setHeatPumpCoilWaterFlowMode("ConstantOnDemand"));

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(htg_coil));
  htg_coil.waterInletModelObject()->setName("HP HC Water Inlet Node");
  htg_coil.waterOutletModelObject()->setName("HP HC Water Outlet Node");
  htg_coil.autosizeRatedAirFlowRate();
  EXPECT_TRUE(htg_coil.setRatedWaterFlowRate(0.005));
  EXPECT_TRUE(htg_coil.setRatedHeatingCapacity(8000.0));
  EXPECT_TRUE(htg_coil.setRatedHeatingCoefficientofPerformance(3.2));
  EXPECT_TRUE(htg_coil.setRatedEnteringWaterTemperature(21.0));
  EXPECT_TRUE(htg_coil.setRatedEnteringAirDryBulbTemperature(19.0));
  EXPECT_TRUE(htg_coil.setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(1.1));

  EXPECT_TRUE(p.addDemandBranchForComponent(clg_coil));
  clg_coil.waterInletModelObject()->setName("HP CC Water Inlet Node");
  clg_coil.waterOutletModelObject()->setName("HP CC Water Outlet Node");
  EXPECT_TRUE(clg_coil.setRatedAirFlowRate(1.1));
  EXPECT_TRUE(clg_coil.setRatedWaterFlowRate(0.004));
  EXPECT_TRUE(clg_coil.setRatedTotalCoolingCapacity(7000.0));
  clg_coil.autosizeRatedSensibleCoolingCapacity();
  EXPECT_TRUE(clg_coil.setRatedCoolingCoefficientofPerformance(3.5));
  EXPECT_TRUE(clg_coil.setRatedEnteringWaterTemperature(31.0));
  EXPECT_TRUE(clg_coil.setRatedEnteringAirDryBulbTemperature(26.0));
  EXPECT_TRUE(clg_coil.setRatedEnteringAirWetBulbTemperature(20.1));

  UnitarySystemPerformanceMultispeed perf(m);
  perf.setName("US Perf Multispeed");
  EXPECT_TRUE(hp.setDesignSpecificationMultispeedObject(perf));

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  {
    // Not attached to a zone: not translated
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneHVAC_WaterToAirHeatPump).size());
  }

  // Need to be in a thermal zone to be translated, and TZ needs at least one space
  hp.addToThermalZone(z);
  z.inletPortList().modelObjects()[0].setName("Zone Air Inlet Node");
  z.exhaustPortList().modelObjects()[0].setName("Zone Air Exhaust Node");
  z.zoneAirNode().setName("Zone Air Node");

  auto getObjects =
    [](const Workspace& w) -> std::tuple<WorkspaceObject, WorkspaceObject, WorkspaceObject, WorkspaceObject, WorkspaceObject, WorkspaceObject> {
    WorkspaceObjectVector idf_hps(w.getObjectsByType(IddObjectType::ZoneHVAC_WaterToAirHeatPump));
    EXPECT_EQ(1, idf_hps.size());
    WorkspaceObject idf_hp(idf_hps[0]);

    auto idf_heatingCoil_ = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName);

    auto idf_coolingCoil_ = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName);

    auto idf_oamixer_ = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerName);

    auto idf_fan_ = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName);

    auto idf_suppHC_ = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName);

    return {idf_hp, idf_oamixer_.get(), idf_coolingCoil_.get(), idf_heatingCoil_.get(), idf_fan_.get(), idf_suppHC_.get()};
  };

  struct NodeNames
  {
    std::string zvInlet;
    std::string zvOutlet;
    std::string oaMixerReturnAir;
    std::string oaMixerOutlet;
    std::string coolingCoilInlet;
    std::string coolingCoilOutlet;
    std::string heatingCoilInlet;
    std::string heatingCoilOutlet;
    std::string fanInlet;
    std::string fanOutlet;
    std::string suppHeatingCoilInlet;
    std::string suppHeatingCoilOutlet;
  };

  auto getNodeNames = [&getObjects](const Workspace& w) {
    auto [idf_hp, idf_oamixer, idf_coolingCoil, idf_heatingCoil, idf_fan, idf_suppHC] = getObjects(w);

    EXPECT_EQ(idf_heatingCoil.iddObject().type(), IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit);
    EXPECT_EQ(idf_coolingCoil.iddObject().type(), IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit);
    EXPECT_EQ(idf_fan.iddObject().type(), IddObjectType::Fan_OnOff);

    NodeNames n;
    n.zvInlet = idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::AirInletNodeName).get();
    n.zvOutlet = idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::AirOutletNodeName).get();
    n.oaMixerReturnAir = idf_oamixer.getString(OutdoorAir_MixerFields::ReturnAirStreamNodeName).get();
    n.oaMixerOutlet = idf_oamixer.getString(OutdoorAir_MixerFields::MixedAirNodeName).get();
    n.coolingCoilInlet = idf_coolingCoil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName).get();
    n.coolingCoilOutlet = idf_coolingCoil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName).get();
    n.heatingCoilInlet = idf_heatingCoil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName).get();
    n.heatingCoilOutlet = idf_heatingCoil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName).get();
    n.fanInlet = idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get();
    n.fanOutlet = idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get();
    n.suppHeatingCoilInlet = idf_suppHC.getString(Coil_Heating_ElectricFields::AirInletNodeName).get();
    n.suppHeatingCoilOutlet = idf_suppHC.getString(Coil_Heating_ElectricFields::AirOutletNodeName).get();
    return n;
  };

  auto validateNodeMatch = [&getNodeNames](const Workspace& w, const std::string& testName = "", bool drawThrough = true) {
    NodeNames n = getNodeNames(w);
    EXPECT_FALSE(n.zvInlet.empty());
    EXPECT_FALSE(n.zvOutlet.empty());
    EXPECT_FALSE(n.oaMixerOutlet.empty());
    EXPECT_FALSE(n.coolingCoilInlet.empty());
    EXPECT_FALSE(n.coolingCoilOutlet.empty());
    EXPECT_FALSE(n.heatingCoilInlet.empty());
    EXPECT_FALSE(n.heatingCoilOutlet.empty());
    EXPECT_FALSE(n.fanInlet.empty());

    EXPECT_EQ(n.oaMixerReturnAir, n.zvInlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough
                                             << "]: OA Mixer Return Air Strem doesn't match WAHP Inlet";
    EXPECT_EQ("Zone Air Exhaust Node", n.zvInlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: WAHP Inlet wrong name";
    EXPECT_EQ(n.suppHeatingCoilOutlet, n.zvOutlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough
                                                   << "]: SuppHC Outlet doesn't match WAHP Outlet";
    EXPECT_EQ("Zone Air Inlet Node", n.zvOutlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: WAHP Onlet wrong name";

    if (drawThrough) {
      // o---- OA ---- CC ---- HC ---- Fan --- supHC -----o
      EXPECT_EQ(n.oaMixerOutlet, n.coolingCoilInlet)
        << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: OA Mixer Outlet doesn't match Cooling Coil Inlet";
      EXPECT_EQ(n.coolingCoilOutlet, n.heatingCoilInlet)
        << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: Cooling Outlet doesn't match Heating Coil Inlet";
      EXPECT_EQ(n.heatingCoilOutlet, n.fanInlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough
                                                 << "]: Heating Outlet doesn't match Fan Inlet";
      EXPECT_EQ(n.fanOutlet, n.suppHeatingCoilInlet)
        << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: Fan Outlet doesn't match SuppHC Inlet";

    } else {  // BlowThrough
      // o---- OA ---- Fan ----- CC ---- HC ---- supHC -----o
      EXPECT_EQ(n.oaMixerOutlet, n.fanInlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough
                                             << "]: OA Mixer Outlet doesn't match Fan Inlet";
      EXPECT_EQ(n.fanOutlet, n.coolingCoilInlet) << testName << " [drawThrough=" << std::boolalpha << drawThrough
                                                 << "]: Fan Outlet doesn't match Cooling Coil Inlet";
      EXPECT_EQ(n.coolingCoilOutlet, n.heatingCoilInlet)
        << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: Cooling Outlet doesn't match Heating Coil Inlet";
      EXPECT_EQ(n.heatingCoilOutlet, n.suppHeatingCoilInlet)
        << testName << " [drawThrough=" << std::boolalpha << drawThrough << "]: Heating Coil Outlet doesn't match SuppHC Inlet";
    }
  };

  for (bool drawThrough : {true, false}) {

    std::string fanPlacement = drawThrough ? "DrawThrough" : "BlowThrough";
    EXPECT_TRUE(hp.setFanPlacement(fanPlacement));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_hps(w.getObjectsByType(IddObjectType::ZoneHVAC_WaterToAirHeatPump));
    EXPECT_EQ(1, idf_hps.size());
    WorkspaceObject& idf_hp = idf_hps.front();

    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Fan_OnOff).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Coil_Heating_Electric).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::ZoneHVAC_EquipmentList).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed).size());

    validateNodeMatch(w, "", drawThrough);

    EXPECT_EQ("HP AvailSch", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityScheduleName).get());
    EXPECT_EQ("OutdoorAir:Mixer", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerObjectType).get());
    EXPECT_EQ("Zone HVAC Water To Air Heat Pump 1 OA Mixer", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerName).get());
    EXPECT_EQ(1.1, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::CoolingSupplyAirFlowRate).get());
    EXPECT_EQ(1.2, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatingSupplyAirFlowRate).get());
    EXPECT_EQ(0.5, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::NoLoadSupplyAirFlowRate).get());
    EXPECT_EQ(0.3, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::CoolingOutdoorAirFlowRate).get());
    EXPECT_EQ(0.4, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatingOutdoorAirFlowRate).get());
    EXPECT_EQ(0.2, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::NoLoadOutdoorAirFlowRate).get());
    EXPECT_EQ("Fan:OnOff", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanObjectType).get());
    EXPECT_EQ("HP FanOnOff", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName).get());
    EXPECT_EQ("Coil:Heating:WaterToAirHeatPump:EquationFit", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilObjectType).get());
    EXPECT_EQ("HP HC", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName).get());
    EXPECT_EQ("Coil:Cooling:WaterToAirHeatPump:EquationFit", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilObjectType).get());
    EXPECT_EQ("HP CC", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName).get());
    /*     EXPECT_EQ(2.6, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate).get());
    EXPECT_EQ(60.1, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant).get());
    EXPECT_EQ(0.02, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse).get());
    EXPECT_EQ(60.2, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime).get()); */
    EXPECT_EQ("Coil:Heating:Electric", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilObjectType).get());
    EXPECT_EQ("HP SupHC", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName).get());
    EXPECT_EQ(30.0, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater).get());
    EXPECT_EQ(15.0, idf_hp.getDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation).get());
    EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorDryBulbTemperatureSensorNodeName));
    EXPECT_EQ(fanPlacement, idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement).get());
    EXPECT_EQ("supplyAirFanSch", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName).get());
    EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityManagerListName));
    EXPECT_EQ("ConstantOnDemand", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpCoilWaterFlowMode).get());
    EXPECT_TRUE(idf_hp.isEmpty(ZoneHVAC_WaterToAirHeatPumpFields::DesignSpecificationZoneHVACSizingObjectName));
    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ("US Perf Multispeed", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::DesignSpecificationMultispeedObjectName).get());

    IdfObject idf_perf = workspace.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed)[0];

    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_TRUE(idf_perf.isEmpty(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio));

    ASSERT_EQ(0u, idf_perf.numExtensibleGroups());

    {
      ASSERT_TRUE(idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName));
      auto idf_heatingCoil = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName).get();

      EXPECT_EQ(htg_coil.nameString(), idf_heatingCoil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::Name).get());
      EXPECT_EQ("HP HC Water Inlet Node", idf_heatingCoil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName).get());
      EXPECT_EQ("HP HC Water Outlet Node", idf_heatingCoil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName).get());
      EXPECT_EQ("Autosize", idf_heatingCoil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, false).get());
      EXPECT_EQ(0.005, idf_heatingCoil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, false).get());
      EXPECT_EQ(8000.0, idf_heatingCoil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity, false).get());
      EXPECT_EQ(3.2, idf_heatingCoil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCOP, false).get());
      EXPECT_EQ(21.0, idf_heatingCoil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, false).get());
      EXPECT_EQ(19.0, idf_heatingCoil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, false).get());
      EXPECT_EQ(
        1.1,
        idf_heatingCoil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatioofRatedHeatingCapacitytoRatedCoolingCapacity, false).get());
      boost::optional<WorkspaceObject> woCurve1(
        idf_heatingCoil.getTarget(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCurveName));
      ASSERT_TRUE(woCurve1);
      EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_QuadLinear);
      boost::optional<WorkspaceObject> woCurve2(
        idf_heatingCoil.getTarget(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCurveName));
      ASSERT_TRUE(woCurve2);
      EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_QuadLinear);
    }

    {
      ASSERT_TRUE(idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName));
      auto idf_coolingCoil = idf_hp.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName).get();

      EXPECT_EQ(clg_coil.nameString(), idf_coolingCoil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::Name).get());
      EXPECT_EQ("HP CC Water Inlet Node", idf_coolingCoil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName).get());
      EXPECT_EQ("HP CC Water Outlet Node", idf_coolingCoil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName).get());
      EXPECT_EQ(1.1, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, false).get());
      EXPECT_EQ(0.004, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, false).get());
      EXPECT_EQ(7000.0, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity, false).get());
      EXPECT_EQ("Autosize",
                idf_coolingCoil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity, false).get());
      EXPECT_EQ(3.5, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedCoolingCOP, false).get());

      EXPECT_EQ(31.0, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, false).get());
      EXPECT_EQ(26.0, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, false).get());
      EXPECT_EQ(20.1, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirWetBulbTemperature, false).get());
      boost::optional<WorkspaceObject> woCurve3(
        idf_coolingCoil.getTarget(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName));
      ASSERT_TRUE(woCurve3);
      EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_QuadLinear);
      boost::optional<WorkspaceObject> woCurve4(
        idf_coolingCoil.getTarget(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName));
      ASSERT_TRUE(woCurve4);
      EXPECT_EQ(woCurve4->iddObject().type(), IddObjectType::Curve_QuintLinear);
      boost::optional<WorkspaceObject> woCurve5(
        idf_coolingCoil.getTarget(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName));
      ASSERT_TRUE(woCurve5);
      EXPECT_EQ(woCurve5->iddObject().type(), IddObjectType::Curve_QuadLinear);
      EXPECT_EQ(0, idf_coolingCoil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, false).get());
      EXPECT_EQ(0.0,
                idf_coolingCoil
                  .getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity)
                  .get());
    }
  }
}
