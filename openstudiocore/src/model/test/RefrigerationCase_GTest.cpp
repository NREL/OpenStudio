/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCase_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RefrigerationCase_RefrigerationCase)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  
  ASSERT_EXIT ( 
  {  
     Model m; 
     ScheduleCompact cds(m);
     RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,RefrigerationCase_iddObjectType)
{    
	Model m; 

    ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	EXPECT_EQ( testRefrigerationCase.iddObjectType(), IddObjectType::OS_Refrigeration_Case);
}

TEST_F(ModelFixture,RefrigerationCase_RatedAmbientTemperature)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum> 0.0
    // default 23.9

    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedAmbientTemperature(), 24.0);  // Brian's value

    EXPECT_TRUE( testRefrigerationCase.setRatedAmbientTemperature(15));
    EXPECT_EQ( testRefrigerationCase.ratedAmbientTemperature(), 15);

    EXPECT_FALSE(testRefrigerationCase.setRatedAmbientTemperature(-15));

    EXPECT_FALSE(testRefrigerationCase.setRatedAmbientTemperature(0.0)); // boundary check

    testRefrigerationCase.setRatedAmbientTemperature(10.0);
    testRefrigerationCase.resetRatedAmbientTemperature();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedAmbientTemperature(), 23.9);   // IDD default value
}


TEST_F(ModelFixture,RefrigerationCase_RatedAmbientRelativeHumidity)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	//   minimum> 0.0
    //   maximum< 100.0
    //   default 55.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedAmbientRelativeHumidity(), 55.0);

    EXPECT_TRUE( testRefrigerationCase.setRatedAmbientRelativeHumidity(15.0));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedAmbientRelativeHumidity(), 15.0);

    EXPECT_FALSE( testRefrigerationCase.setRatedAmbientRelativeHumidity(-15.0));
    EXPECT_FALSE( testRefrigerationCase.setRatedAmbientRelativeHumidity(150.0));

    EXPECT_FALSE( testRefrigerationCase.setRatedAmbientRelativeHumidity(0.0));   //boundary check
    EXPECT_FALSE( testRefrigerationCase.setRatedAmbientRelativeHumidity(100.0)); //boundary check

    testRefrigerationCase.setRatedAmbientRelativeHumidity(10.0);
    testRefrigerationCase.resetRatedAmbientRelativeHumidity();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedAmbientRelativeHumidity(), 55.0);
}


TEST_F(ModelFixture,RefrigerationCase_RatedTotalCoolingCapacityperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    //    minimum> 0.0
    //    default 1900.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedTotalCoolingCapacityperUnitLength(), 1432.66); // Brian's supplied value

    EXPECT_TRUE( testRefrigerationCase.setRatedTotalCoolingCapacityperUnitLength(15));
    EXPECT_EQ( testRefrigerationCase.ratedTotalCoolingCapacityperUnitLength(), 15);

    EXPECT_FALSE( testRefrigerationCase.setRatedTotalCoolingCapacityperUnitLength(-15));

    EXPECT_FALSE( testRefrigerationCase.setRatedTotalCoolingCapacityperUnitLength(0.0)); // boundary check

    testRefrigerationCase.setRatedTotalCoolingCapacityperUnitLength(10.0);
    testRefrigerationCase.resetRatedTotalCoolingCapacityperUnitLength();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedTotalCoolingCapacityperUnitLength(), 1900.0); // reset brings back IDD default value
}


TEST_F(ModelFixture,RefrigerationCase_RatedLatentHeatRatio)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    //    minimum 0.0
    //    maximum 1.0
    //    default 0.3

    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedLatentHeatRatio(), 0.3);

    EXPECT_TRUE( testRefrigerationCase.setRatedLatentHeatRatio(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedLatentHeatRatio(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setRatedLatentHeatRatio(-1.0));
    EXPECT_FALSE( testRefrigerationCase.setRatedLatentHeatRatio(1.5));

    EXPECT_TRUE( testRefrigerationCase.setRatedLatentHeatRatio(0.0)); //boundary check
    EXPECT_TRUE( testRefrigerationCase.setRatedLatentHeatRatio(1.0));    //boundary check

    testRefrigerationCase.setRatedLatentHeatRatio(0.8);
    testRefrigerationCase.resetRatedLatentHeatRatio();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedLatentHeatRatio(), 0.3);
}

