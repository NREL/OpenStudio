/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IddFixture.hpp"
#include "../IddFileAndFactoryWrapper.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;

TEST_F(IddFixture, IddFileAndFactoryWrapper_ExplicitIddFile) {
  IddFileAndFactoryWrapper wrapper(IddFactory::instance().getIddFile(IddFileType::OpenStudio));

  EXPECT_EQ(IddFactory::instance().getIddFile(IddFileType::OpenStudio).objects().size(), wrapper.iddFile().objects().size());

  // Even though OpenStudio IddFile, it has been disembodied from IddFactory, so IddFileType
  // has been lost.
  EXPECT_TRUE(wrapper.iddFileType() == IddFileType(IddFileType::UserCustom));

  EXPECT_TRUE(wrapper.requiredObjects().size() == IddFactory::instance().getRequiredObjects(IddFileType::OpenStudio).size());
  EXPECT_TRUE(wrapper.uniqueObjects().size() == IddFactory::instance().getUniqueObjects(IddFileType::OpenStudio).size());

  EXPECT_TRUE(wrapper.isInFile(IddObjectType::OS_AirLoopHVAC));
  EXPECT_FALSE(wrapper.isInFile(IddObjectType::Window));
  EXPECT_FALSE(wrapper.getObject(IddObjectType::UserCustom));
}

TEST_F(IddFixture, IddFileAndFactoryWrapper_DefaultConstructor) {
  IddFileAndFactoryWrapper wrapper;
  EXPECT_TRUE(wrapper.requiredObjects() == IddFactory::instance().getRequiredObjects(IddFileType::OpenStudio));
  EXPECT_TRUE(wrapper.uniqueObjects() == IddFactory::instance().getUniqueObjects(IddFileType::OpenStudio));
  EXPECT_FALSE(wrapper.getObject(IddObjectType::UserCustom));

  wrapper.setIddFile(IddFileType::UserCustom);
  EXPECT_TRUE(wrapper.requiredObjects().empty());
  EXPECT_TRUE(wrapper.uniqueObjects().empty());

  wrapper.setIddFile(IddFactory::instance().getIddFile(IddFileType::OpenStudio));
  // Even though OpenStudio IddFile, it has been disembodied from IddFactory, so IddFileType
  // has been lost.
  EXPECT_TRUE(wrapper.iddFileType() == IddFileType(IddFileType::UserCustom));
  EXPECT_TRUE(wrapper.requiredObjects().size() == IddFactory::instance().getRequiredObjects(IddFileType::OpenStudio).size());
  EXPECT_TRUE(wrapper.uniqueObjects().size() == IddFactory::instance().getUniqueObjects(IddFileType::OpenStudio).size());

  // no change to underlying IddFile
  wrapper.setIddFile(IddFileType::UserCustom);
  EXPECT_TRUE(wrapper.requiredObjects().size() == IddFactory::instance().getRequiredObjects(IddFileType::OpenStudio).size());
  EXPECT_TRUE(wrapper.uniqueObjects().size() == IddFactory::instance().getUniqueObjects(IddFileType::OpenStudio).size());
}

TEST_F(IddFixture, IddFileAndFactoryWrapper_DirectlySetUserCustom) {
  IddFileAndFactoryWrapper wrapper(IddFileType::UserCustom);
  IddFile file = wrapper.iddFile();
  EXPECT_TRUE(file.objects().empty());
  for (int typeIndex : IddObjectType::getValues()) {
    EXPECT_FALSE(wrapper.getObject(IddObjectType(typeIndex)));
  }
}
