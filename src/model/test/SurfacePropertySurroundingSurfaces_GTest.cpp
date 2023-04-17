/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../SurfacePropertySurroundingSurfaces.hpp"
#include "../SurfacePropertySurroundingSurfaces_Impl.hpp"

#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertySurroundingSurfaces_GettersSetters) {
  Model m;

  SurfacePropertySurroundingSurfaces surfacePropertySurroundingSurfaces(m);

  surfacePropertySurroundingSurfaces.setName("My SurfacePropertySurroundingSurfaces");

  // Sky View Factor: Optional Double
  EXPECT_TRUE(surfacePropertySurroundingSurfaces.isSkyViewFactorAutocalculated());

  EXPECT_TRUE(surfacePropertySurroundingSurfaces.setSkyViewFactor(0.5));
  ASSERT_TRUE(surfacePropertySurroundingSurfaces.skyViewFactor());
  EXPECT_EQ(0.5, surfacePropertySurroundingSurfaces.skyViewFactor().get());
  // Bad Value
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.setSkyViewFactor(-10.0));
  ASSERT_TRUE(surfacePropertySurroundingSurfaces.skyViewFactor());
  EXPECT_EQ(0.5, surfacePropertySurroundingSurfaces.skyViewFactor().get());
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.isSkyViewFactorAutocalculated());

  surfacePropertySurroundingSurfaces.autocalculateSkyViewFactor();
  EXPECT_TRUE(surfacePropertySurroundingSurfaces.isSkyViewFactorAutocalculated());

  // Sky Temperature Schedule Name: Optional Object
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.skyTemperatureSchedule());
  ScheduleConstant skyTemp(m);
  skyTemp.setValue(25.0);
  EXPECT_TRUE(surfacePropertySurroundingSurfaces.setSkyTemperatureSchedule(skyTemp));
  ASSERT_TRUE(surfacePropertySurroundingSurfaces.skyTemperatureSchedule());
  EXPECT_EQ(skyTemp, surfacePropertySurroundingSurfaces.skyTemperatureSchedule().get());
  surfacePropertySurroundingSurfaces.resetSkyTemperatureSchedule();
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.skyTemperatureSchedule());

  // Ground View Factor: Optional Double
  EXPECT_TRUE(surfacePropertySurroundingSurfaces.isGroundViewFactorAutocalculated());

  EXPECT_TRUE(surfacePropertySurroundingSurfaces.setGroundViewFactor(0.65));
  ASSERT_TRUE(surfacePropertySurroundingSurfaces.groundViewFactor());
  EXPECT_EQ(0.65, surfacePropertySurroundingSurfaces.groundViewFactor().get());
  // Bad Value
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.setGroundViewFactor(-10.0));
  ASSERT_TRUE(surfacePropertySurroundingSurfaces.groundViewFactor());
  EXPECT_EQ(0.65, surfacePropertySurroundingSurfaces.groundViewFactor().get());
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.isGroundViewFactorAutocalculated());

  surfacePropertySurroundingSurfaces.autocalculateGroundViewFactor();
  EXPECT_TRUE(surfacePropertySurroundingSurfaces.isGroundViewFactorAutocalculated());

  // Ground Temperature Schedule Name: Optional Object
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.skyTemperatureSchedule());
  ScheduleConstant groundTemp(m);
  groundTemp.setValue(25.0);
  EXPECT_TRUE(surfacePropertySurroundingSurfaces.setGroundTemperatureSchedule(groundTemp));
  ASSERT_TRUE(surfacePropertySurroundingSurfaces.groundTemperatureSchedule());
  EXPECT_EQ(groundTemp, surfacePropertySurroundingSurfaces.groundTemperatureSchedule().get());
  surfacePropertySurroundingSurfaces.resetGroundTemperatureSchedule();
  EXPECT_FALSE(surfacePropertySurroundingSurfaces.skyTemperatureSchedule());
}

