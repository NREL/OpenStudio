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
/*
#include "ModelFixture.hpp"
#include "../Model.hpp"

#include "../../energyplus/ReverseTranslator.hpp"
#include "../CmpntCostAdjustments.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/data/TimeSeries.hpp"

#include <utilities/idd/OS_ComponentCost_Adjustments_FieldEnums.hxx>
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
