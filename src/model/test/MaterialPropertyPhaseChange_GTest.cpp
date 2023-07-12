/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../MaterialPropertyPhaseChange.hpp"
#include "../MaterialPropertyPhaseChange_Impl.hpp"

#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MaterialPropertyPhaseChange_MaterialPropertyPhaseChange) {
  // create a model to use
  Model model;

  // create a material object to use
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(1, model.modelObjects().size());

  // new material does not have material property phase change yet
  EXPECT_FALSE(material.materialPropertyPhaseChange());

  // create a material property phase change object to use
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  ASSERT_TRUE(optphaseChange);
  EXPECT_EQ(2, model.modelObjects().size());

  // now the material has material property phase change
  EXPECT_TRUE(material.materialPropertyPhaseChange());

  // check to make sure the temperature coefficient for thermal conductivity field is defaulted as expected
  auto phaseChange = optphaseChange.get();
  EXPECT_EQ(material.nameString(), phaseChange.materialName());
  EXPECT_EQ(material.handle(), phaseChange.material().handle());
  EXPECT_TRUE(phaseChange.isTemperatureCoefficientforThermalConductivityDefaulted());

  // check that creating the material property phase change when they already exists does nothing and returns nil
  boost::optional<MaterialPropertyPhaseChange> optphaseChange2 = material.createMaterialPropertyPhaseChange();
  ASSERT_FALSE(optphaseChange2);
  EXPECT_EQ(2, model.modelObjects().size());
}

// test setting and getting
TEST_F(ModelFixture, MaterialPropertyPhaseChange_SetGetFields) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  auto phaseChange = optphaseChange.get();

  // check that the properties were set properly
  EXPECT_EQ(0.0, phaseChange.temperatureCoefficientforThermalConductivity());
  EXPECT_EQ(4, phaseChange.temperatureEnthalpys().size());
  EXPECT_EQ(4, phaseChange.numberofTemperatureEnthalpys());

  // now override the defaults with explicit values
  phaseChange.setTemperatureCoefficientforThermalConductivity(1.0);

  EXPECT_EQ(1.0, phaseChange.temperatureCoefficientforThermalConductivity());
  EXPECT_FALSE(phaseChange.isTemperatureCoefficientforThermalConductivityDefaulted());

  // test setting back to defaults
  phaseChange.resetTemperatureCoefficientforThermalConductivity();
  EXPECT_TRUE(phaseChange.isTemperatureCoefficientforThermalConductivityDefaulted());
}

// check temperature-enthalpy
TEST_F(ModelFixture, MaterialPropertyPhaseChange_TemperatureEnthalpy) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  auto phaseChange = optphaseChange.get();

  phaseChange.removeAllTemperatureEnthalpys();

  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(10.0, 20.0));
  EXPECT_EQ(1, phaseChange.numberofTemperatureEnthalpys());
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(25.0, 30.0));
  EXPECT_EQ(2, phaseChange.numberofTemperatureEnthalpys());
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(1.0, 2.0));
  EXPECT_EQ(3, phaseChange.numberofTemperatureEnthalpys());

  phaseChange.removeTemperatureEnthalpy(1);
  EXPECT_EQ(2, phaseChange.numberofTemperatureEnthalpys());

  // check that remaining temperature enthalpys moved correctly
  std::vector<TemperatureEnthalpy> temperatureEnthalpys = phaseChange.temperatureEnthalpys();
  EXPECT_EQ(10.0, temperatureEnthalpys[0].temperature());
  EXPECT_EQ(20.0, temperatureEnthalpys[0].enthalpy());
  EXPECT_EQ(1.0, temperatureEnthalpys[1].temperature());
  EXPECT_EQ(2.0, temperatureEnthalpys[1].enthalpy());

  // more remove checking
  phaseChange.removeAllTemperatureEnthalpys();
  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());
  phaseChange.removeTemperatureEnthalpy(0);
  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());

  // check adding temperature enthalpy non-conveniently
  TemperatureEnthalpy temperatureEnthalpy(50.0, 100.0);
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(temperatureEnthalpy));
  EXPECT_EQ(1, phaseChange.numberofTemperatureEnthalpys());

  // check bulk-adding temperature enthalpys
  // check bulk-adding temperature enthalpys
  std::vector<TemperatureEnthalpy> temperatureEnthalpysToAdd{{3.0, 4.0}, {5.0, 6.0}};
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpys(temperatureEnthalpysToAdd));
  EXPECT_EQ(3, phaseChange.numberofTemperatureEnthalpys());
  EXPECT_EQ(3, phaseChange.temperatureEnthalpys().size());
}

TEST_F(ModelFixture, MaterialPropertyPhaseChange_AlternateCtor) {
  Model model;
  StandardOpaqueMaterial material(model);
  std::vector<TemperatureEnthalpy> temperatureEnthalpys;
  TemperatureEnthalpy temperatureEnthalpy1(-20.0, 0.1);
  temperatureEnthalpys.push_back(temperatureEnthalpy1);
  TemperatureEnthalpy temperatureEnthalpy2(22.0, 18260.0);
  temperatureEnthalpys.push_back(temperatureEnthalpy2);
  TemperatureEnthalpy temperatureEnthalpy3(22.1, 32000.0);
  temperatureEnthalpys.push_back(temperatureEnthalpy3);

  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange(temperatureEnthalpys);
  auto phaseChange = optphaseChange.get();
  EXPECT_EQ(3, phaseChange.temperatureEnthalpys().size());
}

TEST_F(ModelFixture, MaterialPropertyPhaseChange_CtorThrow) {
  Model model;
  StandardOpaqueMaterial material(model);

  EXPECT_NO_THROW(MaterialPropertyPhaseChange phaseChange(material));
  EXPECT_TRUE(material.materialPropertyPhaseChange());
  EXPECT_THROW(MaterialPropertyPhaseChange phaseChange(material), openstudio::Exception);
  EXPECT_TRUE(material.materialPropertyPhaseChange());
}
