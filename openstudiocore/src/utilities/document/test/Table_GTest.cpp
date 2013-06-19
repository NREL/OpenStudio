/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/document/Table.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/QuantityFactory.hpp>
#include <utilities/units/QuantityConverter.hpp>

#include <resources.hxx>

#include <boost/filesystem/fstream.hpp>

#include <iomanip>

using openstudio::Table;
using openstudio::OptionalTable;
using openstudio::TableFormat;

using openstudio::OptionalString;
using openstudio::StringVector;
using openstudio::toPath;

using openstudio::extractUnitString;
using openstudio::convertToStandardForm;
using openstudio::containsRegisteredBaseUnit;
using openstudio::OptionalInt;
using openstudio::Quantity;
using openstudio::createQuantity;

TEST(Table, ConstructFromCSV)
{
  openstudio::path p = resourcesPath()/toPath("utilities/Table/HeightWeight.csv");
  Table table = Table::load(p);
  table.setTitle("Height and Weight");
  table.setNHead(1);
  table.setNLeft(1);
  ASSERT_EQ(static_cast<unsigned>(6),table.nRows());
  ASSERT_EQ(static_cast<unsigned>(3),table.nCols());
  std::stringstream ss;
  ss << table[3][1];
  EXPECT_EQ("4'11\"",ss.str()); ss.str("");
  ss << table[5][2];
  EXPECT_EQ("95",ss.str()); ss.str("");

  p = resourcesPath()/toPath("utilities/Table/EUI.csv");
  table = Table::load(p);
  table.setTitle("EUIs");
  table.setCaption("EUIs of several buildings");
  table.setNHead(2);
  table.setNLeft(2);
  ASSERT_EQ(static_cast<unsigned>(10),table.nRows());
  ASSERT_EQ(static_cast<unsigned>(6),table.nCols());
  ss << std::fixed << std::setprecision(0) << table[2][2];
  EXPECT_EQ("120",ss.str()); ss.str("");

  p = resourcesPath()/toPath("utilities/Table/OddlyShaped.csv");
  table = Table::load(p);
  table.setCaption("An Oddly Shaped Table");
  table.setNHead(1);
  table.setNLeft(1);
  ASSERT_EQ(static_cast<unsigned>(6),table.nRows());
  ASSERT_EQ(static_cast<unsigned>(6),table.nCols());
  ss << table[2][0];
  EXPECT_EQ("Row 2",ss.str()); ss.str("");
}

// ETH@06092010 Test added to help with gcc debug.
TEST(Table, Trim) {
  Table t;
  StringVector row;

  // create table that can be trimmed
  t.appendRow(row);
  t.appendRow(row);
  row.push_back("real row 1"); row.push_back("header"); row.push_back("");
  t.appendRow(row); row.clear();
  row.push_back("real row 2"); row.push_back("2");
  t.appendRow(row); row.clear();
  t.appendRow(row);
  EXPECT_EQ(static_cast<unsigned>(5),t.nRows());
  EXPECT_EQ(static_cast<unsigned>(3),t.nCols());

  // trim and test
  t.trim();
  EXPECT_EQ(static_cast<unsigned>(2),t.nRows());
  EXPECT_EQ(static_cast<unsigned>(2),t.nCols());
}

TEST(Table, Serialization)
{
  // construct table
  openstudio::path p = resourcesPath()/toPath("utilities/Table/EUI.csv");
  Table table = Table::load(p);
  table.setTitle("EUIs");
  table.setCaption("EUIs of several buildings");
  table.setNHead(2);
  table.setNLeft(2);
  p = resourcesPath()/toPath("utilities/Table/EUI.ost");

  // serialize
  openstudio::Time start = openstudio::Time::currentTime();
  table.save(p);
  openstudio::Time totalTime = openstudio::Time::currentTime() - start;
  LOG_FREE(Info, "openstudio.Table", "Time to serialize a small table = " << totalTime);

  // ETH@20100526 Uncommenting this line results in a compiler error that I haven't been
  // able to debug.
  // OptionalTable ot;

  // deserialize
  start = openstudio::Time::currentTime();
  Table loaded = Table::load(p);
  totalTime = openstudio::Time::currentTime() - start;
  LOG_FREE(Info, "openstudio.Table", "Time to deserialize a small table = " << totalTime);
  ASSERT_EQ(static_cast<unsigned>(10),loaded.nRows());
  ASSERT_EQ(static_cast<unsigned>(6),loaded.nCols());
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << table[2][2];
  EXPECT_EQ("120",ss.str()); ss.str("");
}

