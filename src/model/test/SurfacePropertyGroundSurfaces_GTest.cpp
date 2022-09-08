/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../SurfacePropertyGroundSurfaces.hpp"
#include "../SurfacePropertyGroundSurfaces_Impl.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyGroundSurfaces_GettersSetters) {
  Model m;

  // Wrong viewFactor (0 to 1)
  EXPECT_THROW(GroundSurfaceGroup("External Surface", 1.2), openstudio::Exception);
  EXPECT_NO_THROW(GroundSurfaceGroup("External Surface", 0.2));

  ScheduleConstant tempSch(m);
  tempSch.setValue(25.0);

  EXPECT_NO_THROW(GroundSurfaceGroup("External Surface", 0.2, tempSch));

  ScheduleConstant reflectanceSchedule(m);
  reflectanceSchedule.setValue(1.0);
  EXPECT_NO_THROW(GroundSurfaceGroup("External Surface", 0.2, tempSch, reflectanceSchedule));

  SurfacePropertyGroundSurfaces sp(m);

  EXPECT_EQ(0, sp.numberofGroundSurfaceGroups());
  EXPECT_TRUE(sp.addGroundSurfaceGroup("External Surface 1", 0.2, tempSch, reflectanceSchedule));
  EXPECT_EQ(1, sp.numberofGroundSurfaceGroups());

  auto group2 = GroundSurfaceGroup("External Surface 2", 0.3, tempSch, reflectanceSchedule);
  EXPECT_TRUE(sp.addGroundSurfaceGroup(group2));
  EXPECT_EQ(2, sp.numberofGroundSurfaceGroups());

  ScheduleConstant tempSch2(m);
  tempSch2.setValue(35.0);
  auto group3 = GroundSurfaceGroup("External Surface 3", 0.4, tempSch2);
  EXPECT_TRUE(sp.addGroundSurfaceGroup(group3));
  EXPECT_EQ(3, sp.numberofGroundSurfaceGroups());

  boost::optional<GroundSurfaceGroup> group_ = sp.getGroundSurfaceGroup(1U);
  ASSERT_TRUE(group_);
  EXPECT_EQ("External Surface 2", group_->groundSurfaceName());
  EXPECT_EQ(0.3, group_->viewFactor());
  ASSERT_TRUE(group_->temperatureSchedule());
  EXPECT_EQ(tempSch, group_->temperatureSchedule().get());

  //   // Test that you cannot add the same group twice (based on Surface Name), but that instead it'll overwrite the values
  //   auto group2bis = GroundSurfaceGroup("External Surface 2", 0.45, surfaceTempSch2);
  //   // Test that we can locate an existing ViewFactor correctly (comparing on to/from only, not viewFactor value)
  //   boost::optional<unsigned> _existingIndex = sp.groundSurfaceGroupIndex(group2bis);
  //   ASSERT_TRUE(_existingIndex);
  //   EXPECT_EQ(1U, _existingIndex.get());
  //   // Test with the std::string version
  //   _existingIndex = sp.groundSurfaceGroupIndex("External Surface 2");
  //   ASSERT_TRUE(_existingIndex);
  //   EXPECT_EQ(1U, _existingIndex.get());
  //
  //   // Now call add, which should jut override it
  //   EXPECT_TRUE(sp.addGroundSurfaceGroup(group2bis));
  //   // Should still have the same number
  //   EXPECT_EQ(3, sp.numberofGroundSurfaceGroups());
  //
  //   // Retrieve it again, making sure it changed only the values viewFactor and Schedule
  //   boost::optional<GroundSurfaceGroup> group2_ = sp.getGroundSurfaceGroup(1U);
  //   ASSERT_TRUE(group2_);
  //   EXPECT_EQ("External Surface 2", group2_->groundSurfaceName());
  //   EXPECT_EQ(0.45, group2_->viewFactor());
  //   EXPECT_EQ(surfaceTempSch2, group2_->temperatureSchedule());
  //
  //   // Via the overloaded method
  //   EXPECT_TRUE(sp.addGroundSurfaceGroup("External Surface 2", 0.32, surfaceTempSch));
  //   boost::optional<GroundSurfaceGroup> group3_ = sp.getGroundSurfaceGroup(1U);
  //   ASSERT_TRUE(group3_);
  //   EXPECT_EQ("External Surface 2", group3_->groundSurfaceName());
  //   EXPECT_EQ(0.32, group3_->viewFactor());
  //   EXPECT_EQ(surfaceTempSch, group3_->temperatureSchedule());
  //
  //   // Test that you cannot get a group by an index that's too high
  //   EXPECT_FALSE(sp.getGroundSurfaceGroup(sp.numberofGroundSurfaceGroups()));
  //
  //   // Test that you cannot find a group if it doesn't exist
  //   auto groupother = GroundSurfaceGroup("Another surface", 0.45, surfaceTempSch2);
  //   EXPECT_FALSE(sp.groundSurfaceGroupIndex(groupother));
  //
  //   // Remove a viewFactor
  //   EXPECT_FALSE(sp.removeGroundSurfaceGroup(1000));
  //   EXPECT_EQ(3, sp.numberofGroundSurfaceGroups());
  //
  //   EXPECT_TRUE(sp.removeGroundSurfaceGroup(1));
  //   EXPECT_EQ(2, sp.numberofGroundSurfaceGroups());
  //
  //   // check that remaining blocks moved correctly
  //   std::vector<GroundSurfaceGroup> groups = sp.groundSurfaceGroups();
  //   EXPECT_EQ(2U, groups.size());
  //   EXPECT_EQ("External Surface 1", groups[0].groundSurfaceName());
  //   EXPECT_EQ(0.2, groups[0].viewFactor());
  //   EXPECT_EQ(surfaceTempSch, groups[0].temperatureSchedule());
  //
  //   EXPECT_EQ("External Surface 3", groups[1].groundSurfaceName());
  //   EXPECT_EQ(0.4, groups[1].viewFactor());
  //   EXPECT_EQ(surfaceTempSch2, groups[1].temperatureSchedule());
  //
  //   // more remove checking
  //   sp.removeAllGroundSurfaceGroups();
  //   EXPECT_EQ(0, sp.numberofGroundSurfaceGroups());
  //   EXPECT_FALSE(sp.removeGroundSurfaceGroup(0));
  //   EXPECT_EQ(0, sp.numberofGroundSurfaceGroups());
  //
  //   // Via a vector
  //   EXPECT_TRUE(sp.addGroundSurfaceGroups({group2bis, groupother, group3}));
  //   groups = sp.groundSurfaceGroups();
  //   EXPECT_EQ(3U, groups.size());
  //   EXPECT_EQ(group2bis.groundSurfaceName(), groups[0].groundSurfaceName());
  //   EXPECT_EQ(group2bis.viewFactor(), groups[0].viewFactor());
  //   EXPECT_EQ(group2bis.temperatureSchedule(), groups[0].temperatureSchedule());
  //
  //   EXPECT_EQ(groupother.groundSurfaceName(), groups[1].groundSurfaceName());
  //   EXPECT_EQ(groupother.viewFactor(), groups[1].viewFactor());
  //   EXPECT_EQ(groupother.temperatureSchedule(), groups[1].temperatureSchedule());
  //
  //   EXPECT_EQ(group3.groundSurfaceName(), groups[2].groundSurfaceName());
  //   EXPECT_EQ(group3.viewFactor(), groups[2].viewFactor());
  //   EXPECT_EQ(group3.temperatureSchedule(), groups[2].temperatureSchedule());
  //   EXPECT_EQ(group3.temperatureSchedule(), groups[2].temperatureSchedule());
}
