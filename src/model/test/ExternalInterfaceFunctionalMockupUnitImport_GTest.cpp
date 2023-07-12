/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImport) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");
  EXPECT_EQ("test name", eifmui.fMUFileName());
  EXPECT_TRUE(eifmui.isFMUTimeoutDefaulted());
  EXPECT_EQ(0.0, eifmui.fMUTimeout());
  EXPECT_TRUE(eifmui.isFMULoggingOnDefaulted());
  EXPECT_EQ(0, eifmui.fMULoggingOn());
  eifmui.setFMUTimeout(100);
  EXPECT_EQ(100.0, eifmui.fMUTimeout());
  eifmui.resetFMUTimeout();
  EXPECT_TRUE(eifmui.isFMUTimeoutDefaulted());
  eifmui.setFMULoggingOn(1);
  EXPECT_EQ(1, eifmui.fMULoggingOn());
  eifmui.setFMULoggingOn(2);
  EXPECT_EQ(2, eifmui.fMULoggingOn());
  eifmui.resetFMULoggingOn();
  EXPECT_TRUE(eifmui.isFMUTimeoutDefaulted());
  EXPECT_TRUE(eifmui.setFMUFileName("Test Name"));
  EXPECT_NE("test name", eifmui.fMUFileName());
  EXPECT_EQ("Test Name", eifmui.fMUFileName());
}

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImport2) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "c:\\Program Files\\Test\\blah.fmu");
  EXPECT_EQ("c:\\Program Files\\Test\\blah.fmu", eifmui.fMUFileName());
  EXPECT_EQ("External Interface Functional Mockup Unit Import 1", eifmui.nameString());
  EXPECT_TRUE(eifmui.setFMUFileName("c:/Program Files/Test/blah.fmu"));
  EXPECT_EQ("c:/Program Files/Test/blah.fmu", eifmui.fMUFileName());
}
