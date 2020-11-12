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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SurfaceProperty_ConvectionCoefficients_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyConvectionCoefficients_InternalMass) {
  // openstudio::Logger::instance().standardOutLogger().enable();
  // openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  Model model;
  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);
  InternalMassDefinition intMassDefn(model);
  intMassDefn.setSurfaceArea(20);
  InternalMass intMass(intMassDefn);
  intMass.setSpace(space);
  openstudio::model::SurfacePropertyConvectionCoefficients cc(intMass);
  EXPECT_EQ(intMass.handle(), cc.surfaceAsModelObject().handle());

  cc.setConvectionCoefficient1Location("Inside");
  cc.setConvectionCoefficient1Type("Value");
  cc.setConvectionCoefficient1(5.0);
  cc.setConvectionCoefficient2Location("Outside");
  cc.setConvectionCoefficient2Type("Schedule");
  ScheduleConstant sched(model);
  sched.setName("Constant Schedule");
  sched.setValue(1.0);
  cc.setConvectionCoefficient2Schedule(sched);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::SurfaceProperty_ConvectionCoefficients));
  ASSERT_EQ(objVector.size(), 1);
  WorkspaceObject wo(objVector.at(0));

  boost::optional<std::string> cc1loc(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location));
  EXPECT_TRUE(cc1loc);
  if (cc1loc) {
    EXPECT_EQ(cc1loc.get(), "Inside");
  }
  boost::optional<std::string> cc1type(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type));
  EXPECT_TRUE(cc1type);
  if (cc1type) {
    EXPECT_EQ(cc1type.get(), "Value");
  }
  boost::optional<double> cc1value(wo.getDouble(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1));
  EXPECT_TRUE(cc1value);
  if (cc1value) {
    EXPECT_EQ(cc1value.get(), 5.0);
  }
  boost::optional<std::string> cc2loc(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location));
  EXPECT_TRUE(cc2loc);
  if (cc2loc) {
    EXPECT_EQ(cc2loc.get(), "Outside");
  }
  boost::optional<std::string> cc2type(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type));
  EXPECT_TRUE(cc2type);
  if (cc2type) {
    EXPECT_EQ(cc2type.get(), "Schedule");
  }
  boost::optional<WorkspaceObject> woSched(wo.getTarget(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName));
  EXPECT_TRUE(woSched);
  if (woSched) {
    EXPECT_EQ(woSched->iddObject().type(), IddObjectType::Schedule_Constant);
  }
  boost::optional<WorkspaceObject> woIntMass(wo.getTarget(SurfaceProperty_ConvectionCoefficientsFields::SurfaceName));
  EXPECT_TRUE(woIntMass);
  if (woIntMass) {
    EXPECT_EQ(woIntMass->iddObject().type(), IddObjectType::InternalMass);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyConvectionCoefficients_Surface) {
  // openstudio::Logger::instance().standardOutLogger().enable();
  // openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  Model model;
  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));

  Surface surface(vertices, model);
  surface.setSurfaceType("Floor");
  surface.setSpace(space);
  surface.setOutsideBoundaryCondition("Outdoors");

  openstudio::model::SurfacePropertyConvectionCoefficients cc(surface);
  EXPECT_EQ(surface.handle(), cc.surfaceAsModelObject().handle());

  cc.setConvectionCoefficient1Location("Inside");
  cc.setConvectionCoefficient1Type("Value");
  cc.setConvectionCoefficient1(3.0);
  cc.setConvectionCoefficient2Location("Outside");
  cc.setConvectionCoefficient2Type("Schedule");
  ScheduleConstant sched(model);
  sched.setName("Constant Schedule");
  sched.setValue(1.0);
  cc.setConvectionCoefficient2Schedule(sched);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::SurfaceProperty_ConvectionCoefficients));
  ASSERT_EQ(objVector.size(), 1);
  WorkspaceObject wo(objVector.at(0));

  boost::optional<std::string> cc1loc(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location));
  EXPECT_TRUE(cc1loc);
  if (cc1loc) {
    EXPECT_EQ(cc1loc.get(), "Inside");
  }
  boost::optional<std::string> cc1type(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type));
  EXPECT_TRUE(cc1type);
  if (cc1type) {
    EXPECT_EQ(cc1type.get(), "Value");
  }
  boost::optional<double> cc1value(wo.getDouble(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1));
  EXPECT_TRUE(cc1value);
  if (cc1value) {
    EXPECT_EQ(cc1value.get(), 3.0);
  }
  boost::optional<std::string> cc2loc(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location));
  EXPECT_TRUE(cc2loc);
  if (cc2loc) {
    EXPECT_EQ(cc2loc.get(), "Outside");
  }
  boost::optional<std::string> cc2type(wo.getString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type));
  EXPECT_TRUE(cc2type);
  if (cc2type) {
    EXPECT_EQ(cc2type.get(), "Schedule");
  }
  boost::optional<WorkspaceObject> woSched(wo.getTarget(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName));
  EXPECT_TRUE(woSched);
  if (woSched) {
    EXPECT_EQ(woSched->iddObject().type(), IddObjectType::Schedule_Constant);
  }
  boost::optional<WorkspaceObject> woSurface(wo.getTarget(SurfaceProperty_ConvectionCoefficientsFields::SurfaceName));
  EXPECT_TRUE(woSurface);
  if (woSurface) {
    EXPECT_EQ(woSurface->iddObject().type(), IddObjectType::BuildingSurface_Detailed);
  }
}
