/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../PhotovoltaicPerformanceEquivalentOneDiode.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PhotovoltaicPerformanceEquivalentOneDiode_Instantiate) {
  auto result = false;
  auto value = 0.1;

  Model model;

  PhotovoltaicPerformanceEquivalentOneDiode photovoltaicPerformanceEquivalentOneDiode(model);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.celltype(), "CrystallineSilicon");
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isCelltypeDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setCelltype("garbage");
  EXPECT_FALSE(result);
  result = photovoltaicPerformanceEquivalentOneDiode.setCelltype("AmorphousSilicon");
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.celltype(), "AmorphousSilicon");
  photovoltaicPerformanceEquivalentOneDiode.resetCelltype();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.celltype(), "CrystallineSilicon");

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.numberOfCellsInSeries(), 36);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isNumberOfCellsInSeriesDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setNumberofCellsinSeries(2);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.numberOfCellsInSeries(), 2);
  photovoltaicPerformanceEquivalentOneDiode.resetNumberofCellsinSeries();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.numberOfCellsInSeries(), 36);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.activeArea(), 0.89);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isActiveAreaDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setActiveArea(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.activeArea(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetActiveArea();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.activeArea(), 0.89);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.transmittanceAbsorptanceProduct(), 0.95);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isTransmittanceAbsorptanceProductDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setTransmittanceAbsorptanceProduct(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.transmittanceAbsorptanceProduct(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetTransmittanceAbsorptanceProduct();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.transmittanceAbsorptanceProduct(), 0.95);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.semiconductorBandgap(), 1.12);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isSemiconductorBandgapDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setSemiconductorBandgap(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.semiconductorBandgap(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetSemiconductorBandgap();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.semiconductorBandgap(), 1.12);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.shuntResistance(), 1000000);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isShuntResistanceDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setShuntResistance(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.shuntResistance(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetShuntResistance();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.shuntResistance(), 1000000);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.shortCircuitCurrent(), 6.5);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isShortCircuitCurrentDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setShortCircuitCurrent(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.shortCircuitCurrent(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetShortCircuitCurrent();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.shortCircuitCurrent(), 6.5);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.openCircuitVoltage(), 21.6);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isOpenCircuitVoltageDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setOpenCircuitVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.openCircuitVoltage(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetOpenCircuitVoltage();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.openCircuitVoltage(), 21.6);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.referenceTemperature(), 25);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isReferenceTemperatureDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setReferenceTemperature(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.referenceTemperature(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetReferenceTemperature();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.referenceTemperature(), 25);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.referenceInsolation(), 1000);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isReferenceInsolationDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setReferenceInsolation(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.referenceInsolation(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetReferenceInsolation();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.referenceInsolation(), 1000);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleCurrentatMaximumPower(), 5.9);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isModuleCurrentatMaximumPowerDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setModuleCurrentatMaximumPower(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleCurrentatMaximumPower(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetModuleCurrentatMaximumPower();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleCurrentatMaximumPower(), 5.9);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleVoltageatMaximumPower(), 17);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isModuleVoltageatMaximumPowerDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setModuleVoltageatMaximumPower(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleVoltageatMaximumPower(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetModuleVoltageatMaximumPower();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleVoltageatMaximumPower(), 17);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestAmbientTemperature(), 20);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isNominalOperatingCellTemperatureTestAmbientTemperatureDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setNominalOperatingCellTemperatureTestAmbientTemperature(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestAmbientTemperature(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetNominalOperatingCellTemperatureTestAmbientTemperature();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestAmbientTemperature(), 20);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestCellTemperature(), 40);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isNominalOperatingCellTemperatureTestCellTemperatureDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setNominalOperatingCellTemperatureTestCellTemperature(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestCellTemperature(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetNominalOperatingCellTemperatureTestCellTemperature();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestCellTemperature(), 40);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestInsolation(), 800);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isNominalOperatingCellTemperatureTestInsolationDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setNominalOperatingCellTemperatureTestInsolation(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestInsolation(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetNominalOperatingCellTemperatureTestInsolation();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.nominalOperatingCellTemperatureTestInsolation(), 800);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleHeatLossCoefficient(), 30);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isModuleHeatLossCoefficientDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setModuleHeatLossCoefficient(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleHeatLossCoefficient(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetModuleHeatLossCoefficient();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.moduleHeatLossCoefficient(), 30);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.totalHeatCapacity(), 50000);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isTotalHeatCapacityDefaulted());
  result = photovoltaicPerformanceEquivalentOneDiode.setTotalHeatCapacity(value);
  EXPECT_TRUE(result);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.totalHeatCapacity(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetTotalHeatCapacity();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.totalHeatCapacity(), 50000);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.temperatureCoefficientofShortCircuitCurrent(), 0.02);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isTemperatureCoefficientofShortCircuitCurrentDefaulted());
  photovoltaicPerformanceEquivalentOneDiode.setTemperatureCoefficientofShortCircuitCurrent(value);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.temperatureCoefficientofShortCircuitCurrent(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetTemperatureCoefficientofShortCircuitCurrent();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.temperatureCoefficientofShortCircuitCurrent(), 0.02);

  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.temperatureCoefficientofOpenCircuitVoltage(), -0.079);
  EXPECT_TRUE(photovoltaicPerformanceEquivalentOneDiode.isTemperatureCoefficientofOpenCircuitVoltageDefaulted());
  photovoltaicPerformanceEquivalentOneDiode.setTemperatureCoefficientofOpenCircuitVoltage(value);
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.temperatureCoefficientofOpenCircuitVoltage(), value);
  photovoltaicPerformanceEquivalentOneDiode.resetTemperatureCoefficientofOpenCircuitVoltage();
  EXPECT_EQ(photovoltaicPerformanceEquivalentOneDiode.temperatureCoefficientofOpenCircuitVoltage(), -0.079);
}
