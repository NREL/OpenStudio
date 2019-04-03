/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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


