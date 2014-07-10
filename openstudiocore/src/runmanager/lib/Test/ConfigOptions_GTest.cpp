/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include "../ConfigOptions.hpp"
#include <QCoreApplication>

#ifdef _WIN32
std::ostream &operator<<(std::ostream &os, const openstudio::path &ver)
{
  os << openstudio::toString(ver);
  return os;
}
#endif


std::ostream &operator<<(std::ostream &os, const std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> &ver)
{
  os << "(";

  if (ver.first.getMajor())
  {
    os << ver.first.getMajor().get();
  } else {
    os << "-";
  }

  os << ".";

  if (ver.first.getMinor())
  {
    os << ver.first.getMinor().get();
  } else {
    os << "-";
  }

  os << ".";

  if (ver.first.getBuild())
  {
    os << ver.first.getBuild().get();
  } else {
    os << "-";
  }

  os << ", " << ver.second << ")";

  return os;
}


TEST_F(RunManagerTestFixture, ConfigOptionsTest)
{
  using namespace openstudio;
  using namespace openstudio::runmanager;

  ConfigOptions co;

  co.reset();

  ToolVersion e1(5,4,2);
  ToolVersion e2(3,2,1);
  ToolVersion e3(3,1,2);
  ToolVersion e4(2,7);
  ToolVersion e5(2,7,1);
  ToolVersion e6(2);

  ToolLocationInfo p1 = ToolLocationInfo(ToolType("EnergyPlus"), toPath("e1"), openstudio::path());
  ToolLocationInfo p2 = ToolLocationInfo(ToolType("EnergyPlus"), toPath("e2"), openstudio::path());
  ToolLocationInfo p3 = ToolLocationInfo(ToolType("EnergyPlus"), toPath("e3"), openstudio::path());
  ToolLocationInfo p4 = ToolLocationInfo(ToolType("EnergyPlus"), toPath("e4"), openstudio::path());
  ToolLocationInfo p5 = ToolLocationInfo(ToolType("EnergyPlus"), toPath("e5"), openstudio::path());
  ToolLocationInfo p6 = ToolLocationInfo(ToolType("EnergyPlus"), toPath("e6"), openstudio::path());

  co.setToolLocation(e6, p6);
  co.setToolLocation(e5, p5);
  co.setToolLocation(e3, p3);
  co.setToolLocation(e1, p1);
  co.setToolLocation(e4, p4);
  co.setToolLocation(e2, p2);


  // getEnergyPlus returns the most completely specified version of E+ that matches the passed
  // in requirements

  EXPECT_EQ(static_cast<size_t>(6), co.getToolLocations().size());

  Tools tools = co.getTools();

  EXPECT_EQ(tools.getTool("energyplus").version, e1);
  EXPECT_EQ(tools.getTool("energyplus", e3).version, e3);
  EXPECT_EQ(tools.getTool("energyplus", ToolVersion(3)).version, e2);
  EXPECT_EQ(tools.getTool("energyplus", ToolVersion(3, 2)).version, e2);
  EXPECT_EQ(tools.getTool("energyplus", ToolVersion(3, 1)).version, e3);
  EXPECT_EQ(tools.getTool("energyplus", ToolVersion(2)).version, e5);

  co.reset();

  EXPECT_EQ(co.getToolLocations().size(), static_cast<size_t>(0));
}


TEST_F(RunManagerTestFixture, ConfigOptionsQSettingsTest)
{
  using namespace openstudio;
  using namespace openstudio::runmanager;
  QCoreApplication::setOrganizationName("TestOrg");
  QCoreApplication::setApplicationName("TestApplication");

  ConfigOptions co;
  co.setDefaultIDFLocation(openstudio::toPath("a"));
  co.setDefaultEPWLocation(openstudio::toPath("b"));
  co.setOutputLocation(openstudio::toPath("c"));
  co.setMaxLocalJobs(10);
  co.setSLURMUserName("d");
  co.setSLURMHost("e");
  co.setMaxSLURMJobs(11);
  co.setSimpleName(true);
  co.setSLURMMaxTime(12);
  co.setSLURMPartition("f");
  co.setSLURMAccount("g");
  co.saveQSettings();

  ConfigOptions co2(true);

  co.setDefaultIDFLocation(openstudio::toPath("a"));
  co.setDefaultEPWLocation(openstudio::toPath("b"));
  co.setOutputLocation(openstudio::toPath("c"));
  co.setMaxLocalJobs(10);
  co.setSLURMUserName("d");
  co.setSLURMHost("e");
  co.setMaxSLURMJobs(11);
  co.setSimpleName(true);
  co.setSLURMMaxTime(12);
  co.setSLURMPartition("f");
  co.setSLURMAccount("g");
  co.saveQSettings();

  EXPECT_EQ(co.getDefaultIDFLocation(), co2.getDefaultIDFLocation());
  EXPECT_EQ(co.getDefaultEPWLocation(), co2.getDefaultEPWLocation());
  EXPECT_EQ(co.getOutputLocation(), co2.getOutputLocation());
  EXPECT_EQ(co.getMaxLocalJobs(), co2.getMaxLocalJobs());
  EXPECT_EQ(co.getSLURMUserName(), co2.getSLURMUserName());
  EXPECT_EQ(co.getSLURMHost(), co2.getSLURMHost());
  EXPECT_EQ(co.getMaxSLURMJobs(), co2.getMaxSLURMJobs());
  EXPECT_EQ(co.getSimpleName(), co2.getSimpleName());
  EXPECT_EQ(co.getSLURMMaxTime(), co2.getSLURMMaxTime());
  EXPECT_EQ(co.getSLURMPartition(), co2.getSLURMPartition());
  EXPECT_EQ(co.getSLURMAccount(), co2.getSLURMAccount());
}


