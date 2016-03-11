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

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using openstudio::model::Model;
using openstudio::model::Building;
using openstudio::model::Site;
using openstudio::model::SimulationControl;
using openstudio::energyplus::ForwardTranslator;
using openstudio::energyplus::ReverseTranslator;

TEST_F(EnergyPlusFixture,ForwardTranslator_Building)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  building.setName("Building");
  EXPECT_TRUE(building.isNorthAxisDefaulted());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Building).size());
  ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::Site_Location).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::SimulationControl).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::Building)[0];
  ASSERT_TRUE(object.getString(BuildingFields::Name));
  EXPECT_EQ("Building", object.getString(BuildingFields::Name).get());
  EXPECT_TRUE(object.isEmpty(BuildingFields::NorthAxis));
  EXPECT_TRUE(object.isEmpty(BuildingFields::Terrain));
  EXPECT_TRUE(object.isEmpty(BuildingFields::LoadsConvergenceToleranceValue));
  EXPECT_TRUE(object.isEmpty(BuildingFields::TemperatureConvergenceToleranceValue));
  EXPECT_TRUE(object.isEmpty(BuildingFields::SolarDistribution));
  EXPECT_TRUE(object.isEmpty(BuildingFields::MaximumNumberofWarmupDays));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Building2)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  building.setName("Building");
  Site site = model.getUniqueModelObject<Site>();
  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Building).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Site_Location).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::SimulationControl).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::Building)[0];
  ASSERT_TRUE(object.getString(BuildingFields::Name));
  EXPECT_EQ("Building", object.getString(BuildingFields::Name).get());
  EXPECT_TRUE(object.isEmpty(BuildingFields::NorthAxis));
  EXPECT_TRUE(object.isEmpty(BuildingFields::Terrain));
  EXPECT_TRUE(object.isEmpty(BuildingFields::LoadsConvergenceToleranceValue));
  EXPECT_TRUE(object.isEmpty(BuildingFields::TemperatureConvergenceToleranceValue));
  EXPECT_TRUE(object.isEmpty(BuildingFields::SolarDistribution));
  EXPECT_TRUE(object.isEmpty(BuildingFields::MaximumNumberofWarmupDays));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Building3)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  building.setName("Building");
  building.setNorthAxis(20);
  building.setNominalFloortoFloorHeight(5);

  Site site = model.getUniqueModelObject<Site>();
  site.setTerrain("Ocean");

  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();
  simulationControl.setLoadsConvergenceToleranceValue(0.2);
  simulationControl.setTemperatureConvergenceToleranceValue(0.2);
  simulationControl.setSolarDistribution("FullInteriorAndExteriorWithReflections");
  simulationControl.setMaximumNumberofWarmupDays(2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Building).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Site_Location).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::SimulationControl).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::Building)[0];
  ASSERT_TRUE(object.getString(BuildingFields::Name));
  EXPECT_EQ("Building", object.getString(BuildingFields::Name).get());
  ASSERT_TRUE(object.getDouble(BuildingFields::NorthAxis));
  EXPECT_EQ(20, object.getDouble(BuildingFields::NorthAxis).get());
  ASSERT_TRUE(object.getString(BuildingFields::Terrain));
  EXPECT_EQ("Ocean", object.getString(BuildingFields::Terrain).get());
  ASSERT_TRUE(object.getDouble(BuildingFields::LoadsConvergenceToleranceValue));
  EXPECT_EQ(0.2, object.getDouble(BuildingFields::LoadsConvergenceToleranceValue).get());
  ASSERT_TRUE(object.getDouble(BuildingFields::TemperatureConvergenceToleranceValue));
  EXPECT_EQ(0.2, object.getDouble(BuildingFields::TemperatureConvergenceToleranceValue).get());
  ASSERT_TRUE(object.getString(BuildingFields::SolarDistribution));
  EXPECT_EQ("FullInteriorAndExteriorWithReflections", object.getString(BuildingFields::SolarDistribution).get());
  ASSERT_TRUE(object.getInt(BuildingFields::MaximumNumberofWarmupDays));
  EXPECT_EQ(2, object.getInt(BuildingFields::MaximumNumberofWarmupDays).get());
}


