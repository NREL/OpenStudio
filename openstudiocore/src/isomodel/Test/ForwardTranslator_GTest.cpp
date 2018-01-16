/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "ISOModelFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../UserModel.hpp"
#include "../SimModel.hpp"

#include "../../model/Model.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::model;
using namespace openstudio::isomodel;
using namespace openstudio;

TEST_F(ISOModelFixture, DISABLED_ForwardTranslator)
{
  Model model = exampleModel();

  openstudio::path weather = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");
  ForwardTranslator forwardTranslator;
  UserModel userModel = forwardTranslator.translateModel(model);
  userModel.setWeatherFilePath(weather);
  SimModel simModel = userModel.toSimModel();
  ISOResults results = simModel.simulate();

  std::vector<EndUseFuelType> fuelTypes = EndUses::fuelTypes();

  double gas = 0;
  double elec = 0;

  for (std::vector<EndUseFuelType>::const_iterator itr = fuelTypes.begin();
      itr != fuelTypes.end();
      ++itr)
  {
    double value = 0;
    for (std::vector<EndUses>::const_iterator itr2 = results.monthlyResults.begin();
        itr2 != results.monthlyResults.end();
        ++itr2)
    {
      value += itr2->getEndUseByFuelType(*itr);
    }

    LOG(Debug, "Read fuel use of " << value << " kWh/m2 For " << itr->valueName() << " which is " << value * 3600000 * userModel.floorArea() << " Joules");
    if (itr->valueName() == "Gas")
    {
      gas = value * 3600000 * userModel.floorArea();
    } else if (itr->valueName() == "Electricity") {
      elec = value * 3600000 * userModel.floorArea();
    }
  }

  EXPECT_GT(elec, 0);
  EXPECT_GT(gas, 0);
  EXPECT_NEAR(118.32e9, elec, 10e9); // J, running exampleModel from OS 1.3.1 in E+ 8.0
  EXPECT_NEAR(46.50e9, gas, 10e9); // J, running exampleModel from OS 1.3.1 in E+ 8.0
}
