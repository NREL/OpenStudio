/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../TableLookup.hpp"
#include "../TableLookup_Impl.hpp"
#include "../TableIndependentVariable.hpp"
#include "../TableIndependentVariable_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CurveQuadLinear.hpp"
#include "../CurveQuadLinear_Impl.hpp"
#include "../CurveQuintLinear.hpp"
#include "../CurveQuintLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, TableLookup_GettersSetters) {
  Model m;
  TableLookup tableLookup(m);

  // Defaults
  EXPECT_EQ("None", tableLookup.normalizationMethod());
  EXPECT_EQ(1.0, tableLookup.normalizationDivisor());
  EXPECT_FALSE(tableLookup.minimumOutput());
  EXPECT_FALSE(tableLookup.maximumOutput());
  EXPECT_EQ("Dimensionless", tableLookup.outputUnitType());
  EXPECT_EQ(0u, tableLookup.outputValues().size());
  EXPECT_EQ(0u, tableLookup.independentVariables().size());

  // Normalization Method: Required String
  EXPECT_TRUE(tableLookup.setNormalizationMethod("DivisorOnly"));
  EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());
  // Bad Value
  EXPECT_FALSE(tableLookup.setNormalizationMethod("BADENUM"));
  EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());

  // Normalization Divisor: Required Double
  EXPECT_TRUE(tableLookup.setNormalizationDivisor(0.5));
  EXPECT_EQ(0.5, tableLookup.normalizationDivisor());

  // #5145 - We reject 0 as a normalization divisor
  EXPECT_FALSE(tableLookup.setNormalizationDivisor(0.0));
  EXPECT_EQ(0.5, tableLookup.normalizationDivisor());

  // Minimum Output: Optional Double
  EXPECT_TRUE(tableLookup.setMinimumOutput(0.6));
  ASSERT_TRUE(tableLookup.minimumOutput());
  EXPECT_EQ(0.6, tableLookup.minimumOutput().get());
  tableLookup.resetMinimumOutput();
  EXPECT_FALSE(tableLookup.minimumOutput());

  // Maximum Output: Optional Double
  EXPECT_TRUE(tableLookup.setMaximumOutput(0.7));
  ASSERT_TRUE(tableLookup.maximumOutput());
  EXPECT_EQ(0.7, tableLookup.maximumOutput().get());
  tableLookup.resetMaximumOutput();
  EXPECT_FALSE(tableLookup.maximumOutput());

  // Output Unit Type: Required String
  EXPECT_TRUE(tableLookup.setOutputUnitType("Capacity"));
  EXPECT_EQ("Capacity", tableLookup.outputUnitType());
  // Bad Value
  EXPECT_FALSE(tableLookup.setOutputUnitType("BADENUM"));
  EXPECT_EQ("Capacity", tableLookup.outputUnitType());

  // OutputValues
  EXPECT_TRUE(tableLookup.addOutputValue(1.0));
  EXPECT_EQ(1u, tableLookup.outputValues().size());
  EXPECT_TRUE(tableLookup.addOutputValue(2.0));
  EXPECT_TRUE(tableLookup.addOutputValue(3.0));
  EXPECT_EQ(3u, tableLookup.numberofOutputValues());
  {
    std::vector<double> outputValues = tableLookup.outputValues();
    ASSERT_EQ(3u, outputValues.size());
    EXPECT_EQ(1.0, outputValues[0]);
    EXPECT_EQ(2.0, outputValues[1]);
    EXPECT_EQ(3.0, outputValues[2]);
  }

  EXPECT_TRUE(tableLookup.removeOutputValue(1));
  {
    std::vector<double> outputValues = tableLookup.outputValues();
    ASSERT_EQ(2, outputValues.size());
    EXPECT_EQ(1.0, outputValues[0]);
    EXPECT_EQ(3.0, outputValues[1]);
  }

  tableLookup.removeAllOutputValues();
  EXPECT_EQ(0u, tableLookup.outputValues().size());
  EXPECT_EQ(0u, tableLookup.numberofOutputValues());

  EXPECT_TRUE(tableLookup.addOutputValue(10.0));
  EXPECT_EQ(1u, tableLookup.outputValues().size());

  // Clears any existing values first
  EXPECT_TRUE(tableLookup.setOutputValues({1.0, 2.0, 3.0, 4.0}));
  EXPECT_EQ(4u, tableLookup.numberofOutputValues());
  {
    std::vector<double> outputValues = tableLookup.outputValues();
    ASSERT_EQ(4u, outputValues.size());
    EXPECT_EQ(1.0, outputValues[0]);
    EXPECT_EQ(2.0, outputValues[1]);
    EXPECT_EQ(3.0, outputValues[2]);
    EXPECT_EQ(4.0, outputValues[3]);
  }

  // IndependentVariables
  TableIndependentVariable independentVariable1(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable1));
  EXPECT_EQ(1u, tableLookup.independentVariables().size());

  TableIndependentVariable independentVariable2(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable2));
  TableIndependentVariable independentVariable3(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable3));
  EXPECT_EQ(3u, tableLookup.independentVariables().size());

  EXPECT_TRUE(tableLookup.removeIndependentVariable(independentVariable2));
  EXPECT_EQ(2u, tableLookup.independentVariables().size());
  tableLookup.removeAllIndependentVariables();
  EXPECT_EQ(0u, tableLookup.independentVariables().size());
}

