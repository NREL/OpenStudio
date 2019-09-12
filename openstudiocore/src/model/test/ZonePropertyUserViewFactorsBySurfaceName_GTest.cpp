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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../InternalMass.hpp"
#include "../InternalMass_Impl.hpp"
#include "../InternalMassDefinition.hpp"
#include "../InternalMassDefinition_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZonePropertyUserViewFactorsBySufaceName_ZonePropertyUserViewFactorsBySufaceName_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a thermal zone object to use
      ThermalZone thermalZone(model);

      ZonePropertyUserViewFactorsBySurfaceName zoneProp = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();

      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );
}

TEST_F(ModelFixture, ZonePropertyUserViewFactorsBySufaceName_ZonePropertyUserViewFactorsBySufaceName_Uniqueness) {

  Model model;
  ThermalZone thermalZone(model);
  auto size = model.modelObjects().size();

  // new thermal zone will create a zone property user view factors by surface name object
  ZonePropertyUserViewFactorsBySurfaceName zoneProp1 = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();
  EXPECT_EQ(thermalZone.handle(), zoneProp1.thermalZone().handle());
  EXPECT_EQ(size+1, model.modelObjects().size());
  // This should be the same one
  ZonePropertyUserViewFactorsBySurfaceName zoneProp2 = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();
  EXPECT_EQ(size+1, model.modelObjects().size());
  EXPECT_EQ(zoneProp1, zoneProp2);

  // check defaults
  EXPECT_EQ(0, zoneProp2.numberofViewFactors());
  EXPECT_EQ(0, zoneProp2.viewFactors().size());
  EXPECT_EQ("Thermal Zone 1", zoneProp2.thermalZone().name().get());

  // Direct Ctor should throw since there is already one
  EXPECT_THROW((ZonePropertyUserViewFactorsBySurfaceName(thermalZone)), openstudio::Exception);
  EXPECT_EQ(size+1, model.modelObjects().size());

  // Clone is disallowed in all cases
  EXPECT_THROW(zoneProp1.clone(model), openstudio::Exception);
  EXPECT_EQ(size+1, model.modelObjects().size());

}

TEST_F(ModelFixture, ZonePropertyUserViewFactorsBySufaceName_AddAndRemove) {
  Model model;
  ThermalZone thermalZone(model);
  ZonePropertyUserViewFactorsBySurfaceName zoneProp = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();

  zoneProp.removeAllViewFactors();

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface fromSurface(points, model);
  Surface toSurface(points, model);
  SubSurface fromSubSurface(points, model);
  SubSurface toSubSurface(points, model);
  InternalMassDefinition fromDefinition(model);
  InternalMass fromInternalMass(fromDefinition);
  InternalMassDefinition toDefinition(model);
  InternalMass toInternalMass(toDefinition);

  EXPECT_EQ(0, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromSurface, toSurface, 0.5));
  EXPECT_EQ(1, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromSurface, toSubSurface, 1));
  EXPECT_EQ(2, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromSurface, toInternalMass, 1.0));
  EXPECT_EQ(3, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromSubSurface, toSubSurface, 0));
  EXPECT_EQ(4, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromSubSurface, toSurface, 0.0));
  EXPECT_EQ(5, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromSubSurface, toInternalMass, -2));
  EXPECT_EQ(6, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromInternalMass, toInternalMass, -1));
  EXPECT_EQ(7, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromInternalMass, toSurface, -1.5));
  EXPECT_EQ(8, zoneProp.numberofViewFactors());
  EXPECT_TRUE(zoneProp.addViewFactor(fromInternalMass, toSubSurface, -0.25));
  EXPECT_EQ(9, zoneProp.numberofViewFactors());

  zoneProp.removeViewFactor(3);
  EXPECT_EQ(8, zoneProp.numberofViewFactors());

  // check that remaining blocks moved correctly
  std::vector<ViewFactor> viewFactors = zoneProp.viewFactors();

  EXPECT_TRUE(viewFactors[0].fromSurfaceAsSurface());
  EXPECT_TRUE(viewFactors[0].toSurfaceAsSurface());
  EXPECT_EQ(fromSurface, viewFactors[0].fromSurfaceAsSurface());
  EXPECT_EQ(toSurface, viewFactors[0].toSurfaceAsSurface());
  EXPECT_FALSE(viewFactors[0].fromSurfaceAsSubSurface());
  EXPECT_FALSE(viewFactors[0].fromSurfaceAsInternalMass());
  EXPECT_FALSE(viewFactors[0].toSurfaceAsSubSurface());
  EXPECT_FALSE(viewFactors[0].toSurfaceAsInternalMass());
  EXPECT_EQ(0.5, viewFactors[0].viewFactor());

  EXPECT_TRUE(viewFactors[5].fromSurfaceAsInternalMass());
  EXPECT_TRUE(viewFactors[5].toSurfaceAsInternalMass());
  EXPECT_EQ(fromInternalMass, viewFactors[5].fromSurfaceAsInternalMass());
  EXPECT_EQ(toInternalMass, viewFactors[5].toSurfaceAsInternalMass());
  EXPECT_FALSE(viewFactors[5].fromSurfaceAsSurface());
  EXPECT_FALSE(viewFactors[5].fromSurfaceAsSubSurface());
  EXPECT_FALSE(viewFactors[5].toSurfaceAsSurface());
  EXPECT_FALSE(viewFactors[5].toSurfaceAsSubSurface());
  EXPECT_EQ(-1, viewFactors[5].viewFactor());

  // more remove checking
  zoneProp.removeAllViewFactors();
  EXPECT_EQ(0, zoneProp.numberofViewFactors());
  zoneProp.removeViewFactor(0);
  EXPECT_EQ(0, zoneProp.numberofViewFactors());

  // check adding view factor non-conveniently
  ViewFactor viewFactor(fromSurface, toSurface, 0.333);
  ASSERT_TRUE(zoneProp.addViewFactor(viewFactor));
  EXPECT_EQ(1, zoneProp.numberofViewFactors());

  // check bulk-adding custom blocks
  std::vector<ViewFactor> viewFactorsToAdd;
  ViewFactor viewFactor1(fromSurface, toSurface, 0.0001);
  viewFactorsToAdd.push_back(viewFactor1);
  ViewFactor viewFactor2(fromSubSurface, toSubSurface, -0.000001);
  viewFactorsToAdd.push_back(viewFactor2);
  ASSERT_TRUE(zoneProp.addViewFactors(viewFactorsToAdd));
  EXPECT_EQ(3, zoneProp.numberofViewFactors());
  EXPECT_EQ(3, zoneProp.viewFactors().size());
}