TEST_F(ModelFixture,RefrigerationCase_RatedRuntimeFraction)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    //  minimum> 0.0
    //  maximum 1.0
    //  default 0.85

    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedRuntimeFraction(), 0.85);

    EXPECT_TRUE( testRefrigerationCase.setRatedRuntimeFraction(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedRuntimeFraction(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setRatedRuntimeFraction(-1.0));
    EXPECT_FALSE( testRefrigerationCase.setRatedRuntimeFraction(1.5));

    EXPECT_FALSE( testRefrigerationCase.setRatedRuntimeFraction(0.0)); // boundary check
    EXPECT_TRUE( testRefrigerationCase.setRatedRuntimeFraction(1.0));  // boundary check

    testRefrigerationCase.setRatedRuntimeFraction(0.7);
    testRefrigerationCase.resetRatedRuntimeFraction();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.ratedRuntimeFraction(), 0.85);
}

TEST_F(ModelFixture,RefrigerationCase_CaseLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum> 0.0
    // default 3.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseLength(), 3.66);  // Brian's value

    EXPECT_TRUE( testRefrigerationCase.setCaseLength(5.3));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseLength(), 5.3);

    EXPECT_FALSE( testRefrigerationCase.setCaseLength(-1.0));

    EXPECT_FALSE( testRefrigerationCase.setCaseLength(0.0)); // boundary check

    testRefrigerationCase.setCaseLength(10.6);
    testRefrigerationCase.resetCaseLength();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseLength(), 3.0);
}

TEST_F(ModelFixture,RefrigerationCase_CaseOperatingTemperature)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	// maximum< 20.0
    // default 1.1

    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseOperatingTemperature(), 2.78); // Brian's value

    EXPECT_TRUE( testRefrigerationCase.setCaseOperatingTemperature(1.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseOperatingTemperature(), 1.5);

    EXPECT_FALSE( testRefrigerationCase.setCaseOperatingTemperature(28.5));

    EXPECT_FALSE( testRefrigerationCase.setCaseOperatingTemperature(20.0)); // boundary check

    testRefrigerationCase.setCaseOperatingTemperature(15.8);
    testRefrigerationCase.resetCaseOperatingTemperature();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseOperatingTemperature(), 1.1);
}

TEST_F(ModelFixture,RefrigerationCase_StandardCaseFanPowerperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	// minimum 0.0
    // default 75.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseFanPowerperUnitLength(), 41.01); // Brian's value

    EXPECT_TRUE( testRefrigerationCase.setStandardCaseFanPowerperUnitLength(18.9));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseFanPowerperUnitLength(), 18.9);

    EXPECT_FALSE( testRefrigerationCase.setStandardCaseFanPowerperUnitLength(-1.0));

    EXPECT_TRUE( testRefrigerationCase.setStandardCaseFanPowerperUnitLength(0.0)); // boundary check

    testRefrigerationCase.setStandardCaseFanPowerperUnitLength(11.7);
    testRefrigerationCase.resetStandardCaseFanPowerperUnitLength();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseFanPowerperUnitLength(), 75.0);  // IDD default value
}


TEST_F(ModelFixture,RefrigerationCase_OperatingCaseFanPowerperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum 0.0
    // default 75.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.operatingCaseFanPowerperUnitLength(), 41.01); // Brian's values

    EXPECT_TRUE( testRefrigerationCase.setOperatingCaseFanPowerperUnitLength(10.7));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.operatingCaseFanPowerperUnitLength(), 10.7);

    EXPECT_FALSE( testRefrigerationCase.setOperatingCaseFanPowerperUnitLength(-1.0));

    EXPECT_TRUE( testRefrigerationCase.setOperatingCaseFanPowerperUnitLength(0.0)); // boundary check

    testRefrigerationCase.setOperatingCaseFanPowerperUnitLength(20.6);
    testRefrigerationCase.resetOperatingCaseFanPowerperUnitLength();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.operatingCaseFanPowerperUnitLength(), 75.0);
}


