/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Curve.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveBiquadratic.hpp"
#include "../EnergyManagementSystemCurveOrTableIndexVariable.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EMS_CurveOrTableIndexVariable) {
  Model model;

  // Create a curve
  CurveBiquadratic c1(model);

  EnergyManagementSystemCurveOrTableIndexVariable emsCurve(model, c1);
  //emsCurve.setCurveorTableObject(c1);
  EXPECT_EQ(c1.handle(), emsCurve.curveOrTableObject().handle());
  // model.save(toPath("./EMS_curvetest.osm"), true);
}
