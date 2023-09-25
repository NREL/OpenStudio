/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkOccupantVentilationControl.hpp"
#include "../AirflowNetworkOccupantVentilationControl_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_OccupantVentilationControl) {
  Model model;

  CurveLinear curve(model);
  curve.setCoefficient1Constant(55.0);
  curve.setCoefficient2x(0.0);

  AirflowNetworkOccupantVentilationControl ovc(model, curve);
  EXPECT_EQ(curve, ovc.thermalComfortLowTemperatureCurve());
  EXPECT_FALSE(ovc.thermalComfortHighTemperatureCurve());
}