TEST_F(ModelFixture,RefrigerationCase_StandardCaseLightingPowerperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	// default 90.0

	EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseLightingPowerperUnitLength(), 45.93); // Brian's values
	
	testRefrigerationCase.setStandardCaseLightingPowerperUnitLength(10.5);
	EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseLightingPowerperUnitLength(), 10.5);

	testRefrigerationCase.setStandardCaseLightingPowerperUnitLength(-10.5);
	EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseLightingPowerperUnitLength(), -10.5);

	testRefrigerationCase.setStandardCaseLightingPowerperUnitLength(20.4);
	testRefrigerationCase.resetStandardCaseLightingPowerperUnitLength();
	EXPECT_DOUBLE_EQ( testRefrigerationCase.standardCaseLightingPowerperUnitLength(), 90.0); // IDD default
}


TEST_F(ModelFixture,RefrigerationCase_InstalledCaseLightingPowerperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // note Default set equal to Standard Case Lighting Power per Unit Length

    EXPECT_DOUBLE_EQ( testRefrigerationCase.installedCaseLightingPowerperUnitLength().get(), 45.93); // Brian's value

    testRefrigerationCase.setInstalledCaseLightingPowerperUnitLength(20.5);
    EXPECT_DOUBLE_EQ( testRefrigerationCase.installedCaseLightingPowerperUnitLength().get(), 20.5);

    testRefrigerationCase.setInstalledCaseLightingPowerperUnitLength(-20.5);
    EXPECT_DOUBLE_EQ( testRefrigerationCase.installedCaseLightingPowerperUnitLength().get(), -20.5);

    testRefrigerationCase.setInstalledCaseLightingPowerperUnitLength(10.7);
    testRefrigerationCase.resetInstalledCaseLightingPowerperUnitLength();
    EXPECT_FALSE( testRefrigerationCase.installedCaseLightingPowerperUnitLength()); // Default
}

TEST_F(ModelFixture,RefrigerationCase_FractionofLightingEnergytoCase)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum 0.0
    // maximum 1.0
    // default 1.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.fractionofLightingEnergytoCase(), 1.0);

    EXPECT_TRUE( testRefrigerationCase.setFractionofLightingEnergytoCase(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.fractionofLightingEnergytoCase(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setFractionofLightingEnergytoCase(-1.0));
    EXPECT_FALSE( testRefrigerationCase.setFractionofLightingEnergytoCase(1.5));

    EXPECT_TRUE( testRefrigerationCase.setFractionofLightingEnergytoCase(0.0)); // boundary check
    EXPECT_TRUE( testRefrigerationCase.setFractionofLightingEnergytoCase(1.0));  // boundary check

    testRefrigerationCase.setFractionofLightingEnergytoCase(0.7);
    testRefrigerationCase.resetFractionofLightingEnergytoCase();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.fractionofLightingEnergytoCase(), 1.0);
}

TEST_F(ModelFixture,RefrigerationCase_CaseAntiSweatHeaterPowerperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	// minimum 0.0
    // default 0.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseAntiSweatHeaterPowerperUnitLength(), 0.0);

    EXPECT_TRUE( testRefrigerationCase.setCaseAntiSweatHeaterPowerperUnitLength(10.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseAntiSweatHeaterPowerperUnitLength(), 10.5);

    EXPECT_FALSE( testRefrigerationCase.setCaseAntiSweatHeaterPowerperUnitLength(-1.0));

    EXPECT_TRUE( testRefrigerationCase.setCaseAntiSweatHeaterPowerperUnitLength(0.0)); // boundary check

    testRefrigerationCase.setCaseAntiSweatHeaterPowerperUnitLength(10.7);
    testRefrigerationCase.resetCaseAntiSweatHeaterPowerperUnitLength();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseAntiSweatHeaterPowerperUnitLength(), 0.0);
}

