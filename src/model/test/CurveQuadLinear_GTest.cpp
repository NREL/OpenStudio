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

#include "ModelFixture.hpp"

#include "../CurveQuadLinear.hpp"
#include "../CurveQuadLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveQuadLinear_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  CurveQuadLinear curveQuadLinear(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CurveQuadLinear curveQuadLinear = m.getUniqueModelObject<CurveQuadLinear>();

  curveQuadLinear.setName("My CurveQuadLinear");

  // Coefficient1 Constant: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient1Constant(3));
  EXPECT_EQ(3, curveQuadLinear.coefficient1Constant());

  // Coefficient2 w: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient2w(3));
  EXPECT_EQ(3, curveQuadLinear.coefficient2w());

  // Coefficient3 x: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient3x(3));
  EXPECT_EQ(3, curveQuadLinear.coefficient3x());

  // Coefficient4 y: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient4y(3));
  EXPECT_EQ(3, curveQuadLinear.coefficient4y());

  // Coefficient5 z: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient5z(3));
  EXPECT_EQ(3, curveQuadLinear.coefficient5z());

  // Minimum Value of w: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofw(3));
  EXPECT_EQ(3, curveQuadLinear.minimumValueofw());

  // Maximum Value of w: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofw(3));
  EXPECT_EQ(3, curveQuadLinear.maximumValueofw());

  // Minimum Value of x: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofx(3));
  EXPECT_EQ(3, curveQuadLinear.minimumValueofx());

  // Maximum Value of x: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofx(3));
  EXPECT_EQ(3, curveQuadLinear.maximumValueofx());

  // Minimum Value of y: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofy(3));
  EXPECT_EQ(3, curveQuadLinear.minimumValueofy());

  // Maximum Value of y: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofy(3));
  EXPECT_EQ(3, curveQuadLinear.maximumValueofy());

  // Minimum Value of z: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofz(3));
  EXPECT_EQ(3, curveQuadLinear.minimumValueofz());

  // Maximum Value of z: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofz(3));
  EXPECT_EQ(3, curveQuadLinear.maximumValueofz());

  // Minimum Curve Output: Optional Double
  EXPECT_TRUE(curveQuadLinear.setMinimumCurveOutput(3));
  ASSERT_TRUE(curveQuadLinear.minimumCurveOutput());
  EXPECT_EQ(3, curveQuadLinear.minimumCurveOutput().get());

  // Maximum Curve Output: Optional Double
  EXPECT_TRUE(curveQuadLinear.setMaximumCurveOutput(3));
  ASSERT_TRUE(curveQuadLinear.maximumCurveOutput());
  EXPECT_EQ(3, curveQuadLinear.maximumCurveOutput().get());

  // Input Unit Type for w: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypeforw("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforw());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypeforw("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforw());

  // Input Unit Type for x: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypeforx("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforx());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypeforx("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforx());

  // Input Unit Type for y: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypefory("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypefory());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypefory("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypefory());

  // Input Unit Type for z: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypeforz("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforz());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypeforz("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforz());
