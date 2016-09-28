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