TEST_F(ModelFixture,RefrigerationCase_HumidityatZeroAntiSweatHeaterEnergy)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

	//	default -10.0
    //    note This field is only applicable to Linear AS heater control type
    //    note Zone relative humidity (%) where anti-sweat heater energy is zero

    // TODO: implement the 'note' clause
    EXPECT_DOUBLE_EQ( testRefrigerationCase.humidityatZeroAntiSweatHeaterEnergy(), -10.0);

    testRefrigerationCase.setHumidityatZeroAntiSweatHeaterEnergy(10.5);
    EXPECT_DOUBLE_EQ( testRefrigerationCase.humidityatZeroAntiSweatHeaterEnergy(), 10.5);

    testRefrigerationCase.setHumidityatZeroAntiSweatHeaterEnergy(-10.5);
    EXPECT_DOUBLE_EQ( testRefrigerationCase.humidityatZeroAntiSweatHeaterEnergy(), -10.5);

    testRefrigerationCase.setHumidityatZeroAntiSweatHeaterEnergy(0.7);
    testRefrigerationCase.resetHumidityatZeroAntiSweatHeaterEnergy();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.humidityatZeroAntiSweatHeaterEnergy(), -10.0);
}

TEST_F(ModelFixture,RefrigerationCase_CaseHeight)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum 0.0
    // default 1.5
    // note This field only applicable to Heat Balance Method AS heater control type
    // note Height must be greater than zero if Heat Balance Method AS heater control is selected

    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseHeight(), 1.5);

    EXPECT_TRUE( testRefrigerationCase.setCaseHeight(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseHeight(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setCaseHeight(-1.0));

    EXPECT_TRUE( testRefrigerationCase.setCaseHeight(0.0)); // boundary check

    testRefrigerationCase.setCaseHeight(10.2);
    testRefrigerationCase.resetCaseHeight();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseHeight(), 1.5);
}


TEST_F(ModelFixture,RefrigerationCase_FractionofAntiSweatHeaterEnergytoCase)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    //  minimum 0.0
    //  maximum 1.0
    //  default 1.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.fractionofAntiSweatHeaterEnergytoCase(), 1.0);

    EXPECT_TRUE( testRefrigerationCase.setFractionofAntiSweatHeaterEnergytoCase(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.fractionofAntiSweatHeaterEnergytoCase(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setFractionofAntiSweatHeaterEnergytoCase(-1.0));
    EXPECT_FALSE( testRefrigerationCase.setFractionofAntiSweatHeaterEnergytoCase(1.5));

    EXPECT_TRUE( testRefrigerationCase.setFractionofAntiSweatHeaterEnergytoCase(0.0)); // boundary check
    EXPECT_TRUE( testRefrigerationCase.setFractionofAntiSweatHeaterEnergytoCase(1.0));  // boundary check

    testRefrigerationCase.setFractionofAntiSweatHeaterEnergytoCase(0.7);
    testRefrigerationCase.resetFractionofAntiSweatHeaterEnergytoCase();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.fractionofAntiSweatHeaterEnergytoCase(), 1.0);
}

TEST_F(ModelFixture,RefrigerationCase_CaseDefrostPowerperUnitLength)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum 0.0
    // default 0.0
    // note Used to evaluate load on case as well as power or heat consumption

    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseDefrostPowerperUnitLength(), 0.0);

    EXPECT_TRUE( testRefrigerationCase.setCaseDefrostPowerperUnitLength(14.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseDefrostPowerperUnitLength(), 14.5);

    EXPECT_FALSE( testRefrigerationCase.setCaseDefrostPowerperUnitLength(-1.0));

    EXPECT_TRUE( testRefrigerationCase.setCaseDefrostPowerperUnitLength(0.0)); // boundary check

    testRefrigerationCase.setCaseDefrostPowerperUnitLength(0.7);
    testRefrigerationCase.resetCaseDefrostPowerperUnitLength();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.caseDefrostPowerperUnitLength(), 0.0);
}



