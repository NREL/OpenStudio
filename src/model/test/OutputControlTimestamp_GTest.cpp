/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../OutputControlTimestamp.hpp"
#include "../OutputControlTimestamp_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputControlTimestamp_GettersSetters) {
  Model m;
  OutputControlTimestamp outputControlTimestamp = m.getUniqueModelObject<OutputControlTimestamp>();

  EXPECT_FALSE(outputControlTimestamp.iso8601Format());
  EXPECT_TRUE(outputControlTimestamp.setISO8601Format(true));
  EXPECT_TRUE(outputControlTimestamp.iso8601Format());
  EXPECT_TRUE(outputControlTimestamp.setISO8601Format(false));
  EXPECT_FALSE(outputControlTimestamp.iso8601Format());

  EXPECT_FALSE(outputControlTimestamp.timestampAtBeginningOfInterval());
  EXPECT_TRUE(outputControlTimestamp.setTimestampAtBeginningOfInterval(true));
  EXPECT_TRUE(outputControlTimestamp.timestampAtBeginningOfInterval());
  EXPECT_TRUE(outputControlTimestamp.setTimestampAtBeginningOfInterval(false));
  EXPECT_FALSE(outputControlTimestamp.timestampAtBeginningOfInterval());
}
