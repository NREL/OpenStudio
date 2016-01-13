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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/TableMultiVariableLookup.hpp"
#include "../../model/TableMultiVariableLookup_Impl.hpp"

#include <utilities/idd/Table_MultiVariableLookup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_TableMultiVariableLookup)
{
  {
    Model m;
    TableMultiVariableLookup table(m,1);

    ASSERT_TRUE(table.addPoint(70,0.1));
    ASSERT_TRUE(table.addPoint(72,0.3));
    ASSERT_TRUE(table.addPoint(74,0.5));
    ASSERT_TRUE(table.addPoint(76,0.7));
    ASSERT_TRUE(table.addPoint(78,0.9));

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> tableObjects = workspace.getObjectsByType(IddObjectType::Table_MultiVariableLookup);
    ASSERT_EQ(1u, tableObjects.size());

    WorkspaceObject idfTable = tableObjects.front();

    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InterpolationMethod).get(),"LagrangeInterpolationLinearExtrapolation");
    ASSERT_EQ(idfTable.getInt(Table_MultiVariableLookupFields::NumberofInterpolationPoints).get(),3);
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::TableDataFormat).get(),"SingleLineIndependentVariableWithMatrix");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::X1SortOrder).get(),"Ascending");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::X2SortOrder).get(),"Ascending");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX1).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX2).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX3).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX4).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX5).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::OutputUnitType).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getInt(Table_MultiVariableLookupFields::NumberofIndependentVariables).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(0).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(1).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),70);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(2).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),72);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(3).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),74);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(4).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),76);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(5).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),78);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(6).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(7).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(8).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(9).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(10).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
  }

  {
    Model m;
    TableMultiVariableLookup table(m,2);

    ASSERT_TRUE(table.addPoint(70,32,0.1));
    ASSERT_TRUE(table.addPoint(72,32,0.3));
    ASSERT_TRUE(table.addPoint(74,32,0.5));
    ASSERT_TRUE(table.addPoint(76,32,0.7));
    ASSERT_TRUE(table.addPoint(78,32,0.9));

    ASSERT_TRUE(table.addPoint(70,45,0.2));
    ASSERT_TRUE(table.addPoint(72,45,0.4));
    ASSERT_TRUE(table.addPoint(74,45,0.6));
    ASSERT_TRUE(table.addPoint(76,45,0.8));
    ASSERT_TRUE(table.addPoint(78,45,1.0));

    ASSERT_TRUE(table.addPoint(70,68,0.3));
    ASSERT_TRUE(table.addPoint(72,68,0.5));
    ASSERT_TRUE(table.addPoint(74,68,0.7));
    ASSERT_TRUE(table.addPoint(76,68,0.9));
    ASSERT_TRUE(table.addPoint(78,68,1.1));

    ASSERT_TRUE(table.addPoint(70,81,0.4));
    ASSERT_TRUE(table.addPoint(72,81,0.6));
    ASSERT_TRUE(table.addPoint(74,81,0.8));
    ASSERT_TRUE(table.addPoint(76,81,1.0));
    ASSERT_TRUE(table.addPoint(78,81,1.2));

    ASSERT_TRUE(table.addPoint(70,94,0.5));
    ASSERT_TRUE(table.addPoint(72,94,0.7));
    ASSERT_TRUE(table.addPoint(74,94,0.9));
    ASSERT_TRUE(table.addPoint(76,94,1.1));
    ASSERT_TRUE(table.addPoint(78,94,1.3));

    ASSERT_TRUE(table.addPoint(70,107,0.6));
    ASSERT_TRUE(table.addPoint(72,107,0.8));
    ASSERT_TRUE(table.addPoint(74,107,1.0));
    ASSERT_TRUE(table.addPoint(76,107,1.2));
    ASSERT_TRUE(table.addPoint(78,107,1.4));

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> tableObjects = workspace.getObjectsByType(IddObjectType::Table_MultiVariableLookup);
    ASSERT_EQ(1u, tableObjects.size());

    WorkspaceObject idfTable = tableObjects.front();
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InterpolationMethod).get(),"LagrangeInterpolationLinearExtrapolation");
    ASSERT_EQ(idfTable.getInt(Table_MultiVariableLookupFields::NumberofInterpolationPoints).get(),3);
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::TableDataFormat).get(),"SingleLineIndependentVariableWithMatrix");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::X1SortOrder).get(),"Ascending");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::X2SortOrder).get(),"Ascending");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX1).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX2).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX3).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX4).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX5).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::OutputUnitType).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getInt(Table_MultiVariableLookupFields::NumberofIndependentVariables).get(),2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(0).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(1).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(2).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),70);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(3).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),72);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(4).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),74);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(5).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),76);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(6).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),78);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(7).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),32);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(8).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),45);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(9).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),68);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(10).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),81);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(11).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),94);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(12).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),107);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(13).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(14).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(15).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(16).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(17).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(18).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(19).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(20).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(21).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(22).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(23).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(24).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(25).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(26).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(27).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(28).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(29).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(30).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(31).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(32).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(33).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(34).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(35).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(36).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(37).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(38).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(39).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(40).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(41).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(42).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.4);
  }

  {
    Model m;
    TableMultiVariableLookup table(m,3);

    ASSERT_TRUE(table.addPoint(70,32,1,0.1));
    ASSERT_TRUE(table.addPoint(72,32,1,0.3));
    ASSERT_TRUE(table.addPoint(74,32,1,0.5));
    ASSERT_TRUE(table.addPoint(76,32,1,0.7));
    ASSERT_TRUE(table.addPoint(78,32,1,0.9));

    ASSERT_TRUE(table.addPoint(70,45,1,0.2));
    ASSERT_TRUE(table.addPoint(72,45,1,0.4));
    ASSERT_TRUE(table.addPoint(74,45,1,0.6));
    ASSERT_TRUE(table.addPoint(76,45,1,0.8));
    ASSERT_TRUE(table.addPoint(78,45,1,1.0));

    ASSERT_TRUE(table.addPoint(70,68,1,0.3));
    ASSERT_TRUE(table.addPoint(72,68,1,0.5));
    ASSERT_TRUE(table.addPoint(74,68,1,0.7));
    ASSERT_TRUE(table.addPoint(76,68,1,0.9));
    ASSERT_TRUE(table.addPoint(78,68,1,1.1));

    ASSERT_TRUE(table.addPoint(70,81,1,0.4));
    ASSERT_TRUE(table.addPoint(72,81,1,0.6));
    ASSERT_TRUE(table.addPoint(74,81,1,0.8));
    ASSERT_TRUE(table.addPoint(76,81,1,1.0));
    ASSERT_TRUE(table.addPoint(78,81,1,1.2));

    ASSERT_TRUE(table.addPoint(70,94,1,0.5));
    ASSERT_TRUE(table.addPoint(72,94,1,0.7));
    ASSERT_TRUE(table.addPoint(74,94,1,0.9));
    ASSERT_TRUE(table.addPoint(76,94,1,1.1));
    ASSERT_TRUE(table.addPoint(78,94,1,1.3));

    ASSERT_TRUE(table.addPoint(70,107,1,0.6));
    ASSERT_TRUE(table.addPoint(72,107,1,0.8));
    ASSERT_TRUE(table.addPoint(74,107,1,1.0));
    ASSERT_TRUE(table.addPoint(76,107,1,1.2));
    ASSERT_TRUE(table.addPoint(78,107,1,1.4));

    ASSERT_TRUE(table.addPoint(70,32,2,10.1));
    ASSERT_TRUE(table.addPoint(72,32,2,10.3));
    ASSERT_TRUE(table.addPoint(74,32,2,10.5));
    ASSERT_TRUE(table.addPoint(76,32,2,10.7));
    ASSERT_TRUE(table.addPoint(78,32,2,10.9));

    ASSERT_TRUE(table.addPoint(70,45,2,10.2));
    ASSERT_TRUE(table.addPoint(72,45,2,10.4));
    ASSERT_TRUE(table.addPoint(74,45,2,10.6));
    ASSERT_TRUE(table.addPoint(76,45,2,10.8));
    ASSERT_TRUE(table.addPoint(78,45,2,11.0));

    ASSERT_TRUE(table.addPoint(70,68,2,10.3));
    ASSERT_TRUE(table.addPoint(72,68,2,10.5));
    ASSERT_TRUE(table.addPoint(74,68,2,10.7));
    ASSERT_TRUE(table.addPoint(76,68,2,10.9));
    ASSERT_TRUE(table.addPoint(78,68,2,11.1));

    ASSERT_TRUE(table.addPoint(70,81,2,10.4));
    ASSERT_TRUE(table.addPoint(72,81,2,10.6));
    ASSERT_TRUE(table.addPoint(74,81,2,10.8));
    ASSERT_TRUE(table.addPoint(76,81,2,11.0));
    ASSERT_TRUE(table.addPoint(78,81,2,11.2));

    ASSERT_TRUE(table.addPoint(70,94,2,10.5));
    ASSERT_TRUE(table.addPoint(72,94,2,10.7));
    ASSERT_TRUE(table.addPoint(74,94,2,10.9));
    ASSERT_TRUE(table.addPoint(76,94,2,11.1));
    ASSERT_TRUE(table.addPoint(78,94,2,11.3));

    ASSERT_TRUE(table.addPoint(70,107,2,10.6));
    ASSERT_TRUE(table.addPoint(72,107,2,10.8));
    ASSERT_TRUE(table.addPoint(74,107,2,11.0));
    ASSERT_TRUE(table.addPoint(76,107,2,11.2));
    ASSERT_TRUE(table.addPoint(78,107,2,11.4));

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> tableObjects = workspace.getObjectsByType(IddObjectType::Table_MultiVariableLookup);
    ASSERT_EQ(1u, tableObjects.size());

    WorkspaceObject idfTable = tableObjects.front();
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InterpolationMethod).get(),"LagrangeInterpolationLinearExtrapolation");
    ASSERT_EQ(idfTable.getInt(Table_MultiVariableLookupFields::NumberofInterpolationPoints).get(),3);
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::TableDataFormat).get(),"SingleLineIndependentVariableWithMatrix");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::X1SortOrder).get(),"Ascending");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::X2SortOrder).get(),"Ascending");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX1).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX2).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX3).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX4).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::InputUnitTypeforX5).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getString(Table_MultiVariableLookupFields::OutputUnitType).get(),"Dimensionless");
    ASSERT_EQ(idfTable.getInt(Table_MultiVariableLookupFields::NumberofIndependentVariables).get(),3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(0).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(1).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(2).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(3).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),70);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(4).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),72);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(5).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),74);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(6).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),76);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(7).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),78);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(8).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),32);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(9).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),45);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(10).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),68);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(11).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),81);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(12).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),94);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(13).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),107);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(14).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(15).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(16).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(17).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(18).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(19).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(20).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(21).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(22).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(23).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(24).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(25).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(26).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(27).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(28).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(29).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(30).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(31).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(32).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(33).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(34).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(35).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(36).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(37).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(38).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(39).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(40).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(41).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(42).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(43).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),0.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(44).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(45).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(46).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),1.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(47).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(48).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(49).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(50).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(51).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(52).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(53).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(54).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(55).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(56).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(57).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(58).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(59).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(60).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(61).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(62).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(63).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.4);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(64).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(65).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(66).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(67).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(68).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(69).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(70).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.9);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(71).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(72).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(73).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.6);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(74).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),10.8);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(75).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(76).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11.2);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(77).getDouble(Table_MultiVariableLookupExtensibleFields::Data).get(),11.4);
  }
}