TEST_F(ModelFixture,RefrigerationCase_UnderCaseHVACReturnAirFraction)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum 0.0
    // maximum 1.0
    // default 0.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.underCaseHVACReturnAirFraction(), 0.0);

    EXPECT_TRUE( testRefrigerationCase.setUnderCaseHVACReturnAirFraction(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.underCaseHVACReturnAirFraction(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setUnderCaseHVACReturnAirFraction(-1.0));
    EXPECT_FALSE( testRefrigerationCase.setUnderCaseHVACReturnAirFraction(1.5));

    EXPECT_TRUE( testRefrigerationCase.setUnderCaseHVACReturnAirFraction(0.0)); // boundary check
    EXPECT_TRUE( testRefrigerationCase.setUnderCaseHVACReturnAirFraction(1.0));  // boundary check

    testRefrigerationCase.setUnderCaseHVACReturnAirFraction(0.7);
    testRefrigerationCase.resetUnderCaseHVACReturnAirFraction();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.underCaseHVACReturnAirFraction(), 0.0);
}

TEST_F(ModelFixture,RefrigerationCase_DesignEvaporatorTemperatureorBrineInletTemperature)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // minimum -70.0
    // maximum 40.0
    // note Required for detailed refrigeration system, not for compressor rack
    // note For a DX system, enter the saturated temperature for refrigerant pressure leaving case
    // note For a brine-cooled cooled (secondary system) case, enter the brine inlet temperature
    // note Default is 5 C less than case operating temperature

    EXPECT_DOUBLE_EQ( testRefrigerationCase.designEvaporatorTemperatureorBrineInletTemperature().get(), -5.56);  // Brian's value

    EXPECT_TRUE( testRefrigerationCase.setDesignEvaporatorTemperatureorBrineInletTemperature(0.5));
    EXPECT_DOUBLE_EQ( testRefrigerationCase.designEvaporatorTemperatureorBrineInletTemperature().get(), 0.5);

    EXPECT_FALSE( testRefrigerationCase.setDesignEvaporatorTemperatureorBrineInletTemperature(-75.0));
    EXPECT_FALSE( testRefrigerationCase.setDesignEvaporatorTemperatureorBrineInletTemperature(45.0));

    EXPECT_TRUE( testRefrigerationCase.setDesignEvaporatorTemperatureorBrineInletTemperature(-70.0)); // boundary check
    EXPECT_TRUE( testRefrigerationCase.setDesignEvaporatorTemperatureorBrineInletTemperature(40.0));  // boundary check

    testRefrigerationCase.setDesignEvaporatorTemperatureorBrineInletTemperature(0.7);
    testRefrigerationCase.resetDesignEvaporatorTemperatureorBrineInletTemperature();
    EXPECT_FALSE( testRefrigerationCase.designEvaporatorTemperatureorBrineInletTemperature());
}

TEST_F(ModelFixture,RefrigerationCase_AverageRefrigerantChargeInventory)
{    
	Model m; 
	ScheduleCompact cds(m);
    RefrigerationCase testRefrigerationCase = RefrigerationCase(m, cds);

    // default 0.0

    EXPECT_DOUBLE_EQ( testRefrigerationCase.averageRefrigerantChargeInventory(), 0.0);

    testRefrigerationCase.setAverageRefrigerantChargeInventory(12.5);
    EXPECT_DOUBLE_EQ( testRefrigerationCase.averageRefrigerantChargeInventory(), 12.5);

    testRefrigerationCase.setAverageRefrigerantChargeInventory(10.7);
    testRefrigerationCase.resetAverageRefrigerantChargeInventory();
    EXPECT_DOUBLE_EQ( testRefrigerationCase.averageRefrigerantChargeInventory(), 0.0);
}