TEST_F(ModelFixture, TableLookup_Clone) {
  // Test cloning the TableLookup, make sure the relationship with TableIndependentVariable is sane
  Model m;

  TableLookup tableLookup(m);
  TableIndependentVariable independentVariable1(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable1));

  EXPECT_EQ(1, m.getConcreteModelObjects<TableLookup>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ModelObjectList>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<TableIndependentVariable>().size());
  EXPECT_EQ(1, tableLookup.independentVariables().size());

  // Clone into the same model
  {
    auto tableLookupClone = tableLookup.clone(m).cast<TableLookup>();
    EXPECT_EQ(2, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1, m.getConcreteModelObjects<TableIndependentVariable>().size());

    ASSERT_EQ(1, tableLookup.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookup.independentVariables().front());

    ASSERT_EQ(1, tableLookupClone.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookupClone.independentVariables().front());
  }

  // Clone into another model
  {
    Model m2;
    auto tableLookupClone2 = tableLookup.clone(m2).cast<TableLookup>();

    // Check original model is unaffected
    EXPECT_EQ(2, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1, m.getConcreteModelObjects<TableIndependentVariable>().size());

    ASSERT_EQ(1, tableLookup.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookup.independentVariables().front());

    // Check that the TableLookup and TableLookupIndependentVariable are carried into new model
    EXPECT_EQ(1, m2.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<TableIndependentVariable>().size());

    ASSERT_EQ(1, tableLookupClone2.independentVariables().size());
    EXPECT_NE(independentVariable1, tableLookupClone2.independentVariables().front());
  }

  BoilerHotWater boiler(m);
  EXPECT_TRUE(boiler.setNormalizedBoilerEfficiencyCurve(tableLookup));
  EXPECT_EQ(6, m.numObjects());
  EXPECT_EQ(2, m.getConcreteModelObjects<TableLookup>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<ModelObjectList>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<TableIndependentVariable>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<BoilerHotWater>().size());

  EXPECT_EQ(1, tableLookup.independentVariables().size());

  // Clone a component that references a TableLookup into another model
  {
    Model m2;
    auto boilerClone = boiler.clone(m2).cast<BoilerHotWater>();

    ASSERT_TRUE(boilerClone.normalizedBoilerEfficiencyCurve());
    auto tableLookupClone2 = boilerClone.normalizedBoilerEfficiencyCurve()->cast<TableLookup>();

    // Check original model is unaffected
    EXPECT_EQ(6, m.numObjects());
    EXPECT_EQ(2, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1, m.getConcreteModelObjects<TableIndependentVariable>().size());
    EXPECT_EQ(1, m.getConcreteModelObjects<BoilerHotWater>().size());

    EXPECT_EQ(1, tableLookup.independentVariables().size());

    // Check that the TableLookup and TableLookupIndependentVariable are carried into the new model
    EXPECT_EQ(4, m2.numObjects());
    EXPECT_EQ(1, m2.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<TableIndependentVariable>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<BoilerHotWater>().size());

    ASSERT_EQ(1, tableLookupClone2.independentVariables().size());
    EXPECT_NE(independentVariable1, tableLookupClone2.independentVariables().front());
  }
}

