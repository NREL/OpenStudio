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
