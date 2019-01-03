/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../Model.hpp"

// Need the Generator:MicroTurbine, the Generator:MicroTurbine:HeatRecovery and an ElectricalLoadCenterDistribution object
#include "../GeneratorMicroTurbine.hpp"
#include "../GeneratorMicroTurbine_Impl.hpp"
#include "../GeneratorMicroTurbineHeatRecovery.hpp"
#include "../GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "../Schedule.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"

// Curves
#include "../Curve.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"



// PlantLoop and Node for Heat Recovery module (and AirLoop to make sure you can't put it on one)
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"


using namespace openstudio;
using namespace openstudio::model;

//Test construction of the GeneratorMicroTurbine, then check that fields in default constructor
//are being set to the expected values
TEST_F(ModelFixture, GeneratorMicroTurbine_DefaultsSettersGetters) {

  //create a model to use in testing this code.
  Model model;

  // test constructor for Generator:MicroTurbine
  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);

  // Test default values that I've set in the constructors
  // Reference Electrical Power Output
  ASSERT_EQ(65000, mchp.referenceElectricalPowerOutput());
  mchp.setReferenceElectricalPowerOutput(60000);
  ASSERT_EQ(60000, mchp.referenceElectricalPowerOutput());

  // Reference Electrical Efficiency Using Lower Heating Value
  ASSERT_EQ(0.29, mchp.referenceElectricalEfficiencyUsingLowerHeatingValue());
  mchp.setReferenceElectricalEfficiencyUsingLowerHeatingValue(0.35);
  ASSERT_EQ(0.35, mchp.referenceElectricalEfficiencyUsingLowerHeatingValue());

  // Check that the curves have been properly defaulted

  //ElectricalPowerFunctionofTemperatureandElevationCurve Name
  ASSERT_EQ(1.2027697,mchp.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(8.797885E-07,mchp.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient6xTIMESY());

  //ElectricalEfficiencyFunctionofTemperatureCurve
  ASSERT_EQ(1.0402217,mchp.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(5.133175E-07,mchp.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient4xPOW3());

  //ElectricalEfficiencyFunctionofPartLoadRatioCurve
  ASSERT_EQ(0.215290,mchp.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(1.497306,mchp.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient4xPOW3());

  // Check curve setters
  // Test curve setters to see if set at the right place
  CurveBiquadratic elecPowerFTempElevation(model);
    elecPowerFTempElevation.setCoefficient1Constant(1);
    elecPowerFTempElevation.setCoefficient2x(0.1);
    elecPowerFTempElevation.setCoefficient3xPOW2(0.01);
    mchp.setElectricalPowerFunctionofTemperatureandElevationCurve(elecPowerFTempElevation);
  CurveCubic elecEffFT(model);
    elecEffFT.setCoefficient1Constant(2);
    elecEffFT.setCoefficient2x(0.2);
    elecEffFT.setCoefficient3xPOW2(0.02);
    mchp.setElectricalEfficiencyFunctionofTemperatureCurve(elecEffFT);
  CurveCubic elecEffFPLR(model);
    elecEffFPLR.setCoefficient1Constant(3);
    elecEffFPLR.setCoefficient2x(0.3);
    elecEffFPLR.setCoefficient3xPOW2(0.03);
    mchp.setElectricalEfficiencyFunctionofPartLoadRatioCurve(elecEffFPLR);

  // Verify the setter
  ASSERT_EQ(1,mchp.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(2,mchp.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(3,mchp.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());


  // Test defaulted values
  EXPECT_FALSE(mchp.availabilitySchedule());
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(mchp.setAvailabilitySchedule(schedule));
  ASSERT_TRUE(mchp.availabilitySchedule());


  EXPECT_TRUE(mchp.isMinimumFullLoadElectricalPowerOutputDefaulted());
  EXPECT_EQ(0.0, mchp.minimumFullLoadElectricalPowerOutput());
  mchp.setMinimumFullLoadElectricalPowerOutput(10);
  EXPECT_EQ(10.0, mchp.minimumFullLoadElectricalPowerOutput());
  EXPECT_FALSE(mchp.isMinimumFullLoadElectricalPowerOutputDefaulted());
  mchp.resetMinimumFullLoadElectricalPowerOutput();
  EXPECT_TRUE(mchp.isMinimumFullLoadElectricalPowerOutputDefaulted());

  // Maximum Full Load Electrical Power Output => defaulted to referenceElectricalPowerOutput
  EXPECT_TRUE(mchp.isMaximumFullLoadElectricalPowerOutputDefaulted());
  EXPECT_EQ(mchp.referenceElectricalPowerOutput(), mchp.maximumFullLoadElectricalPowerOutput());
  mchp.setMaximumFullLoadElectricalPowerOutput(55000);
  EXPECT_EQ(55000.0, mchp.maximumFullLoadElectricalPowerOutput());
  EXPECT_FALSE(mchp.isMaximumFullLoadElectricalPowerOutputDefaulted());
  mchp.resetMaximumFullLoadElectricalPowerOutput();
  EXPECT_TRUE(mchp.isMaximumFullLoadElectricalPowerOutputDefaulted());

  // Reference Combustion Air Inlet Temperature defaults to 15
  EXPECT_TRUE(mchp.isReferenceCombustionAirInletTemperatureDefaulted());
  EXPECT_EQ(15.0, mchp.referenceCombustionAirInletTemperature());
  mchp.setReferenceCombustionAirInletTemperature(18);
  EXPECT_EQ(18.0, mchp.referenceCombustionAirInletTemperature());
  EXPECT_FALSE(mchp.isReferenceCombustionAirInletTemperatureDefaulted());
  mchp.resetReferenceCombustionAirInletTemperature();
  EXPECT_TRUE(mchp.isReferenceCombustionAirInletTemperatureDefaulted());

  // Reference Combustion Air Inlet Humidity Ratio, defaults to 0.00638
  EXPECT_TRUE(mchp.isReferenceCombustionAirInletHumidityRatioDefaulted());
  EXPECT_EQ(0.00638, mchp.referenceCombustionAirInletHumidityRatio());
  mchp.setReferenceCombustionAirInletHumidityRatio(0.008);
  EXPECT_EQ(0.008, mchp.referenceCombustionAirInletHumidityRatio());
  EXPECT_FALSE(mchp.isReferenceCombustionAirInletHumidityRatioDefaulted());
  mchp.resetReferenceCombustionAirInletHumidityRatio();
  EXPECT_TRUE(mchp.isReferenceCombustionAirInletHumidityRatioDefaulted());

  // Reference Elevation
  EXPECT_TRUE(mchp.isReferenceElevationDefaulted());
  EXPECT_EQ(0.0, mchp.referenceElevation());
  mchp.setReferenceElevation(10);
  EXPECT_EQ(10.0, mchp.referenceElevation());
  EXPECT_FALSE(mchp.isReferenceElevationDefaulted());
  mchp.resetReferenceElevation();
  EXPECT_TRUE(mchp.isReferenceElevationDefaulted());

  //Fuel Type
  EXPECT_TRUE(mchp.isFuelTypeDefaulted());
  EXPECT_EQ("NaturalGas", mchp.fuelType());
  mchp.setFuelType("PropaneGas");
  EXPECT_EQ("PropaneGas", mchp.fuelType());
  EXPECT_FALSE(mchp.isFuelTypeDefaulted());
  mchp.resetFuelType();
  EXPECT_TRUE(mchp.isFuelTypeDefaulted());

  // FuelHigherHeatingValue
  // \default 50000
  EXPECT_TRUE(mchp.isFuelHigherHeatingValueDefaulted());
  EXPECT_EQ(50000.0, mchp.fuelHigherHeatingValue());
  mchp.setFuelHigherHeatingValue(45000);
  EXPECT_EQ(45000, mchp.fuelHigherHeatingValue());
  EXPECT_FALSE(mchp.isFuelHigherHeatingValueDefaulted());
  mchp.resetFuelHigherHeatingValue();
  EXPECT_TRUE(mchp.isFuelHigherHeatingValueDefaulted());

  //N9, \field Fuel Lower Heating Value
  //   \default 45450
  EXPECT_TRUE(mchp.isFuelLowerHeatingValueDefaulted());
  EXPECT_EQ(45450.0, mchp.fuelLowerHeatingValue());
  mchp.setFuelLowerHeatingValue(45000);
  EXPECT_EQ(45000, mchp.fuelLowerHeatingValue());
  EXPECT_FALSE(mchp.isFuelLowerHeatingValueDefaulted());
  mchp.resetFuelLowerHeatingValue();
  EXPECT_TRUE(mchp.isFuelLowerHeatingValueDefaulted());

  // N10, \field Standby Power
  //   \default 0.0
  EXPECT_TRUE(mchp.isStandbyPowerDefaulted());
  EXPECT_EQ(0.0, mchp.standbyPower());
  mchp.setStandbyPower(10);
  EXPECT_EQ(10.0, mchp.standbyPower());
  EXPECT_FALSE(mchp.isStandbyPowerDefaulted());
  mchp.resetStandbyPower();
  EXPECT_TRUE(mchp.isStandbyPowerDefaulted());


  // N11, \field Ancillary Power
  //   \default 0.0
  EXPECT_TRUE(mchp.isAncillaryPowerDefaulted());
  EXPECT_EQ(0.0, mchp.ancillaryPower());
  mchp.setAncillaryPower(10);
  EXPECT_EQ(10.0, mchp.ancillaryPower());
  EXPECT_FALSE(mchp.isAncillaryPowerDefaulted());
  mchp.resetAncillaryPower();
  EXPECT_TRUE(mchp.isAncillaryPowerDefaulted());


  // \field Ancillary Power Function of Fuel Input Curve Name
  // \object-list QuadraticCurves
  EXPECT_FALSE(mchp.ancillaryPowerFunctionofFuelInputCurve());
  CurveQuadratic ancPFfuelInputCurve(model);
  mchp.setAncillaryPowerFunctionofFuelInputCurve(ancPFfuelInputCurve);
  ASSERT_TRUE(mchp.ancillaryPowerFunctionofFuelInputCurve());
  if( boost::optional<Curve> setCurve = mchp.ancillaryPowerFunctionofFuelInputCurve() ) {
    EXPECT_EQ(ancPFfuelInputCurve.handle(), setCurve->handle());
  }


  // Optional Generator:MicroTurbine:HeatRecovery
  // \field Generator MicroTurbine Heat Recovery Name
  // I'll check the setter and getter in a separate function where I also test all of this object
  EXPECT_FALSE(mchp.generatorMicroTurbineHeatRecovery());

  // TODO: Check return type. From object lists, some candidates are: Connection.
  //boost::optional<Connection> combustionAirInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // boost::optional<Connection> combustionAirOutletNode() const;

  // N12, \field Reference Exhaust Air Mass Flow Rate
  EXPECT_FALSE(mchp.referenceExhaustAirMassFlowRate());
  mchp.setReferenceExhaustAirMassFlowRate(100);
  boost::optional<double> testrefEAmdotR = mchp.referenceExhaustAirMassFlowRate();
  EXPECT_EQ((*testrefEAmdotR),100);

  // A12, \field Exhaust Air Flow Rate Function of Temperature Curve Name
  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  EXPECT_FALSE(mchp.exhaustAirFlowRateFunctionofTemperatureCurve());
  CurveQuadratic exhaustAirFlowFT(model);
  mchp.setExhaustAirFlowRateFunctionofTemperatureCurve(exhaustAirFlowFT);
  ASSERT_TRUE(mchp.exhaustAirFlowRateFunctionofTemperatureCurve());
  if( boost::optional<Curve> setCurve = mchp.exhaustAirFlowRateFunctionofTemperatureCurve() ) {
    EXPECT_EQ(exhaustAirFlowFT.handle(), setCurve->handle());
  }




  // A13, \field Exhaust Air Flow Rate Function of Part Load Ratio Curve Name
  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  // Exhaust Air Flow Rate Function of Part Load Ratio Curve Name
  EXPECT_FALSE(mchp.exhaustAirFlowRateFunctionofPartLoadRatioCurve());
  CurveQuadratic exhaustAirFlowFPLR(model);
  mchp.setExhaustAirFlowRateFunctionofPartLoadRatioCurve(exhaustAirFlowFPLR);
  ASSERT_TRUE(mchp.exhaustAirFlowRateFunctionofPartLoadRatioCurve());
  if( boost::optional<Curve> setCurve = mchp.exhaustAirFlowRateFunctionofPartLoadRatioCurve() ) {
    EXPECT_EQ(exhaustAirFlowFPLR.handle(), setCurve->handle());
  }


  // N13, \field Nominal Exhaust Air Outlet Temperature
  EXPECT_FALSE(mchp.nominalExhaustAirOutletTemperature());
  mchp.setNominalExhaustAirOutletTemperature(100);
  boost::optional<double> testnomEAmdotR = mchp.nominalExhaustAirOutletTemperature();
  EXPECT_EQ((*testnomEAmdotR),100);

  // A14, \field Exhaust Air Temperature Function of Temperature Curve Name
  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  EXPECT_FALSE(mchp.exhaustAirTemperatureFunctionofPartLoadRatioCurve());
  CurveQuadratic exhaustAirTempFPLR(model);
  mchp.setExhaustAirTemperatureFunctionofPartLoadRatioCurve(exhaustAirTempFPLR);
  ASSERT_TRUE(mchp.exhaustAirTemperatureFunctionofPartLoadRatioCurve());
  if( boost::optional<Curve> setCurve = mchp.exhaustAirTemperatureFunctionofPartLoadRatioCurve() ) {
    EXPECT_EQ(exhaustAirTempFPLR.handle(), setCurve->handle());
  }

}

TEST_F(ModelFixture, GeneratorMicroTurbine_HeatRecovery) {

  //create a model to use in testing this code.
  Model model;

  // test constructor for Generator:MicroTurbine
  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);

  // A3, \field Heat Recovery Water Inlet Node Name

  //  A4, \field Heat Recovery Water Outlet Node Name


  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);

  // Check if the parent mchp does have this mchpHR set
  EXPECT_EQ(mchpHR, mchp.generatorMicroTurbineHeatRecovery().get());


  // Check if we can get the parent mchp from the mchpHR
  EXPECT_EQ(mchp, mchpHR.generatorMicroTurbine());

  //N1 , \field Reference Thermal Efficiency Using Lower Heat Value
  //  \default 0.0 but overriden in construction to 0.4975
  EXPECT_FALSE(mchpHR.isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted());
  EXPECT_EQ(0.4975, mchpHR.referenceThermalEfficiencyUsingLowerHeatValue());
  mchpHR.setReferenceThermalEfficiencyUsingLowerHeatValue(0.5);
  EXPECT_EQ(0.5, mchpHR.referenceThermalEfficiencyUsingLowerHeatValue());
  EXPECT_FALSE(mchpHR.isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted());
  mchpHR.resetReferenceThermalEfficiencyUsingLowerHeatValue();
  EXPECT_TRUE(mchpHR.isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted());

  // N2 , \field Reference Inlet Water Temperature = 60 in constructor
  // boost::optional<double> referenceInletWaterTemperature() const;
  EXPECT_EQ(60.0, mchpHR.referenceInletWaterTemperature());
  mchpHR.setReferenceInletWaterTemperature(65);
  EXPECT_EQ(65.0, mchpHR.referenceInletWaterTemperature());


  // A5 , \field Heat Recovery Water Flow Operating Mode
  //  \default PlantControl
  EXPECT_TRUE(mchpHR.isHeatRecoveryWaterFlowOperatingModeDefaulted());
  EXPECT_EQ("PlantControl", mchpHR.heatRecoveryWaterFlowOperatingMode());
  mchpHR.setHeatRecoveryWaterFlowOperatingMode("InternalControl");
  EXPECT_EQ("InternalControl", mchpHR.heatRecoveryWaterFlowOperatingMode());
  EXPECT_FALSE(mchpHR.isHeatRecoveryWaterFlowOperatingModeDefaulted());
  mchpHR.resetHeatRecoveryWaterFlowOperatingMode();
  EXPECT_TRUE(mchpHR.isHeatRecoveryWaterFlowOperatingModeDefaulted());

  // N6 , \field Reference Heat Recovery Water Flow Rate = 0.00252362
  EXPECT_EQ(0.00252362, mchpHR.referenceHeatRecoveryWaterFlowRate());
  mchpHR.setReferenceHeatRecoveryWaterFlowRate(0.005);
  EXPECT_EQ(0.005, mchpHR.referenceHeatRecoveryWaterFlowRate());

  // A6 , \field Heat Recovery Water Flow Rate Function of Temperature and Power Curve Name
  // BiquadraticCurves, BiVariateTables.
  // boost::optional<Curve> heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const;
  // \note If left blank, model assumes the heat recovery water flow rate
  EXPECT_FALSE(mchpHR.heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve());
  CurveBiquadratic hrWaterFlowFTP(model);
  mchpHR.setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(hrWaterFlowFTP);
  ASSERT_TRUE(mchpHR.heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve());
  if( boost::optional<Curve> setCurve = mchpHR.heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() ) {
    EXPECT_EQ(hrWaterFlowFTP.handle(), setCurve->handle());
  }


  // A7 , \field Thermal Efficiency Function of Temperature and Elevation Curve Name
  // \object-list BicubicBiquadraticCurves, BiVariateTables
  // boost::optional<Curve> thermalEfficiencyFunctionofTemperatureandElevationCurve() const;
  EXPECT_FALSE(mchpHR.thermalEfficiencyFunctionofTemperatureandElevationCurve());
  CurveBiquadratic hrEffFTempElev(model);
  mchpHR.setThermalEfficiencyFunctionofTemperatureandElevationCurve(hrEffFTempElev);
  ASSERT_TRUE(mchpHR.thermalEfficiencyFunctionofTemperatureandElevationCurve());
  if( boost::optional<Curve> setCurve = mchpHR.thermalEfficiencyFunctionofTemperatureandElevationCurve() ) {
    EXPECT_EQ(hrEffFTempElev.handle(), setCurve->handle());
  }

  // A8 , \field Heat Recovery Rate Function of Part Load Ratio Curve Name
  // QuadraticCubicCurves, UniVariateTables
  // boost::optional<Curve> heatRecoveryRateFunctionofPartLoadRatioCurve() const;
  EXPECT_FALSE(mchpHR.heatRecoveryRateFunctionofPartLoadRatioCurve());
  CurveCubic hrRRWaterFlowFPLR(model);
  mchpHR.setHeatRecoveryRateFunctionofPartLoadRatioCurve(hrRRWaterFlowFPLR);
  ASSERT_TRUE(mchpHR.heatRecoveryRateFunctionofPartLoadRatioCurve());
  if( boost::optional<Curve> setCurve = mchpHR.heatRecoveryRateFunctionofPartLoadRatioCurve() ) {
    EXPECT_EQ(hrRRWaterFlowFPLR.handle(), setCurve->handle());
  }



  // A9 , \field Heat Recovery Rate Function of Inlet Water Temperature Curve Name
  // QuadraticCurves, UniVariateTables
  // boost::optional<Curve> heatRecoveryRateFunctionofInletWaterTemperatureCurve() const;
  EXPECT_FALSE(mchpHR.heatRecoveryRateFunctionofInletWaterTemperatureCurve());
  CurveQuadratic hrRRFInletTemp(model);
  mchpHR.setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(hrRRFInletTemp);
  ASSERT_TRUE(mchpHR.heatRecoveryRateFunctionofInletWaterTemperatureCurve());
  if( boost::optional<Curve> setCurve = mchpHR.heatRecoveryRateFunctionofInletWaterTemperatureCurve() ) {
    EXPECT_EQ(hrRRFInletTemp.handle(), setCurve->handle());
  }


  // A10, \field Heat Recovery Rate Function of Water Flow Rate Curve Name
  // QuadraticCurves, UniVariateTables.
  // boost::optional<Curve> heatRecoveryRateFunctionofWaterFlowRateCurve() const;
  EXPECT_FALSE(mchpHR.heatRecoveryRateFunctionofWaterFlowRateCurve());
  CurveQuadratic hrRRFWaterFlow(model);
  mchpHR.setHeatRecoveryRateFunctionofWaterFlowRateCurve(hrRRFWaterFlow);
  ASSERT_TRUE(mchpHR.heatRecoveryRateFunctionofWaterFlowRateCurve());
  if( boost::optional<Curve> setCurve = mchpHR.heatRecoveryRateFunctionofWaterFlowRateCurve() ) {
    EXPECT_EQ(hrRRFWaterFlow.handle(), setCurve->handle());
  }


  // N7, \field Minimum Heat Recovery Water Flow Rate
  EXPECT_TRUE(mchpHR.isMinimumHeatRecoveryWaterFlowRateDefaulted());
  EXPECT_EQ(0, mchpHR.minimumHeatRecoveryWaterFlowRate());
  mchpHR.setMinimumHeatRecoveryWaterFlowRate(0.001);
  EXPECT_EQ(0.001, mchpHR.minimumHeatRecoveryWaterFlowRate());
  EXPECT_FALSE(mchpHR.isMinimumHeatRecoveryWaterFlowRateDefaulted());
  mchpHR.resetMinimumHeatRecoveryWaterFlowRate();
  EXPECT_TRUE(mchpHR.isMinimumHeatRecoveryWaterFlowRateDefaulted());

  // N8, \field Maximum Heat Recovery Water Flow Rate
  // TODO: I think I should really make this default to reference hr water flow rate times 1.5
  EXPECT_TRUE(mchpHR.isMaximumHeatRecoveryWaterFlowRateDefaulted());
  //EXPECT_EQ("PlantControl", mchpHR.maximumHeatRecoveryWaterFlowRate());
  mchpHR.setMaximumHeatRecoveryWaterFlowRate(0.003);
  EXPECT_EQ(0.003, mchpHR.maximumHeatRecoveryWaterFlowRate());
  EXPECT_FALSE(mchpHR.isMaximumHeatRecoveryWaterFlowRateDefaulted());
  mchpHR.resetMaximumHeatRecoveryWaterFlowRate();
  EXPECT_TRUE(mchpHR.isMaximumHeatRecoveryWaterFlowRateDefaulted());

  //  N9; \field Maximum Heat Recovery Water Temperature
  // boost::optional<double> maximumHeatRecoveryWaterTemperature() const;
  EXPECT_FALSE(mchpHR.maximumHeatRecoveryWaterTemperature());
  mchpHR.setMaximumHeatRecoveryWaterTemperature(90);
  boost::optional<double> testmaxHRWT = mchpHR.maximumHeatRecoveryWaterTemperature();
  EXPECT_EQ((*testmaxHRWT),90);

  // Test setRatedThermaltoElectricalPowerRatio
  mchpHR.setRatedThermaltoElectricalPowerRatio(1.72);
  EXPECT_EQ(1.72, mchpHR.ratedThermaltoElectricalPowerRatio());

  // Test the accessor from the mchp
  boost::optional<double> ratedThermaltoElectricalPowerRatio = mchp.ratedThermaltoElectricalPowerRatio();
  EXPECT_EQ((*ratedThermaltoElectricalPowerRatio),1.72);

  // Test setting it to a microturbine and resetting
  /*mchp.setGeneratorMicroTurbineHeatRecovery(mchpHR);
  ASSERT_TRUE(mchp.generatorMicroTurbineHeatRecovery());
  boost::optional<GeneratorMicroTurbineHeatRecovery> setmchpHR = mchp.generatorMicroTurbineHeatRecovery();
  EXPECT_EQ(mchpHR.handle(), setmchpHR->handle());
  mchp.resetGeneratorMicroTurbineHeatRecovery();
  ASSERT_FALSE(mchp.generatorMicroTurbineHeatRecovery());*/

  // Try deleting the mchpHR and see if the mchp no longer has a mchpHR or not
  mchpHR.remove();

  EXPECT_FALSE(mchp.generatorMicroTurbineHeatRecovery());



}


//Test cloning the MicroTurbine
// Todo: Need to handle that properly, check if the HR was cloned too
TEST_F(ModelFixture,GeneratorMicroTurbine_Clone)
{

  Model model;

  // test constructor for Generator:MicroTurbine
  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);

  // Test curve setters to see if set at the right place
  CurveBiquadratic elecPowerFTempElevation(model);
    elecPowerFTempElevation.setCoefficient1Constant(1);
    elecPowerFTempElevation.setCoefficient2x(0.1);
    elecPowerFTempElevation.setCoefficient3xPOW2(0.01);
    mchp.setElectricalPowerFunctionofTemperatureandElevationCurve(elecPowerFTempElevation);
  CurveCubic elecEffFT(model);
    elecEffFT.setCoefficient1Constant(2);
    elecEffFT.setCoefficient2x(0.2);
    elecEffFT.setCoefficient3xPOW2(0.02);
    mchp.setElectricalEfficiencyFunctionofTemperatureCurve(elecEffFT);
  CurveCubic elecEffFPLR(model);
    elecEffFPLR.setCoefficient1Constant(3);
    elecEffFPLR.setCoefficient2x(0.3);
    elecEffFPLR.setCoefficient3xPOW2(0.03);
    mchp.setElectricalEfficiencyFunctionofPartLoadRatioCurve(elecEffFPLR);

  // Verify the setter is already done above...

  //Clone into the same model
  GeneratorMicroTurbine  mchpClone = mchp.clone(model).cast<GeneratorMicroTurbine>();

  ASSERT_EQ(1,mchpClone.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(2,mchpClone.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(3,mchpClone.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());

  // Add a MicroTurbine:HeatRecovery and clone again
  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);

  // Clone in same model and verify that the mCHPHR is also cloned
  GeneratorMicroTurbine  mchpClone1 = mchp.clone(model).cast<GeneratorMicroTurbine>();
  ASSERT_TRUE(mchpClone1.generatorMicroTurbineHeatRecovery());
  // Make sure it's not just pointing to the same one
  boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHRclone = mchpClone1.generatorMicroTurbineHeatRecovery();
  EXPECT_NE(mchpHR.handle(), mchpHRclone->handle());


  //Clone into another model
  Model model2;
  GeneratorMicroTurbine  mchpClone2 = mchp.clone(model2).cast<GeneratorMicroTurbine>();

  // Check that curves have been carried with it
  ASSERT_EQ(1,mchpClone2.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(2,mchpClone2.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(3,mchpClone2.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());

  // Check that the heatRecovery module has been clone into the model too
  ASSERT_TRUE(mchpClone2.generatorMicroTurbineHeatRecovery());
  // Make sure it's not just pointing to the same one
  boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHRclone2 = mchpClone1.generatorMicroTurbineHeatRecovery();
  EXPECT_NE(mchpHR.handle(), mchpHRclone2->handle());

}


TEST_F(ModelFixture,GeneratorMicroTurbine_HeatRecovery_addToNode) {
  Model model;

  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);

  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);

  AirLoopHVAC airLoop = AirLoopHVAC(model);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(mchpHR.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(mchpHR.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(model);

  // This should de settable to the supply side
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(mchpHR.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );

  // This should be settable to the demand side too
  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(mchpHR.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  GeneratorMicroTurbineHeatRecovery mchpHRClone = mchpHR.clone(model).cast<GeneratorMicroTurbineHeatRecovery>();
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_TRUE(mchpHRClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );
}

TEST_F(ModelFixture, GeneratorMicroTurbine_ElectricLoadCenterDistribution) {
  Model model;

  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);

  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);

  //should be 1 default ELCD attached to mchp
  std::vector<ElectricLoadCenterDistribution> elcd = model.getModelObjects<ElectricLoadCenterDistribution>();
  EXPECT_EQ(1u, elcd.size());
  EXPECT_EQ(1u, elcd[0].generators().size());
  std::vector<Generator> generators = elcd[0].generators();
  EXPECT_EQ(generators[0].handle(), mchp.handle());
  EXPECT_TRUE(mchp.electricLoadCenterDistribution());
  EXPECT_EQ(elcd[0].handle(), mchp.electricLoadCenterDistribution().get().handle());
  //Add another ELCD
  ElectricLoadCenterDistribution elcd2(model);
  EXPECT_EQ(2, model.getModelObjects<ElectricLoadCenterDistribution>().size());
  //Add the mchp to it which should remove the existing one attached to mchp
  EXPECT_TRUE(elcd2.addGenerator(mchp));
  EXPECT_EQ(0, elcd[0].generators().size());
  EXPECT_EQ(elcd2.handle(), mchp.electricLoadCenterDistribution().get().handle());
}

