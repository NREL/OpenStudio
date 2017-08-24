/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include "../GeneratorFuelCellInverter.hpp"
#include "../GeneratorFuelCellInverter_Impl.hpp"

#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"


using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellInverter) {
  Model model;

  // check default Inverter
  GeneratorFuelCellInverter inverter(model);
  EXPECT_EQ("Constant",inverter.inverterEfficiencyCalculationMode());
  EXPECT_EQ(1.0,inverter.inverterEfficiency());
  boost::optional<CurveQuadratic> electricCurve = inverter.efficiencyFunctionofDCPowerCurve();
  ASSERT_TRUE(electricCurve);
  inverter.setInverterEfficiency(0.5);
  EXPECT_EQ(0.5, inverter.inverterEfficiency());
  inverter.resetInverterEfficiency();
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
}

TEST_F(ModelFixture, FuelCellInverter2) {
  Model model;

  // check default Inverter
  CurveQuadratic electricCurve(model);
  GeneratorFuelCellInverter inverter(model, electricCurve);
  EXPECT_EQ("Quadratic", inverter.inverterEfficiencyCalculationMode());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
  EXPECT_EQ(electricCurve, inverter.efficiencyFunctionofDCPowerCurve());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
  //check reset calc mode
  inverter.resetInverterEfficiencyCalculationMode();
  EXPECT_EQ("Constant", inverter.inverterEfficiencyCalculationMode());
  EXPECT_EQ(1.0, inverter.inverterEfficiency());
  
}
