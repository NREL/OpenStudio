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

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"

#include "../../model/ThermalZone.hpp"

#include "../../model/ElectricLoadCenterTransformer.hpp"
#include "../../model/ElectricLoadCenterTransformer_Impl.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_ElectricLoadCenter_Transformer_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Transformer_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <QThread>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;


TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  ElectricLoadCenterTransformer elct(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Schedule_Compact).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];
  WorkspaceObject schedule = workspace.getObjectsByType(IddObjectType::Schedule_Compact)[0];

  EXPECT_EQ(schedule.nameString(), object.getString(ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::ZoneName, false).get());
  EXPECT_EQ("0", object.getString(ElectricLoadCenter_TransformerFields::RadiativeFraction, false).get());
  EXPECT_EQ("15000", object.getString(ElectricLoadCenter_TransformerFields::RatedCapacity, false).get());
  EXPECT_EQ("3", object.getString(ElectricLoadCenter_TransformerFields::Phase, false).get());
  EXPECT_EQ("Aluminum", object.getString(ElectricLoadCenter_TransformerFields::ConductorMaterial, false).get());
  EXPECT_EQ("150", object.getString(ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, false).get());
  EXPECT_EQ("0.1", object.getString(ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, false).get());
  EXPECT_EQ("NominalEfficiency", object.getString(ElectricLoadCenter_TransformerFields::PerformanceInputMethod, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::RatedLoadLoss, false).get());
  EXPECT_EQ("0.98", object.getString(ElectricLoadCenter_TransformerFields::NameplateEfficiency, false).get());
  EXPECT_EQ("0.35", object.getString(ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, false).get());
  EXPECT_EQ("75", object.getString(ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, false).get());
  EXPECT_EQ("Yes", object.getString(ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, false).get());
  std::vector<IdfExtensibleGroup> extensibleGroups = object.extensibleGroups();
  EXPECT_EQ(0u, extensibleGroups.size());

  model.save(toPath("./ElectricLoadCenterTransformer.osm"), true);
  workspace.save(toPath("./ElectricLoadCenterTransformer.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer2) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  //add schedule
  Schedule s = model.alwaysOffDiscreteSchedule();
  ElectricLoadCenterTransformer elct(model);

  elct.setAvailabilitySchedule(s);
  elct.setZone(zone1);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];

  EXPECT_EQ(s.nameString(), object.getString(ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ(zone1.nameString(), object.getString(ElectricLoadCenter_TransformerFields::ZoneName, false).get());

  model.save(toPath("./ElectricLoadCenterTransformer2.osm"), true);
  workspace.save(toPath("./ElectricLoadCenterTransformer2.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer3) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  // add Meter
  OutputMeter meter(model);
  meter.setName("Electricity:Building");
  OutputMeter meter2(model);
  meter2.setName("Electricity:HVAC");
  OutputMeter meter3(model);
  meter3.setName("Electricity:Fans");

  ElectricLoadCenterTransformer elct(model);

  elct.addMeter(meter.nameString());
  elct.addMeter(meter2.nameString());
  elct.addMeter(meter3.nameString());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];

  std::vector<IdfExtensibleGroup> extensibleGroups = object.extensibleGroups();
  EXPECT_EQ(3u, extensibleGroups.size());

  model.save(toPath("./ElectricLoadCenterTransformer2.osm"), true);
  workspace.save(toPath("./ElectricLoadCenterTransformer2.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer_Distribution) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  ElectricLoadCenterTransformer elct(model);
  ElectricLoadCenterDistribution elcd(model);

  elcd.setTransformer(elct);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  model.save(toPath("./ElectricLoadCenterDistribution.osm"), true);
  workspace.save(toPath("./ElectricLoadCenterDistribution.idf"), true);

  WorkspaceObject transformer = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];
  WorkspaceObject distribution = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution)[0];

  EXPECT_EQ(transformer.nameString(), distribution.getString(ElectricLoadCenter_DistributionFields::TransformerObjectName, false).get());

  model.save(toPath("./ElectricLoadCenterDistribution.osm"), true);
  workspace.save(toPath("./ElectricLoadCenterDistribution.idf"), true);
}
