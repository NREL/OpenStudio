/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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

TEST_F(ISOModelFixture, ForwardTranslator)
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
