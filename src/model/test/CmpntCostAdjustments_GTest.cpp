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
/*
#include "ModelFixture.hpp"
#include "../Model.hpp"

#include "../../energyplus/ReverseTranslator.hpp"
#include "../CmpntCostAdjustments.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/data/TimeSeries.hpp"

#include <utilities/idd/OS_ComponentCost_Adjustments_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Optional.hpp"
#include <boost/algorithm/string/case_conv.hpp>


using namespace zero::model;
using namespace zero;

void checkObject(ComponentCostAdjustments object){

  // object will have a total and costperarea
  OptionalDouble CostEstTotal = object.getCostEstTotalBldg();
  ASSERT_TRUE(*CostEstTotal);
  ASSERT_NE(0, *CostEstTotal)<< " - total is zero, it should not be";

  OptionalDouble iCostPer = object.getCostPerBldgAreaBldg();
  ASSERT_TRUE(*iCostPer);
  EXPECT_NE(0, *iCostPer)<< " - costperarea is zero.  total could be zero and it should not be - or some other problem";


}

TEST_F(ModelFixture, ComponentCostAdj)
{
  zero::path idfPath = resourcesPath() / toPath("energyplus/Daylighting_School/in.idf");
  IdfFile idfFile(idfPath);
  Workspace workspace(idfFile);
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optModel = reverseTranslator.convert();
  ASSERT_TRUE(optModel) << "OptModel not reverse translated";
  Model model = optModel.get();

  zero::path sqlPath = resourcesPath() / toPath("energyplus/Daylighting_School/eplusout.sql");
  SqlFile sqlFile(sqlPath);
  ASSERT_TRUE(sqlFile.connectionOpen()) << "sqlFile connection not opened :(";
  model.setSqlFile(sqlFile);

//  EXPECT_EQ(static_cast<size_t>(10), ComponentCostAdjustments::getComponentCostAdjustments_All().size()) << "size of ComponentCostAdjustments vector is !=10"; //<<ComponentCostAdjustments::getComponentCostAdjustments_All(model).size();
//not sure I need foreach.  I think it only does this once.
//  for (const ComponentCostAdjustments& adjItem : ComponentCostAdjustments::getComponentCostAdjustments_All()){
//    checkObject(adjItem);
//  }
}


*/