TEST_F(ModelFixture, RefrigerationCase_Remove)
{
	Model model; 
	ScheduleCompact cds(model);
	RefrigerationCase testRefrigerationCase = RefrigerationCase(model, cds);

    std::vector<RefrigerationCase> testAllRefrigerationCases = model.getModelObjects<RefrigerationCase>();
    EXPECT_EQ(1, testAllRefrigerationCases.size());

    testRefrigerationCase.remove();

    testAllRefrigerationCases = model.getModelObjects<RefrigerationCase>();
    EXPECT_EQ(0, testAllRefrigerationCases.size());
}

TEST_F(ModelFixture, RefrigerationCase_CloneOneModelWithDefaultData)
{
	Model model; 
	ScheduleCompact cds(model);
	RefrigerationCase testObject = RefrigerationCase(model, cds);

	RefrigerationCase testObjectClone = testObject.clone(model).cast<RefrigerationCase>();

	EXPECT_NE(testObject.latentCaseCreditCurve().handle(), testObjectClone.latentCaseCreditCurve().handle());

	EXPECT_DOUBLE_EQ( 24.0, testObjectClone.ratedAmbientTemperature() );  
	EXPECT_DOUBLE_EQ( 55.0, testObjectClone.ratedAmbientRelativeHumidity() );
	EXPECT_DOUBLE_EQ( 1432.66, testObjectClone.ratedTotalCoolingCapacityperUnitLength() ); 
	EXPECT_DOUBLE_EQ( 0.3, testObjectClone.ratedLatentHeatRatio() );  
	EXPECT_DOUBLE_EQ( 0.85, testObjectClone.ratedRuntimeFraction() );
	EXPECT_DOUBLE_EQ( 3.66, testObjectClone.caseLength()); 
	EXPECT_DOUBLE_EQ( 2.78, testObjectClone.caseOperatingTemperature());  
	EXPECT_EQ( "CaseTemperatureMethod", testObjectClone.latentCaseCreditCurveType() );
	EXPECT_DOUBLE_EQ( 41.01, testObjectClone.standardCaseFanPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 41.01, testObjectClone.operatingCaseFanPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 45.93, testObjectClone.standardCaseLightingPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 45.93, testObjectClone.installedCaseLightingPowerperUnitLength().get());
	EXPECT_EQ( "None", testObjectClone.antiSweatHeaterControlType() );
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone.fractionofLightingEnergytoCase() );
	EXPECT_DOUBLE_EQ( -10.0, testObjectClone.humidityatZeroAntiSweatHeaterEnergy() );
	EXPECT_DOUBLE_EQ( 1.5, testObjectClone.caseHeight() );
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone.fractionofAntiSweatHeaterEnergytoCase() );
	EXPECT_DOUBLE_EQ( 0.0, testObjectClone.caseDefrostPowerperUnitLength() );
	EXPECT_DOUBLE_EQ( 0.0, testObjectClone.underCaseHVACReturnAirFraction() );
	EXPECT_DOUBLE_EQ( -5.56, testObjectClone.designEvaporatorTemperatureorBrineInletTemperature().get() );
	EXPECT_EQ( "OffCycle", testObjectClone.caseDefrostType() );
    EXPECT_EQ( "None", testObjectClone.defrostEnergyCorrectionCurveType() );
}


