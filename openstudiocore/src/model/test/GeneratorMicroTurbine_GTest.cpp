/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

// Need the Generator:MicroTurbine, the Generator:MicroTurbine:HeatRecovery and an ElectricalLoadCenterDistribution object
#include "../GeneratorMicroTurbine.hpp"
#include "../GeneratorMicroTurbine_Impl.hpp"
#include "../GeneratorMicroTurbineHeatRecovery.hpp"
#include "../GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"

// Curves
#include "../Curve.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

// PlantLoop for Heat Recovery module
#include "../PlantLoop.hpp"


using namespace openstudio;
using namespace openstudio::model;

//Test construction of the GeneratorMicroTurbine, then check that fields in default constructor
//are being set to the expected values
TEST_F(ModelFixture, GeneratorMicroTurbine_SimpleDefaults) {
  
  //create a model to use in testing this code.
  Model model;
  
  // test constructor for Generator:MicroTurbine
  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);
  
  // Test default values that I've set in the constructors
  // Reference Electrical Power Output
  ASSERT_EQ(65000, mchp.referenceElectricalPowerOutput());

  // Reference Electrical Efficiency Using Lower Heating Value
  ASSERT_EQ(0.29, mchp.referenceElectricalEfficiencyUsingLowerHeatingValue());
  
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

  
  // Test defaulted values
  EXPECT_TRUE(mchp.isMinimumFullLoadElectricalPowerOutputDefaulted());
  EXPECT_EQ(0.0, mchp.minimumFullLoadElectricalPowerOutput());
  
  // Maximum Full Load Electrical Power Output => defaulted to referenceElectricalPowerOutput
  EXPECT_TRUE(mchp.isMaximumFullLoadElectricalPowerOutputDefaulted());
  EXPECT_EQ(mchp.referenceElectricalPowerOutput, mchp.maximumFullLoadElectricalPowerOutput());
  
  // Reference Combustion Air Inlet Temperature defaults to 15
  EXPECT_TRUE(mchp.isReferenceCombustionAirInletTemperatureDefaulted());
  EXPECT_EQ(15.0, mchp.referenceCombustionAirInletTemperature());
  
  // Reference Combustion Air Inlet Humidity Ratio, defaults to 0.00638
  EXPECT_TRUE(mchp.isReferenceCombustionAirInletHumidityRatioDefaulted());
  EXPECT_EQ(0.00638, mchp.referenceCombustionAirInletHumidityRatio());
  
  // Reference Elevation
  EXPECT_TRUE(mchp.isReferenceElevationDefaulted());
  EXPECT_EQ(0.0, mchp.referenceElevation());
  
  
  EXPECT_FALSE(panel.surface());
  EXPECT_EQ("Decoupled", panel.heatTransferIntegrationMode());
  EXPECT_TRUE(panel.isHeatTransferIntegrationModeDefaulted());
  EXPECT_EQ(1.0, panel.numberOfModulesInParallel());
  EXPECT_TRUE(panel.isNumberOfModulesInParallelDefaulted());
  EXPECT_EQ(1.0, panel.numberOfModulesInSeries());
  EXPECT_TRUE(panel.isNumberOfModulesInSeriesDefaulted());
  EXPECT_FALSE(panel.ratedElectricPowerOutput());
  EXPECT_FALSE(panel.availabilitySchedule());

  EXPECT_FALSE(panel.ratedElectricPowerOutput());
  EXPECT_FALSE(panel.availabilitySchedule());
  EXPECT_FALSE(panel.ratedThermalToElectricalPowerRatio());
  EXPECT_FALSE(panel.electricLoadCenterDistribution());



  ShadingSurface shadingSurface(points, model);

  EXPECT_TRUE(panel.setSurface(shadingSurface));
  EXPECT_TRUE(panel.surface());
  EXPECT_EQ(1u, shadingSurface.generatorPhotovoltaics().size());
  panel.resetSurface();
  EXPECT_TRUE(panel.setHeatTransferIntegrationMode("IntegratedSurfaceOutsideFace"));
  EXPECT_EQ("IntegratedSurfaceOutsideFace", panel.heatTransferIntegrationMode());
  EXPECT_FALSE(panel.isHeatTransferIntegrationModeDefaulted());
  panel.resetHeatTransferIntegrationMode();
  EXPECT_TRUE(panel.setNumberOfModulesInParallel(2.0));
  EXPECT_EQ(2.0, panel.numberOfModulesInParallel());
  EXPECT_FALSE(panel.isNumberOfModulesInParallelDefaulted());
  panel.resetNumberOfModulesInParallel();
  EXPECT_TRUE(panel.setNumberOfModulesInSeries(3.0));
  EXPECT_EQ(3.0, panel.numberOfModulesInSeries());
  EXPECT_FALSE(panel.isNumberOfModulesInSeriesDefaulted());
  panel.resetNumberOfModulesInSeries();
  panel.setRatedElectricPowerOutput(10.0);
  ASSERT_TRUE(panel.ratedElectricPowerOutput());
  EXPECT_EQ(10.0, panel.ratedElectricPowerOutput().get());
  panel.resetRatedElectricPowerOutput();
  auto schedule = model.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(panel.setAvailabilitySchedule(schedule));
  ASSERT_TRUE(panel.availabilitySchedule());
  panel.resetAvailabilitySchedule();

  PhotovoltaicPerformance performance = panel.photovoltaicPerformance();
  ASSERT_TRUE(performance.optionalCast<PhotovoltaicPerformanceSimple>());
}