TEST_F(ModelFixture, SurfacePropertySurroundingSurfaces_SurroundingSurfaceGroups) {

  Model m;
  ScheduleConstant surfaceTempSch(m);
  surfaceTempSch.setValue(25.0);

  // Wrong viewFactor (0 to 1)
  EXPECT_THROW(SurroundingSurfaceGroup("External Surface", 1.2, surfaceTempSch), openstudio::Exception);

  // Name can't ve empty
  EXPECT_THROW(SurroundingSurfaceGroup("External Surface", 1.2, surfaceTempSch), openstudio::Exception);

  EXPECT_NO_THROW(SurroundingSurfaceGroup("External Surface", 0.2, surfaceTempSch));

  SurfacePropertySurroundingSurfaces sp(m);

  EXPECT_EQ(0, sp.numberofSurroundingSurfaceGroups());
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup("External Surface 1", 0.2, surfaceTempSch));
  EXPECT_EQ(1, sp.numberofSurroundingSurfaceGroups());

  auto group2 = SurroundingSurfaceGroup("External Surface 2", 0.3, surfaceTempSch);
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup(group2));
  EXPECT_EQ(2, sp.numberofSurroundingSurfaceGroups());

  ScheduleConstant surfaceTempSch2(m);
  surfaceTempSch.setValue(35.0);
  auto group3 = SurroundingSurfaceGroup("External Surface 3", 0.4, surfaceTempSch2);
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup(group3));
  EXPECT_EQ(3, sp.numberofSurroundingSurfaceGroups());

  boost::optional<SurroundingSurfaceGroup> group_ = sp.getSurroundingSurfaceGroup(1U);
  ASSERT_TRUE(group_);
  EXPECT_EQ("External Surface 2", group_->surroundingSurfaceName());
  EXPECT_EQ(0.3, group_->viewFactor());
  EXPECT_EQ(surfaceTempSch, group_->temperatureSchedule());

  // Test that you cannot add the same group twice (based on Surface Name), but that instead it'll overwrite the values
  auto group2bis = SurroundingSurfaceGroup("External Surface 2", 0.45, surfaceTempSch2);
  // Test that we can locate an existing ViewFactor correctly (comparing on to/from only, not viewFactor value)
  boost::optional<unsigned> _existingIndex = sp.surroundingSurfaceGroupIndex(group2bis);
  ASSERT_TRUE(_existingIndex);
  EXPECT_EQ(1U, _existingIndex.get());
  // Test with the std::string version
  _existingIndex = sp.surroundingSurfaceGroupIndex("External Surface 2");
  ASSERT_TRUE(_existingIndex);
  EXPECT_EQ(1U, _existingIndex.get());

  // Now call add, which should jut override it
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup(group2bis));
  // Should still have the same number
  EXPECT_EQ(3, sp.numberofSurroundingSurfaceGroups());

  // Retrieve it again, making sure it changed only the values viewFactor and Schedule
  boost::optional<SurroundingSurfaceGroup> group2_ = sp.getSurroundingSurfaceGroup(1U);
  ASSERT_TRUE(group2_);
  EXPECT_EQ("External Surface 2", group2_->surroundingSurfaceName());
  EXPECT_EQ(0.45, group2_->viewFactor());
  EXPECT_EQ(surfaceTempSch2, group2_->temperatureSchedule());

  // Via the overloaded method
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup("External Surface 2", 0.32, surfaceTempSch));
  boost::optional<SurroundingSurfaceGroup> group3_ = sp.getSurroundingSurfaceGroup(1U);
  ASSERT_TRUE(group3_);
  EXPECT_EQ("External Surface 2", group3_->surroundingSurfaceName());
  EXPECT_EQ(0.32, group3_->viewFactor());
  EXPECT_EQ(surfaceTempSch, group3_->temperatureSchedule());

  // Test that you cannot get a group by an index that's too high
  EXPECT_FALSE(sp.getSurroundingSurfaceGroup(sp.numberofSurroundingSurfaceGroups()));

  // Test that you cannot find a group if it doesn't exist
  auto groupother = SurroundingSurfaceGroup("Another surface", 0.45, surfaceTempSch2);
  EXPECT_FALSE(sp.surroundingSurfaceGroupIndex(groupother));

  // Remove a viewFactor
  EXPECT_FALSE(sp.removeSurroundingSurfaceGroup(1000));
  EXPECT_EQ(3, sp.numberofSurroundingSurfaceGroups());

  EXPECT_TRUE(sp.removeSurroundingSurfaceGroup(1));
  EXPECT_EQ(2, sp.numberofSurroundingSurfaceGroups());

  // check that remaining blocks moved correctly
  std::vector<SurroundingSurfaceGroup> groups = sp.surroundingSurfaceGroups();
  EXPECT_EQ(2U, groups.size());
  EXPECT_EQ("External Surface 1", groups[0].surroundingSurfaceName());
  EXPECT_EQ(0.2, groups[0].viewFactor());
  EXPECT_EQ(surfaceTempSch, groups[0].temperatureSchedule());

  EXPECT_EQ("External Surface 3", groups[1].surroundingSurfaceName());
  EXPECT_EQ(0.4, groups[1].viewFactor());
  EXPECT_EQ(surfaceTempSch2, groups[1].temperatureSchedule());

  // more remove checking
  sp.removeAllSurroundingSurfaceGroups();
  EXPECT_EQ(0, sp.numberofSurroundingSurfaceGroups());
  EXPECT_FALSE(sp.removeSurroundingSurfaceGroup(0));
  EXPECT_EQ(0, sp.numberofSurroundingSurfaceGroups());

  // Via a vector
  EXPECT_TRUE(sp.addSurroundingSurfaceGroups({group2bis, groupother, group3}));
  groups = sp.surroundingSurfaceGroups();
  EXPECT_EQ(3U, groups.size());
  EXPECT_EQ(group2bis.surroundingSurfaceName(), groups[0].surroundingSurfaceName());
  EXPECT_EQ(group2bis.viewFactor(), groups[0].viewFactor());
  EXPECT_EQ(group2bis.temperatureSchedule(), groups[0].temperatureSchedule());

  EXPECT_EQ(groupother.surroundingSurfaceName(), groups[1].surroundingSurfaceName());
  EXPECT_EQ(groupother.viewFactor(), groups[1].viewFactor());
  EXPECT_EQ(groupother.temperatureSchedule(), groups[1].temperatureSchedule());

  EXPECT_EQ(group3.surroundingSurfaceName(), groups[2].surroundingSurfaceName());
  EXPECT_EQ(group3.viewFactor(), groups[2].viewFactor());
  EXPECT_EQ(group3.temperatureSchedule(), groups[2].temperatureSchedule());
}
