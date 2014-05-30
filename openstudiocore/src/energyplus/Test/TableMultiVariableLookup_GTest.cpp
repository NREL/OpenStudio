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
#include <energyplus/Test/EnergyPlusFixture.hpp>

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/TableMultiVariableLookup.hpp>
#include <model/TableMultiVariableLookup_Impl.hpp>

#include <utilities/idd/Table_MultiVariableLookup_FieldEnums.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_TableMultiVariableLookup)
{
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

    std::cout << workspace << std::endl;
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

    std::cout << workspace << std::endl;
  }
}

