/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../GeneratorFuelCell.hpp"
#include "../GeneratorFuelCell_Impl.hpp"
#include "../GeneratorFuelCellPowerModule.hpp"
#include "../GeneratorFuelCellPowerModule_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCell) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // create default fuelcell
  GeneratorFuelCell fuelcell(model);
  // get default power module
  GeneratorFuelCellPowerModule fCPM = fuelcell.powerModule();
  // check default power module curve values
  Curve curve = fCPM.efficiencyCurve();
  auto curveQ = curve.cast<CurveQuadratic>();
  EXPECT_EQ(0.642388, curveQ.coefficient1Constant());
  EXPECT_EQ(-0.0001619, curveQ.coefficient2x());
  EXPECT_EQ(2.26e-008, curveQ.coefficient3xPOW2());
  EXPECT_EQ("Annex42", fCPM.efficiencyCurveMode());

  // check default Airsupply
  GeneratorFuelCellAirSupply fAS = fuelcell.airSupply();
  EXPECT_EQ("AirRatiobyStoics", fAS.airSupplyRateCalculationMode());
  EXPECT_EQ(1.0, fAS.stoichiometricRatio().get());
  EXPECT_EQ("NoRecovery", fAS.airIntakeHeatRecoveryMode());
  EXPECT_EQ("AmbientAir", fAS.airSupplyConstituentMode());

  // check default fuel supply
  GeneratorFuelSupply fS = fuelcell.fuelSupply();

  // check default water supply
  GeneratorFuelCellWaterSupply fWS = fuelcell.waterSupply();

  // check default aux heater
  GeneratorFuelCellAuxiliaryHeater fAX = fuelcell.auxiliaryHeater();

  // check default heat exchanger
  GeneratorFuelCellExhaustGasToWaterHeatExchanger fHX = fuelcell.heatExchanger();

  // check default electric storage
  GeneratorFuelCellElectricalStorage fES = fuelcell.electricalStorage();

  // check default inverter
  GeneratorFuelCellInverter fI = fuelcell.inverter();

  // check default optional stackcooler
  boost::optional<GeneratorFuelCellStackCooler> fSC = fuelcell.stackCooler();
}

TEST_F(ModelFixture, FuelCell2) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  GeneratorFuelCellAirSupply airSupply(model);
  GeneratorFuelCellAuxiliaryHeater auxHeater(model);
  GeneratorFuelCellElectricalStorage elecStorage(model);
  GeneratorFuelCellExhaustGasToWaterHeatExchanger exhaustHX(model);
  GeneratorFuelCellInverter inverter(model);
  GeneratorFuelCellPowerModule powerModule(model);
  GeneratorFuelCellStackCooler stackCooler(model);
  GeneratorFuelCellWaterSupply waterSupply(model);
  GeneratorFuelSupply fuelSupply(model);
  // create default fuelcell

  GeneratorFuelCell fuelcell(model, powerModule, airSupply, waterSupply, auxHeater, exhaustHX, elecStorage, inverter, fuelSupply);

  EXPECT_EQ(powerModule, fuelcell.powerModule());
  EXPECT_EQ(airSupply, fuelcell.airSupply());
  EXPECT_EQ(waterSupply, fuelcell.waterSupply());
  EXPECT_EQ(auxHeater, fuelcell.auxiliaryHeater());
  EXPECT_EQ(exhaustHX, fuelcell.heatExchanger());
  EXPECT_EQ(elecStorage, fuelcell.electricalStorage());
  EXPECT_EQ(inverter, fuelcell.inverter());
  EXPECT_EQ(fuelSupply, fuelcell.fuelSupply());
  //should be 0 default ELCD attached to FC
  std::vector<ElectricLoadCenterDistribution> elcd = model.getConcreteModelObjects<ElectricLoadCenterDistribution>();
  EXPECT_EQ(0u, elcd.size());
  EXPECT_FALSE(fuelcell.electricLoadCenterDistribution());
  //Add a ELCD
  ElectricLoadCenterDistribution elcd1(model);
  EXPECT_TRUE(elcd1.addGenerator(fuelcell));
  EXPECT_EQ(elcd1.handle(), fuelcell.electricLoadCenterDistribution().get().handle());
  //Add another ELCD
  ElectricLoadCenterDistribution elcd2(model);
  EXPECT_EQ(2, model.getConcreteModelObjects<ElectricLoadCenterDistribution>().size());
  //Add the FC to it which should remove the existing one attached to FC
  EXPECT_TRUE(elcd2.addGenerator(fuelcell));
  EXPECT_EQ(0, elcd1.generators().size());
  EXPECT_EQ(elcd2.handle(), fuelcell.electricLoadCenterDistribution().get().handle());
}

TEST_F(ModelFixture, FuelCell3) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  GeneratorFuelCellAirSupply airSupply(model);
  GeneratorFuelCellAuxiliaryHeater auxHeater(model);
  GeneratorFuelCellElectricalStorage elecStorage(model);
  GeneratorFuelCellExhaustGasToWaterHeatExchanger exhaustHX(model);
  GeneratorFuelCellInverter inverter(model);
  GeneratorFuelCellPowerModule powerModule(model);
  GeneratorFuelCellStackCooler stackCooler(model);
  GeneratorFuelCellWaterSupply waterSupply(model);
  GeneratorFuelSupply fuelSupply(model);
  // create default fuelcell

  GeneratorFuelCell fuelcell(model, powerModule, airSupply, waterSupply, auxHeater, exhaustHX, elecStorage, inverter, fuelSupply);

  auto fuelcellClone = fuelcell.clone(model).cast<GeneratorFuelCell>();

  EXPECT_EQ(fuelcell.children().size(), fuelcellClone.children().size());
}
