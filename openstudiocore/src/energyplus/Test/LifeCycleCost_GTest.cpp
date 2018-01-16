/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/LifeCycleCost_Impl.hpp"
#include "../../model/LifeCycleCostParameters.hpp"
#include "../../model/LifeCycleCostParameters_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/ComponentCost_LineItem_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_RecurringCosts_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"

#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include "../../model/ZoneHVACUnitHeater.hpp"
#include "../../model/ZoneHVACUnitHeater_Impl.hpp"

#include <sstream>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::energyplus;

TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_Construction)
{
  Model model;

  Construction construction(model);

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(2, 2, 0));
  Surface surface(points, model);
  surface.setSpace(space);
  surface.setConstruction(construction);

  EXPECT_DOUBLE_EQ(4.0, construction.getNetArea());

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Construction", construction, 10.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Maintenance", construction, 1.0, "CostPerArea", "Maintenance", 1, 1);
  ASSERT_TRUE(cost2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(40.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(4.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_Construction2)
{
  Model model;

  Construction construction(model);

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(2, 2, 0));
  Surface surface(points, model);
  surface.setSpace(space);
  surface.setConstruction(construction);

  EXPECT_DOUBLE_EQ(4.0, construction.getNetArea());

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Material", construction, 10.0, "CostPerArea", "Construction", 3, 5);
  ASSERT_TRUE(cost);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Material", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(40.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_EQ(5, idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Material Replacement", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Replacement", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(40.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_EQ(8, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(3, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_Construction3)
{
  Model model;

  Construction construction(model);

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(2, 2, 0));
  Surface surface(points, model);
  surface.setSpace(space);
  surface.setConstruction(construction);

  EXPECT_DOUBLE_EQ(4.0, construction.getNetArea());

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Demolition", construction, 10.0, "CostPerArea", "Salvage", 20, 20);
  ASSERT_TRUE(cost);

  LifeCycleCostParameters lcc = model.getUniqueModelObject<LifeCycleCostParameters>();
  lcc.setLengthOfStudyPeriodInYears(25);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Demolition", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Salvage", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(40.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_EQ(20, idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  EXPECT_EQ(0u, idfObjects.size());
}


TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_Lights)
{
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  LightsDefinition lightsDefinition(model);
  Lights lights(lightsDefinition);
  lights.setSpace(space);

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(2, 2, 0));
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_DOUBLE_EQ(4.0, space.floorArea());

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Installation", lightsDefinition, 10.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Maintenance", lightsDefinition, 1.0, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Installation", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(40.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(4.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_Lights_Multiplier)
{
  Model model;

  ThermalZone thermalZone(model);
  thermalZone.setMultiplier(2.0);

  Space space(model);
  space.setThermalZone(thermalZone);

  LightsDefinition lightsDefinition(model);
  Lights lights(lightsDefinition);
  lights.setSpace(space);
  lights.setMultiplier(3.0);

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(2, 2, 0));
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_DOUBLE_EQ(4.0, space.floorArea());

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Installation", lightsDefinition, 10.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Maintenance", lightsDefinition, 1.0, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Installation", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(6*40.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(6*4.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}


TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_Building)
{
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(2, 2, 0));
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_DOUBLE_EQ(4.0, space.floorArea());

  Building building = model.getUniqueModelObject<Building>();

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Installation", building, 10.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Maintenance", building, 1.0, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Installation", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(40.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(4.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_HVACComponent)
{

  //create a model to use in testing this code.
  model::Model model;

  //create a schedule and the curves to use in the constructor
  ScheduleConstant schedule(model);
  schedule.setValue(1.0); // Always on
  model::CurveBiquadratic ccfot1(model);
  model::CurveCubic ccfof2(model);
  model::CurveBiquadratic eirfot3(model);
  model::CurveQuadratic eirfof4(model);
  model::CurveCubic plf5(model);
  model::CurveBiquadratic lsccfot6(model);
  model::CurveBiquadratic lseirfot7(model);

  //make a coil to do the testing on
  model::CoilCoolingDXTwoSpeed coil(model,
                                    schedule,
                                    ccfot1,
                                    ccfof2,
                                    eirfot3,
                                    eirfof4,
                                    plf5,
                                    lsccfot6,
                                    lseirfot7);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Installation", coil, 10.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Maintenance", coil, 1.0, "CostPerEach", "Maintenance", 1);
  ASSERT_TRUE(cost2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Installation", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(10.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(1.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}

TEST_F(EnergyPlusFixture,ForwardTranslator_LifeCycleCost_ZoneHVACComponent)
{

  model::Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  FanConstantVolume fan(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model,sched,fan,heatingCoil);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Installation", zoneHVACUnitHeater, 10.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Maintenance", zoneHVACUnitHeater, 1.0, "CostPerEach", "Maintenance", 1);
  ASSERT_TRUE(cost2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Installation", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(10.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(1.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}