TEST(Table, UnitExtraction) {

  openstudio::path p = resourcesPath()/toPath("utilities/Table/HeightWeight.csv");
  Table table = Table::load(p);
  table.setTitle("Height and Weight");
  table.setNHead(1);
  table.setNLeft(1);
  std::string unitStr;
  unitStr = table.units(0);
  EXPECT_TRUE(unitStr.empty());
  unitStr = table.units(1);
  EXPECT_TRUE(unitStr.empty());
  unitStr = table.units(2);
  EXPECT_EQ("lb_m",unitStr);

  p = resourcesPath()/toPath("utilities/Table/EUI.csv");
  table = Table::load(p);
  table.setTitle("EUIs");
  table.setCaption("EUIs of several buildings");
  table.setNHead(2);
  table.setNLeft(2);
  unitStr = table.units(0);
  EXPECT_TRUE(unitStr.empty());
  unitStr = table.units(1);
  EXPECT_TRUE(unitStr.empty());
  unitStr = table.units(2);
  EXPECT_EQ("kBtu/ft^2",unitStr);
  unitStr = table.units(3);
  EXPECT_EQ("MJ/m^2",unitStr);
  unitStr = table.units(4);
  EXPECT_EQ("kBtu/ft^2",unitStr);
  unitStr = table.units(5);
  EXPECT_EQ("MJ/m^2",unitStr);
}

TEST(Table, UnitConversion) {

  openstudio::path p = resourcesPath()/toPath("utilities/Table/HeightWeight.csv");
  Table table = Table::load(p);
  table.setTitle("Height and Weight");
  table.setNHead(1);
  table.setNLeft(1);
  EXPECT_FALSE(table.convertUnits(0));
  EXPECT_FALSE(table.convertUnits(1));

  // expanded test to help debug on gcc
  ASSERT_TRUE(table.nRows() >= 2);
  ASSERT_TRUE(table.nCols() >= 3);
  EXPECT_EQ("lb_m",table.units(2));
  std::stringstream ss;
  ss << table[0][2];
  std::string tmp = extractUnitString(ss.str());
  EXPECT_EQ("lb_m",tmp);
  tmp = convertToStandardForm(tmp);
  EXPECT_EQ("lb_m",tmp);
  EXPECT_TRUE(containsRegisteredBaseUnit(tmp));

  ASSERT_TRUE(table[1][2].isInt());
  double d = table[1][2].toDouble();
  Quantity q = createQuantity(d,tmp).get();

  EXPECT_TRUE(table.convertUnits(2));
  std::string unitStr = table.units(2);
  EXPECT_EQ("kg",unitStr);
  p = resourcesPath()/toPath("utilities/Table/HeightWeightSI.csv");
  boost::filesystem::ofstream outFile(p);
  ASSERT_TRUE(outFile);
  table.printToStream(outFile,TableFormat(TableFormat::CSV));
  outFile.close();

  p = resourcesPath()/toPath("utilities/Table/EUI.csv");
  table = Table::load(p);
  table.setTitle("EUIs");
  table.setCaption("EUIs of several buildings");
  table.setNHead(2);
  table.setNLeft(2);
  EXPECT_TRUE(table.convertUnits(2));
  unitStr = table.units(2);
  EXPECT_EQ("kJ/m^2",unitStr);
  // now cols 2 and 3 should be equal except for kJ/m^2 v. MJ/m^2.
  for (unsigned i = 0, n = table.nRows(); i < n; ++i) {
    if (table.isRowIndex(i,Table::BODY)) {
      double d2 = table[i][2].toDouble();
      double d3 = table[i][3].toDouble();
      EXPECT_NEAR((d2-(d3*1000.0))/(d2),0.0,1.0E-3);
    }
  }
  p = resourcesPath()/toPath("utilities/Table/EUISI.csv");
  outFile.open(p);
  ASSERT_TRUE(outFile);
  table.printToStream(outFile,TableFormat(TableFormat::CSV));
  outFile.close();

}