TEST_F(ModelFixture, TableLookup_Remove) {
  // Make sure the relationship with TableIndependentVariable is sane

  {
    // Removing a TableLookup that is the sole owner of a TableIndependentVariable: should delete all
    Model m;

    TableLookup tableLookup(m);
    TableIndependentVariable independentVariable1(m);
    EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable1));

    EXPECT_EQ(1u, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<TableIndependentVariable>().size());
    ASSERT_EQ(1u, tableLookup.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookup.independentVariables().front());

    tableLookup.remove();
    EXPECT_EQ(0u, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(0u, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0u, m.getConcreteModelObjects<TableIndependentVariable>().size());
  }

  {
    // Two TableLookups sharing a single TableIndependentVariable: remove one shouldn't affect the other
    Model m;

    TableIndependentVariable independentVariable1(m);

    TableLookup tableLookup1(m);
    EXPECT_TRUE(tableLookup1.addIndependentVariable(independentVariable1));

    TableLookup tableLookup2(m);
    EXPECT_TRUE(tableLookup2.addIndependentVariable(independentVariable1));

    EXPECT_EQ(2u, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<TableIndependentVariable>().size());

    ASSERT_EQ(1u, tableLookup1.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookup1.independentVariables().front());
    ASSERT_EQ(1u, tableLookup2.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookup2.independentVariables().front());

    tableLookup1.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<TableLookup>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<TableIndependentVariable>().size());

    ASSERT_EQ(1u, tableLookup2.independentVariables().size());
    EXPECT_EQ(independentVariable1, tableLookup2.independentVariables().front());
  }
}