TEST_F(ModelFixture, RefrigerationCase_CloneOneModelWithCustomData)
{
 	Model model; 
	ScheduleCompact cds(model);
	RefrigerationCase testObject = RefrigerationCase(model, cds);

	CurveCubic latentCaseCreditCurve = CurveCubic(model);
	latentCaseCreditCurve.setName("Refrigeration Case Latent Credit Curve Test");
	latentCaseCreditCurve.setCoefficient1Constant(0.06757726281);
	latentCaseCreditCurve.setCoefficient2x(0.001078032);
	latentCaseCreditCurve.setCoefficient3xPOW2(-0.0000602558);
	latentCaseCreditCurve.setCoefficient4xPOW3(0.00000123732);
	latentCaseCreditCurve.setMinimumValueofx(-35);
	latentCaseCreditCurve.setMaximumValueofx(20);
	testObject.setLatentCaseCreditCurve(latentCaseCreditCurve);

	testObject.setRatedAmbientTemperature(28.5);  
	testObject.setRatedAmbientRelativeHumidity(58.0);
	testObject.setRatedTotalCoolingCapacityperUnitLength(1200.1); 
	testObject.setRatedLatentHeatRatio(0.5);  
	testObject.setRatedRuntimeFraction(0.5);
	testObject.setCaseLength(1.0); 
	testObject.setCaseOperatingTemperature(1.0);  
	testObject.setLatentCaseCreditCurveType("RelativeHumidityMethod");
	testObject.setStandardCaseFanPowerperUnitLength(45.0);
	testObject.setOperatingCaseFanPowerperUnitLength(45.0);
	testObject.setStandardCaseLightingPowerperUnitLength(30.0);
	testObject.setInstalledCaseLightingPowerperUnitLength(30.0);
	testObject.setFractionofLightingEnergytoCase(0.5);
	testObject.setHumidityatZeroAntiSweatHeaterEnergy(5.0);
	testObject.setCaseHeight(2.5);
	testObject.setFractionofAntiSweatHeaterEnergytoCase(0.5);
	testObject.setCaseDefrostPowerperUnitLength(1.0);
	testObject.setUnderCaseHVACReturnAirFraction(0.5);
	testObject.setDesignEvaporatorTemperatureorBrineInletTemperature(-1.0);
	testObject.setCaseDefrostType("HotGas");

	RefrigerationCase testObjectClone = testObject.clone(model).cast<RefrigerationCase>();

	EXPECT_NE(testObject.latentCaseCreditCurve().handle(), testObjectClone.latentCaseCreditCurve().handle());
	EXPECT_NE(latentCaseCreditCurve.handle(), testObjectClone.latentCaseCreditCurve().handle());
	EXPECT_DOUBLE_EQ( 28.5, testObjectClone.ratedAmbientTemperature() );  
	EXPECT_DOUBLE_EQ( 58.0, testObjectClone.ratedAmbientRelativeHumidity() );
	EXPECT_DOUBLE_EQ( 1200.1, testObjectClone.ratedTotalCoolingCapacityperUnitLength() ); 
	EXPECT_DOUBLE_EQ( 0.5, testObjectClone.ratedLatentHeatRatio() );  
	EXPECT_DOUBLE_EQ( 0.5, testObjectClone.ratedRuntimeFraction() );
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone.caseLength()); 
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone.caseOperatingTemperature());  
	EXPECT_EQ( "RelativeHumidityMethod", testObjectClone.latentCaseCreditCurveType() );
	EXPECT_DOUBLE_EQ( 45.0, testObjectClone.standardCaseFanPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 45.0, testObjectClone.operatingCaseFanPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 30.0, testObjectClone.standardCaseLightingPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 30.0, testObjectClone.installedCaseLightingPowerperUnitLength().get());
	EXPECT_DOUBLE_EQ( 0.5, testObjectClone.fractionofLightingEnergytoCase() );
	EXPECT_DOUBLE_EQ( 5.0, testObjectClone.humidityatZeroAntiSweatHeaterEnergy() );
	EXPECT_DOUBLE_EQ( 2.5, testObjectClone.caseHeight() );
	EXPECT_DOUBLE_EQ( 0.5, testObjectClone.fractionofAntiSweatHeaterEnergytoCase() );
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone.caseDefrostPowerperUnitLength() );
	EXPECT_DOUBLE_EQ( 0.5, testObjectClone.underCaseHVACReturnAirFraction() );
	EXPECT_DOUBLE_EQ( -1.0, testObjectClone.designEvaporatorTemperatureorBrineInletTemperature().get() );
	EXPECT_EQ( "HotGas", testObjectClone.caseDefrostType() );
}

