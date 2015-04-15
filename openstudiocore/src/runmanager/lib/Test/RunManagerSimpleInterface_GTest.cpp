/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>

#include "../RunManager.hpp"
#include "../../model/Model.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"

TEST_F(RunManagerTestFixture, SimpleInterfaceWithErrors)
{
  auto results = openstudio::runmanager::RunManager::runSimulation(openstudio::model::exampleModel(), 
      openstudio::runmanager::SimulationOptions(true, true, false, true, 2, 7, openstudio::toPath(""), openstudio::toPath(""), ""));

  auto errors = results.getErrors();

  ASSERT_TRUE(errors);
  EXPECT_FALSE(errors->succeeded());

  auto energyPlusResults = results.getEnergyPlusFuelUses();
  auto isoModelResults = results.getISOFuelUses();

  EXPECT_TRUE(energyPlusResults.size() == 0);
  EXPECT_TRUE(isoModelResults.size() == 0);

}

TEST_F(RunManagerTestFixture, SimpleInterfaceWithLocation)
{
  auto model = openstudio::model::exampleModel();


  auto results = openstudio::runmanager::RunManager::runSimulation(model, 
      openstudio::runmanager::SimulationOptions(true, true, false, true, 2, 7, openstudio::toPath(""), openstudio::toPath(""), "Golden, CO, USA"));

  //std::cout << openstudio::toString(results.epwUsed()) << '\n';

  auto errors = results.getErrors();

  ASSERT_TRUE(errors);

  EXPECT_TRUE(errors->succeeded());

  auto energyPlusResults = results.getEnergyPlusFuelUses();
  auto isoModelResults = results.getISOFuelUses();

  EXPECT_TRUE(energyPlusResults.size() > 0);
  EXPECT_TRUE(isoModelResults.size() > 0);

  auto energyPlusUses = 0.0;
  for (const auto &use : energyPlusResults)
  {
    energyPlusUses += use.second;
  }
  EXPECT_TRUE(energyPlusUses > 0.0);

  auto isoModelUses = 0.0;
  for (const auto &use : isoModelResults)
  {
    isoModelUses += use.second;
  }
  EXPECT_TRUE(isoModelUses> 0.0);

}


