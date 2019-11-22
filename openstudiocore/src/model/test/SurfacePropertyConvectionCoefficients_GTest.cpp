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

#include <algorithm>
#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../InternalMass.hpp"
#include "../InternalMass_Impl.hpp"
#include "../InternalMassDefinition.hpp"
#include "../InternalMassDefinition_Impl.hpp"
#include "../SurfacePropertyConvectionCoefficients.hpp"
#include "../SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyConvectionCoefficients) {
  Model model;
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));

  Surface surface(vertices, model);
  SurfacePropertyConvectionCoefficients cc(surface);

  ModelObject surfaceModelObj = cc.surfaceAsModelObject();
  ASSERT_EQ(surfaceModelObj, surface);
  OptionalSurface surface2 = cc.surfaceAsSurface();
  OptionalSubSurface subsurface2 = cc.surfaceAsSubSurface();
  OptionalInternalMass internalmass2 = cc.surfaceAsInternalMass();
  ASSERT_TRUE(surface2);
  ASSERT_FALSE(subsurface2);
  ASSERT_FALSE(internalmass2);
  ASSERT_EQ(*surface2, surface);
  ASSERT_EQ(surface2->surfacePropertyConvectionCoefficients(), cc);

  SubSurface subSurface(vertices, model);
  ASSERT_TRUE(cc.setSurface(subSurface));
  surfaceModelObj = cc.surfaceAsModelObject();
  ASSERT_EQ(surfaceModelObj, subSurface);
  surface2 = cc.surfaceAsSurface();
  subsurface2 = cc.surfaceAsSubSurface();
  internalmass2 = cc.surfaceAsInternalMass();
  ASSERT_FALSE(surface2);
  ASSERT_TRUE(subsurface2);
  ASSERT_FALSE(internalmass2);
  ASSERT_EQ(*subsurface2, subSurface);
  ASSERT_EQ(subsurface2->surfacePropertyConvectionCoefficients(), cc);

  InternalMassDefinition internalMassDefn(model);
  InternalMass internalMass(internalMassDefn);
  ASSERT_TRUE(cc.setSurface(internalMass));
  surfaceModelObj = cc.surfaceAsModelObject();
  ASSERT_EQ(surfaceModelObj, internalMass);
  surface2 = cc.surfaceAsSurface();
  subsurface2 = cc.surfaceAsSubSurface();
  internalmass2 = cc.surfaceAsInternalMass();
  ASSERT_FALSE(surface2);
  ASSERT_FALSE(subsurface2);
  ASSERT_TRUE(internalmass2);
  ASSERT_EQ(*internalmass2, internalMass);
  ASSERT_EQ(internalmass2->surfacePropertyConvectionCoefficients(), cc);

  std::vector<std::string> locationValues1(cc.convectionCoefficient1LocationValues());
  ASSERT_EQ(locationValues1.size(), 2);
  ASSERT_NE(std::find(locationValues1.begin(), locationValues1.end(), "Outside"), locationValues1.end()) << "'Outside' not in list'";
  ASSERT_NE(std::find(locationValues1.begin(), locationValues1.end(), "Inside"), locationValues1.end()) << "'Inside' not in list'";

  std::vector<std::string> typeValues1(cc.convectionCoefficient1TypeValues());
  ASSERT_EQ(typeValues1.size(), 43);
  ASSERT_NE(std::find(typeValues1.begin(), typeValues1.end(), "Value"), typeValues1.end()) << "'Value' not in list";
  ASSERT_NE(std::find(typeValues1.begin(), typeValues1.end(), "ASHRAEVerticalWall"), typeValues1.end()) << "'ASHRAEVerticalWall' not in list";
  ASSERT_NE(std::find(typeValues1.begin(), typeValues1.end(), "BeausoleilMorrisonMixedStableFloor"), typeValues1.end()) << "'BeausoleilMorrisonMixedStableFloor' not in list";
  ASSERT_NE(std::find(typeValues1.begin(), typeValues1.end(), "ClearRoof"), typeValues1.end()) << "'ClearRoof' not in list";

  std::vector<std::string> locationValues2(cc.convectionCoefficient2LocationValues());
  ASSERT_EQ(locationValues1, locationValues2);

  std::vector<std::string> typeValues2(cc.convectionCoefficient2TypeValues());
  ASSERT_EQ(typeValues1, typeValues2);

  OptionalString location(cc.convectionCoefficient1Location());
  ASSERT_FALSE(location);
  ASSERT_TRUE(cc.setConvectionCoefficient1Location("Inside"));
  location = cc.convectionCoefficient1Location();
  ASSERT_TRUE(location);
  ASSERT_EQ(*location, "Inside");
  ASSERT_TRUE(cc.setConvectionCoefficient1Location("Outside"));
  location = cc.convectionCoefficient1Location();
  ASSERT_TRUE(location);
  ASSERT_EQ(*location, "Outside");
  cc.resetConvectionCoefficient1Location();
  location = cc.convectionCoefficient1Location();
  ASSERT_FALSE(location);

  location = cc.convectionCoefficient2Location();
  ASSERT_FALSE(location);
  ASSERT_TRUE(cc.setConvectionCoefficient2Location("Inside"));
  location = cc.convectionCoefficient2Location();
  ASSERT_TRUE(location);
  ASSERT_EQ(*location, "Inside");
  ASSERT_TRUE(cc.setConvectionCoefficient2Location("Outside"));
  location = cc.convectionCoefficient2Location();
  ASSERT_TRUE(location);
  ASSERT_EQ(*location, "Outside");
  cc.resetConvectionCoefficient2Location();
  location = cc.convectionCoefficient2Location();
  ASSERT_FALSE(location);

  OptionalString convType(cc.convectionCoefficient1Type());
  ASSERT_FALSE(convType);
  ASSERT_TRUE(cc.setConvectionCoefficient1Type("TARP"));
  convType = cc.convectionCoefficient1Type();
  ASSERT_TRUE(convType);
  ASSERT_EQ(*convType, "TARP");
  ASSERT_TRUE(cc.setConvectionCoefficient1Type("Value"));
  convType = cc.convectionCoefficient1Type();
  ASSERT_TRUE(convType);
  ASSERT_EQ(*convType, "Value");
  cc.resetConvectionCoefficient1Type();
  convType = cc.convectionCoefficient1Type();
  ASSERT_FALSE(convType);

  convType = cc.convectionCoefficient2Type();
  ASSERT_FALSE(convType);
  ASSERT_TRUE(cc.setConvectionCoefficient2Type("TARP"));
  convType = cc.convectionCoefficient2Type();
  ASSERT_TRUE(convType);
  ASSERT_EQ(*convType, "TARP");
  ASSERT_TRUE(cc.setConvectionCoefficient2Type("Value"));
  convType = cc.convectionCoefficient2Type();
  ASSERT_TRUE(convType);
  ASSERT_EQ(*convType, "Value");
  cc.resetConvectionCoefficient2Type();
  convType = cc.convectionCoefficient2Type();
  ASSERT_FALSE(convType);

  OptionalDouble coef(cc.convectionCoefficient1());
  ASSERT_FALSE(coef);
  ASSERT_TRUE(cc.setConvectionCoefficient1(1.0));
  coef = cc.convectionCoefficient1();
  ASSERT_TRUE(coef);
  ASSERT_FLOAT_EQ(*coef, 1.0);
  cc.resetConvectionCoefficient1();
  coef = cc.convectionCoefficient1();
  ASSERT_FALSE(coef);

  coef = cc.convectionCoefficient2();
  ASSERT_FALSE(coef);
  ASSERT_TRUE(cc.setConvectionCoefficient2(2.0));
  coef = cc.convectionCoefficient2();
  ASSERT_TRUE(coef);
  ASSERT_FLOAT_EQ(*coef, 2.0);
  cc.resetConvectionCoefficient2();
  coef = cc.convectionCoefficient2();
  ASSERT_FALSE(coef);

  OptionalSchedule optSched(cc.convectionCoefficient1Schedule());
  ASSERT_FALSE(optSched);
  ScheduleConstant sched(model);
  sched.setValue(1.0);
  cc.setConvectionCoefficient1Schedule(sched);
  optSched = cc.convectionCoefficient1Schedule();
  ASSERT_TRUE(optSched);
  ASSERT_EQ(*optSched, sched);
  cc.resetConvectionCoefficient1Schedule();
  optSched = cc.convectionCoefficient1Schedule();
  ASSERT_FALSE(optSched);

  optSched = cc.convectionCoefficient2Schedule();
  ASSERT_FALSE(optSched);
  cc.setConvectionCoefficient2Schedule(sched);
  optSched = cc.convectionCoefficient2Schedule();
  ASSERT_TRUE(optSched);
  ASSERT_EQ(*optSched, sched);
  cc.resetConvectionCoefficient2Schedule();
  optSched = cc.convectionCoefficient2Schedule();
  ASSERT_FALSE(optSched);

  SurfacePropertyConvectionCoefficients cc2(surface);
  boost::optional<SurfacePropertyConvectionCoefficients> cc3(surface.surfacePropertyConvectionCoefficients());
  ASSERT_TRUE(cc3);
  ASSERT_EQ(cc3.get().handle(), cc2.handle());

}
