/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../SurfacePropertyGroundSurfaces.hpp"
#include "../SurfacePropertyGroundSurfaces_Impl.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyGroundSurfaces_GettersSetters) {
  Model m;

  ScheduleConstant tempSch(m);
  tempSch.setValue(25.0);
  ScheduleConstant reflectanceSch(m);
  reflectanceSch.setValue(1.0);

  {
    // Wrong viewFactor (0 to 1)
    EXPECT_THROW(GroundSurfaceGroup("External Surface", 1.2), openstudio::Exception);
    EXPECT_NO_THROW(GroundSurfaceGroup("External Surface", 0.2));

    EXPECT_NO_THROW(GroundSurfaceGroup("External Surface", 0.2, tempSch));

    EXPECT_NO_THROW(GroundSurfaceGroup("External Surface", 0.2, tempSch, reflectanceSch));
  }

  SurfacePropertyGroundSurfaces sp(m);

  EXPECT_EQ(0, sp.numberofGroundSurfaceGroups());
  EXPECT_TRUE(sp.addGroundSurfaceGroup("External Surface 1", 0.2, tempSch, reflectanceSch));
  EXPECT_EQ(1, sp.numberofGroundSurfaceGroups());

  auto group2 = GroundSurfaceGroup("External Surface 2", 0.3, tempSch, reflectanceSch);
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
  ASSERT_TRUE(group_->reflectanceSchedule());
  EXPECT_EQ(reflectanceSch, group_->reflectanceSchedule().get());

  // Test that you cannot add the same group twice (based on Surface Name), but that instead it'll overwrite the values
  auto group2bis = GroundSurfaceGroup("External Surface 2", 0.45, tempSch2);
  // Test that we can locate an existing ViewFactor correctly (comparing on to/from only, not viewFactor value)
  boost::optional<unsigned> _existingIndex = sp.groundSurfaceGroupIndex(group2bis);
  ASSERT_TRUE(_existingIndex);
  EXPECT_EQ(1U, _existingIndex.get());
  // Test with the std::string version
  _existingIndex = sp.groundSurfaceGroupIndex("External Surface 2");
  ASSERT_TRUE(_existingIndex);
  EXPECT_EQ(1U, _existingIndex.get());

  // Now call add, which should jut override it
  EXPECT_TRUE(sp.addGroundSurfaceGroup(group2bis));
  // Should still have the same number
  EXPECT_EQ(3, sp.numberofGroundSurfaceGroups());

  // Retrieve it again, making sure it changed only the values viewFactor and Schedule
  boost::optional<GroundSurfaceGroup> group2_ = sp.getGroundSurfaceGroup(1U);
  ASSERT_TRUE(group2_);
  EXPECT_EQ("External Surface 2", group2_->groundSurfaceName());
  EXPECT_EQ(0.45, group2_->viewFactor());
  ASSERT_TRUE(group2_->temperatureSchedule());
  EXPECT_EQ(tempSch2, group2_->temperatureSchedule().get());
  EXPECT_FALSE(group2_->reflectanceSchedule());

  // Via the overloaded method
  EXPECT_TRUE(sp.addGroundSurfaceGroup("External Surface 2", 0.32, tempSch, reflectanceSch));
  boost::optional<GroundSurfaceGroup> group3_ = sp.getGroundSurfaceGroup(1U);
  ASSERT_TRUE(group3_);
  EXPECT_EQ("External Surface 2", group3_->groundSurfaceName());
  EXPECT_EQ(0.32, group3_->viewFactor());
  ASSERT_TRUE(group3_->temperatureSchedule());
  EXPECT_EQ(tempSch, group3_->temperatureSchedule().get());
  ASSERT_TRUE(group3_->reflectanceSchedule());
  EXPECT_EQ(reflectanceSch, group3_->reflectanceSchedule().get());

  // Test that you cannot get a group by an index that's too high
  EXPECT_FALSE(sp.getGroundSurfaceGroup(sp.numberofGroundSurfaceGroups()));

  // Test that you cannot find a group if it doesn't exist
  auto groupother = GroundSurfaceGroup("Another surface", 0.45, tempSch2);
  EXPECT_FALSE(sp.groundSurfaceGroupIndex(groupother));

  // Remove a viewFactor
  EXPECT_FALSE(sp.removeGroundSurfaceGroup(1000));
  EXPECT_EQ(3, sp.numberofGroundSurfaceGroups());

  EXPECT_TRUE(sp.removeGroundSurfaceGroup(1));
  EXPECT_EQ(2, sp.numberofGroundSurfaceGroups());

  // check that remaining blocks moved correctly
  std::vector<GroundSurfaceGroup> groups = sp.groundSurfaceGroups();
  EXPECT_EQ(2U, groups.size());
  EXPECT_EQ("External Surface 1", groups[0].groundSurfaceName());
  EXPECT_EQ(0.2, groups[0].viewFactor());
  ASSERT_TRUE(groups[0].temperatureSchedule());
  EXPECT_EQ(tempSch, groups[0].temperatureSchedule().get());
  ASSERT_TRUE(groups[0].reflectanceSchedule());
  EXPECT_EQ(reflectanceSch, groups[0].reflectanceSchedule().get());

  EXPECT_EQ("External Surface 3", groups[1].groundSurfaceName());
  EXPECT_EQ(0.4, groups[1].viewFactor());
  ASSERT_TRUE(groups[1].temperatureSchedule());
  EXPECT_EQ(tempSch2, groups[1].temperatureSchedule().get());
  EXPECT_FALSE(groups[1].reflectanceSchedule());

  // more remove checking
  sp.removeAllGroundSurfaceGroups();
  EXPECT_EQ(0, sp.numberofGroundSurfaceGroups());
  EXPECT_FALSE(sp.removeGroundSurfaceGroup(0));
  EXPECT_EQ(0, sp.numberofGroundSurfaceGroups());

  // Via a vector
  EXPECT_TRUE(sp.addGroundSurfaceGroups({group2bis, groupother, group3}));
  groups = sp.groundSurfaceGroups();
  EXPECT_EQ(3U, groups.size());
  EXPECT_EQ(group2bis.groundSurfaceName(), groups[0].groundSurfaceName());
  EXPECT_EQ(group2bis.viewFactor(), groups[0].viewFactor());
  ASSERT_TRUE(groups[0].temperatureSchedule());
  EXPECT_EQ(group2bis.temperatureSchedule().get(), groups[0].temperatureSchedule().get());
  EXPECT_FALSE(groups[0].reflectanceSchedule());

  EXPECT_EQ(groupother.groundSurfaceName(), groups[1].groundSurfaceName());
  EXPECT_EQ(groupother.viewFactor(), groups[1].viewFactor());
  ASSERT_TRUE(groups[1].temperatureSchedule());
  EXPECT_EQ(groupother.temperatureSchedule().get(), groups[1].temperatureSchedule().get());
  EXPECT_FALSE(groups[1].reflectanceSchedule());

  EXPECT_EQ(group3.groundSurfaceName(), groups[2].groundSurfaceName());
  EXPECT_EQ(group3.viewFactor(), groups[2].viewFactor());
  ASSERT_TRUE(groups[2].temperatureSchedule());
  EXPECT_EQ(group3.temperatureSchedule().get(), groups[2].temperatureSchedule().get());
  EXPECT_FALSE(groups[2].reflectanceSchedule());
}
