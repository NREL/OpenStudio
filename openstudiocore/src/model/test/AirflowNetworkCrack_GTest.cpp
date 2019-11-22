/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../AirflowNetworkCrack.hpp"
#include "../AirflowNetworkCrack_Impl.hpp"
#include "../AirflowNetworkReferenceCrackConditions.hpp"
#include "../AirflowNetworkReferenceCrackConditions_Impl.hpp"

//#include "../../utilities/units/Quantity.hpp"
//#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirflowNetwork_Crack) {
  Model model;

  AirflowNetworkCrack crack0(model, 1.0, 0.5);
  EXPECT_EQ(1, crack0.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack0.airMassFlowExponent());
  boost::optional<AirflowNetworkReferenceCrackConditions> opt = crack0.referenceCrackConditions();
  EXPECT_FALSE(opt);

  AirflowNetworkReferenceCrackConditions cond(model);
  EXPECT_EQ(101325, cond.barometricPressure());
  EXPECT_EQ(0, cond.humidityRatio());
  EXPECT_EQ(20, cond.temperature());
  cond.setBarometricPressure(0);
  EXPECT_EQ(101325, cond.barometricPressure());

  crack0.setReferenceCrackConditions(cond);
  EXPECT_EQ(20, crack0.referenceCrackConditions().get().temperature());
  cond.setTemperature(25);
  EXPECT_EQ(25, crack0.referenceCrackConditions().get().temperature());

  AirflowNetworkCrack crack1(model, 2.0, 0.5, cond);
  EXPECT_EQ(2, crack1.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack1.airMassFlowExponent());
  opt = crack1.referenceCrackConditions();
  EXPECT_TRUE(opt);
  EXPECT_EQ(25, opt.get().temperature());
}

