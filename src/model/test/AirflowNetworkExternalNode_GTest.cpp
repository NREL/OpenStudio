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

#include "../AirflowNetworkExternalNode.hpp"
#include "../AirflowNetworkExternalNode_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_ExternalNode) {
  Model model;

  AirflowNetworkExternalNode extnode0(model);
  Curve curve0 = extnode0.windPressureCoefficientCurve();
  CurveLinear* cpt = static_cast<CurveLinear*>(&curve0);
  //CurveLinear *cpt = dynamic_cast<CurveLinear*>(&curve0);
  //ASSERT_NE(nullptr, cpt);
  EXPECT_EQ(1.0, cpt->coefficient1Constant());
  EXPECT_EQ(0.0, cpt->coefficient2x());

  EXPECT_TRUE(extnode0.isExternalNodeHeightDefaulted());
  EXPECT_TRUE(extnode0.isSymmetricWindPressureCoefficientCurveDefaulted());
  EXPECT_EQ("Absolute", extnode0.windAngleType());
  EXPECT_TRUE(extnode0.isWindAngleTypeDefaulted());
  EXPECT_TRUE(extnode0.setWindAngleType("Relative"));
  EXPECT_EQ("Relative", extnode0.windAngleType());
  EXPECT_FALSE(extnode0.isWindAngleTypeDefaulted());

  CurveLinear curve1(model);
  curve1.setCoefficient1Constant(0.5);
  curve1.setCoefficient2x(0.0);
  curve1.setMinimumValueofx(0.0);
  curve1.setMaximumValueofx(360.0);

  extnode0.setWindPressureCoefficientCurve(curve1);
  EXPECT_EQ(curve1, extnode0.windPressureCoefficientCurve());

  AirflowNetworkExternalNode extnode1(model, curve1);
  EXPECT_EQ(curve1, extnode1.windPressureCoefficientCurve());
}