//Test cloning the coil
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
    mchp.setElectricalEfficiencyFunctionofTemperatureCurve(elecEffFPLR);
    
  // Verify the setter
  ASSERT_EQ(1,mchp.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(2,mchp.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(3,mchp.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());
  
  //Clone into the same model
  GeneratorMicroTurbine  mchpClone = mchp.clone(model).cast<GeneratorMicroTurbine>();

  ASSERT_EQ(1,mchpClone.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(2,mchpClone.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(3,mchpClone.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());


  //Clone into another model
  Model model2;
  GeneratorMicroTurbine  mchpClone2 = coil.clone(model2).cast<GeneratorMicroTurbine>();

  ASSERT_EQ(1,mchpClone2.electricalPowerFunctionofTemperatureandElevationCurve().cast<CurveBiquadratic>().coefficient1Constant());
  ASSERT_EQ(2,mchpClone2.electricalEfficiencyFunctionofTemperatureCurve().cast<CurveCubic>().coefficient1Constant());
  ASSERT_EQ(3,mchpClone2.electricalEfficiencyFunctionofPartLoadRatioCurve().cast<CurveCubic>().coefficient1Constant());
  
}



TEST_F(ModelFixture, GeneratorPhotovoltaic_Delete) {
  Model model;

  {
    GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);
    PhotovoltaicPerformance performance = panel.photovoltaicPerformance();

    std::vector<IdfObject> removed = performance.remove();
    EXPECT_EQ(0, removed.size());
    EXPECT_NO_THROW(panel.photovoltaicPerformance());

    removed = panel.remove();
    ASSERT_EQ(2, removed.size());
    EXPECT_EQ(GeneratorPhotovoltaic::iddObjectType(), removed[0].iddObject().type());
    EXPECT_EQ(PhotovoltaicPerformanceSimple::iddObjectType(), removed[1].iddObject().type());
  }

  {
    GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::equivalentOneDiode(model);
    PhotovoltaicPerformance performance = panel.photovoltaicPerformance();

    std::vector<IdfObject> removed = performance.remove();
    EXPECT_EQ(0, removed.size());
    EXPECT_NO_THROW(panel.photovoltaicPerformance());

    removed = panel.remove();
    ASSERT_EQ(2, removed.size());
    EXPECT_EQ(GeneratorPhotovoltaic::iddObjectType(), removed[0].iddObject().type());
    EXPECT_EQ(PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType(), removed[1].iddObject().type());
  }

}

