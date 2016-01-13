/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include <resources.hxx>

#include "../JobFactory.hpp"
#include "../ModelToRadJob.hpp"
#include "../RunManager.hpp"
#include "../Workflow.hpp"

#include "../../../model/Model.hpp"

#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/IdfObject.hpp"
#include "../../../utilities/data/EndUses.hpp"
#include "../../../utilities/data/Attribute.hpp"
#include "../../../utilities/sql/SqlFile.hpp"

#include <boost/filesystem/path.hpp>


TEST_F(RunManagerTestFixture, JobErrors_RepeatingWarnings)
{
  openstudio::runmanager::JobErrors err;
  err.addError(openstudio::runmanager::ErrorType::Warning, "CalcDoe2DXCoil: Coil:Cooling:DX:SingleSpeed=\"COIL COOLING DX SINGLE SPEED 12\" - Low condenser dry-bulb temperature error continues...\n This error occurred 3 total times;\n during Warmup 0 times;\n during Sizing 0 times.\n Max=-0.966667 [C]  Min=-3.8 [C]");

  err.addError(openstudio::runmanager::ErrorType::Warning, "CalcDoe2DXCoil: Coil:Cooling:DX:SingleSpeed=\"COIL COOLING DX SINGLE SPEED 12\" - Full load outlet temperature indicates a possibility of frost/freeze error continues. Outlet air temperature statistics follow:\n This error occurred 958 total times;\n during Warmup 0 times;\n during Sizing 0 times.\n Max=1.997448  Min=-8.176312");

  err.addError(openstudio::runmanager::ErrorType::Warning, "CalcDoe2DXCoil: Coil:Cooling:DX:SingleSpeed=\"COIL COOLING DX SINGLE SPEED 6\" - Low condenser dry-bulb temperature error continues...\n This error occurred 4 total times;\n during Warmup 0 times;\n during Sizing 0 times.\n Max=-0.966667 [C]  Min=-3.8 [C]");


  EXPECT_EQ(3u, err.errorsByType(openstudio::runmanager::ErrorType::Warning).size());
  EXPECT_EQ(962, err.totalCountByType(openstudio::runmanager::ErrorType::Warning));

  std::vector<std::pair<int, std::string> > errs = err.errorsByTypeWithCount(openstudio::runmanager::ErrorType::Warning);

  ASSERT_EQ(3u, errs.size());
  EXPECT_EQ(2, errs[0].first);
  EXPECT_EQ(957, errs[1].first);
  EXPECT_EQ(3, errs[2].first);

}