TEST_F(ModelFixture, TableLookup_CoilWaterToAirHeatPumpEquationFit) {
  // Test for #4946 - WaterToAirHeatPumpEquationFit curves do not support Table:Lookup

  Model m;

  // test ctor 1
  {
    // ctor maintains backward compatibility
    CoilCoolingWaterToAirHeatPumpEquationFit cc(m);
    ASSERT_TRUE(cc.totalCoolingCapacityCurve().optionalCast<CurveQuadLinear>());
    ASSERT_TRUE(cc.sensibleCoolingCapacityCurve().optionalCast<CurveQuintLinear>());
    ASSERT_TRUE(cc.coolingPowerConsumptionCurve().optionalCast<CurveQuadLinear>());

    CoilHeatingWaterToAirHeatPumpEquationFit ch(m);
    ASSERT_TRUE(ch.heatingCapacityCurve().optionalCast<CurveQuadLinear>());
    ASSERT_TRUE(ch.heatingPowerConsumptionCurve().optionalCast<CurveQuadLinear>());
  }

  // test ctor 2
  {
    // ctor maintains backward compatibility
    CurveQuadLinear tccc(m);
    CurveQuintLinear sccc(m);
    CurveQuadLinear cpcc(m);
    CoilCoolingWaterToAirHeatPumpEquationFit cc(m, tccc, sccc, cpcc);
    ASSERT_TRUE(cc.totalCoolingCapacityCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(tccc, cc.totalCoolingCapacityCurve());
    ASSERT_TRUE(cc.sensibleCoolingCapacityCurve().optionalCast<CurveQuintLinear>());
    EXPECT_EQ(sccc, cc.sensibleCoolingCapacityCurve());
    ASSERT_TRUE(cc.coolingPowerConsumptionCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(cpcc, cc.coolingPowerConsumptionCurve());

    CurveQuadLinear hcc(m);
    CurveQuadLinear hpcc(m);
    CoilHeatingWaterToAirHeatPumpEquationFit ch(m, hcc, hpcc);
    ASSERT_TRUE(ch.heatingCapacityCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(hcc, ch.heatingCapacityCurve());
    ASSERT_TRUE(ch.heatingPowerConsumptionCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(hpcc, ch.heatingPowerConsumptionCurve());
  }

  {
    // ctor can now handle table lookup curves
    TableLookup tl1(m);
    TableLookup tl2(m);
    TableLookup tl3(m);
    CoilCoolingWaterToAirHeatPumpEquationFit cc(m, tl1, tl2, tl3);
    ASSERT_TRUE(cc.totalCoolingCapacityCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tl1, cc.totalCoolingCapacityCurve());
    ASSERT_TRUE(cc.sensibleCoolingCapacityCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tl2, cc.sensibleCoolingCapacityCurve());
    ASSERT_TRUE(cc.coolingPowerConsumptionCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tl3, cc.coolingPowerConsumptionCurve());

    TableLookup tl4(m);
    TableLookup tl5(m);
    CoilHeatingWaterToAirHeatPumpEquationFit ch(m, tl4, tl5);
    ASSERT_TRUE(ch.heatingCapacityCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tl4, ch.heatingCapacityCurve());
    ASSERT_TRUE(ch.heatingPowerConsumptionCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tl5, ch.heatingPowerConsumptionCurve());
  }

  // test new setter/getter
  {
    CoilCoolingWaterToAirHeatPumpEquationFit cc(m);
    CoilHeatingWaterToAirHeatPumpEquationFit ch(m);

    // setter maintains backward compatibility
    CurveQuadLinear cql1(m);
    EXPECT_TRUE(cc.setTotalCoolingCapacityCurve(cql1));
    ASSERT_TRUE(cc.totalCoolingCapacityCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(cql1, cc.totalCoolingCapacityCurve());

    CurveQuintLinear cql2(m);
    EXPECT_TRUE(cc.setSensibleCoolingCapacityCurve(cql2));
    ASSERT_TRUE(cc.sensibleCoolingCapacityCurve().optionalCast<CurveQuintLinear>());
    EXPECT_EQ(cql2, cc.sensibleCoolingCapacityCurve());

    CurveQuadLinear cql3(m);
    EXPECT_TRUE(cc.setCoolingPowerConsumptionCurve(cql3));
    ASSERT_TRUE(cc.coolingPowerConsumptionCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(cql3, cc.coolingPowerConsumptionCurve());

    CurveQuadLinear cql4(m);
    EXPECT_TRUE(ch.setHeatingCapacityCurve(cql4));
    ASSERT_TRUE(ch.heatingCapacityCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(cql4, ch.heatingCapacityCurve());

    CurveQuadLinear cql5(m);
    EXPECT_TRUE(ch.setHeatingPowerConsumptionCurve(cql5));
    ASSERT_TRUE(ch.heatingPowerConsumptionCurve().optionalCast<CurveQuadLinear>());
    EXPECT_EQ(cql5, ch.heatingPowerConsumptionCurve());

    // setter can now handle table lookup curves
    TableLookup tbl1(m);
    EXPECT_TRUE(cc.setTotalCoolingCapacityCurve(tbl1));
    ASSERT_TRUE(cc.totalCoolingCapacityCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tbl1, cc.totalCoolingCapacityCurve());

    TableLookup tbl2(m);
    EXPECT_TRUE(cc.setSensibleCoolingCapacityCurve(tbl2));
    ASSERT_TRUE(cc.sensibleCoolingCapacityCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tbl2, cc.sensibleCoolingCapacityCurve());

    TableLookup tbl3(m);
    EXPECT_TRUE(cc.setCoolingPowerConsumptionCurve(tbl3));
    ASSERT_TRUE(cc.coolingPowerConsumptionCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tbl3, cc.coolingPowerConsumptionCurve());

    TableLookup tbl4(m);
    EXPECT_TRUE(ch.setHeatingCapacityCurve(tbl4));
    ASSERT_TRUE(ch.heatingCapacityCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tbl4, ch.heatingCapacityCurve());

    TableLookup tbl5(m);
    EXPECT_TRUE(ch.setHeatingPowerConsumptionCurve(tbl5));
    ASSERT_TRUE(ch.heatingPowerConsumptionCurve().optionalCast<TableLookup>());
    EXPECT_EQ(tbl5, ch.heatingPowerConsumptionCurve());
  }
}