TEST_F(ModelFixture, RefrigerationCase_CloneTwoModelsWithDefaultData)
{
	Model model;
	ScheduleCompact cds(model);
	RefrigerationCase testObject = RefrigerationCase(model, cds);

	RefrigerationCase testObjectClone = testObject.clone(model).cast<RefrigerationCase>();

	Model model2;

	std::vector<CurveCubic> refrigerationCaseCurves2 = model2.getModelObjects<CurveCubic>();
	EXPECT_EQ(0, refrigerationCaseCurves2.size());

	RefrigerationCase testObjectClone2 = testObject.clone(model2).cast<RefrigerationCase>();

	std::vector<CurveCubic> refrigerationCaseCurves = model.getModelObjects<CurveCubic>();
	EXPECT_EQ(2, refrigerationCaseCurves.size());

	refrigerationCaseCurves2 = model2.getModelObjects<CurveCubic>();
	EXPECT_EQ(1, refrigerationCaseCurves2.size());

	for(std::vector<CurveCubic>::iterator it = refrigerationCaseCurves.begin(); it != refrigerationCaseCurves.end(); ++it) {
		EXPECT_TRUE(it->parent());
	}

	for(std::vector<CurveCubic>::iterator it = refrigerationCaseCurves2.begin(); it != refrigerationCaseCurves2.end(); ++it) {
		EXPECT_TRUE(it->parent());
	}

	EXPECT_NE(testObject.latentCaseCreditCurve().handle(), testObjectClone.latentCaseCreditCurve().handle());
	EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());

	EXPECT_DOUBLE_EQ( 24.0, testObjectClone2.ratedAmbientTemperature() );  
	EXPECT_DOUBLE_EQ( 55.0, testObjectClone2.ratedAmbientRelativeHumidity() );
	EXPECT_DOUBLE_EQ( 1432.66, testObjectClone2.ratedTotalCoolingCapacityperUnitLength() ); 
	EXPECT_DOUBLE_EQ( 0.3, testObjectClone2.ratedLatentHeatRatio() );  
	EXPECT_DOUBLE_EQ( 0.85, testObjectClone2.ratedRuntimeFraction() );
	EXPECT_DOUBLE_EQ( 3.66, testObjectClone2.caseLength()); 
	EXPECT_DOUBLE_EQ( 2.78, testObjectClone2.caseOperatingTemperature());  
	EXPECT_EQ( "CaseTemperatureMethod", testObjectClone2.latentCaseCreditCurveType() );
	EXPECT_DOUBLE_EQ( 41.01, testObjectClone2.standardCaseFanPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 41.01, testObjectClone2.operatingCaseFanPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 45.93, testObjectClone2.standardCaseLightingPowerperUnitLength());
	EXPECT_DOUBLE_EQ( 45.93, testObjectClone2.installedCaseLightingPowerperUnitLength().get());
	EXPECT_EQ( "None", testObjectClone2.antiSweatHeaterControlType() );
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone2.fractionofLightingEnergytoCase() );
	EXPECT_DOUBLE_EQ( -10.0, testObjectClone2.humidityatZeroAntiSweatHeaterEnergy() );
	EXPECT_DOUBLE_EQ( 1.5, testObjectClone2.caseHeight() );
	EXPECT_DOUBLE_EQ( 1.0, testObjectClone2.fractionofAntiSweatHeaterEnergytoCase() );
	EXPECT_DOUBLE_EQ( 0.0, testObjectClone2.caseDefrostPowerperUnitLength() );
	EXPECT_DOUBLE_EQ( 0.0, testObjectClone2.underCaseHVACReturnAirFraction() );
	EXPECT_DOUBLE_EQ( -5.56, testObjectClone2.designEvaporatorTemperatureorBrineInletTemperature().get() );
	EXPECT_EQ( "OffCycle", testObjectClone2.caseDefrostType() );
    EXPECT_EQ( "None", testObjectClone2.defrostEnergyCorrectionCurveType() );
}
