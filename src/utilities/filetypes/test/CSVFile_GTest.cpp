/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../CSVFile.hpp"
#include "../../data/Variant.hpp"
#include "../../data/Vector.hpp"
#include "../../time/Date.hpp"
#include "../../time/Time.hpp"
#include "../../time/DateTime.hpp"

#include <resources.hxx>

using namespace openstudio;

TEST(Filetypes, CSVFile_New) {
  CSVFile csvFile;
  EXPECT_EQ(0, csvFile.numRows());
  EXPECT_EQ(0, csvFile.numColumns());

  auto rows = csvFile.rows();
  EXPECT_EQ(0, rows.size());

  std::vector<Variant> row;

  row.push_back(Variant(1));
  csvFile.addRow(row);

  row.clear();
  row.push_back(Variant(1));
  row.push_back(Variant(2.2));
  csvFile.addRow(row);

  row.clear();
  row.push_back(Variant(1));
  row.push_back(Variant(2.2));
  row.push_back(Variant(0.33));
  csvFile.addRow(row);

  ASSERT_EQ(3, csvFile.numRows());
  ASSERT_EQ(3, csvFile.numColumns());

  rows = csvFile.rows();
  for (const auto& r : rows) {
    ASSERT_EQ(csvFile.numColumns(), r.size());
  }

  // row 1
  ASSERT_EQ(VariantType::Integer, rows[0][0].variantType().value());
  EXPECT_EQ(1, rows[0][0].valueAsInteger());

  ASSERT_EQ(VariantType::String, rows[0][1].variantType().value());
  EXPECT_EQ("", rows[0][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[0][2].variantType().value());
  EXPECT_EQ("", rows[0][2].valueAsString());

  // row 2
  ASSERT_EQ(VariantType::Integer, rows[1][0].variantType().value());
  EXPECT_EQ(1, rows[1][0].valueAsInteger());

  ASSERT_EQ(VariantType::Double, rows[1][1].variantType().value());
  EXPECT_EQ(2.2, rows[1][1].valueAsDouble());

  ASSERT_EQ(VariantType::String, rows[1][2].variantType().value());
  EXPECT_EQ("", rows[1][2].valueAsString());

  // row 3
  ASSERT_EQ(VariantType::Integer, rows[2][0].variantType().value());
  EXPECT_EQ(1, rows[2][0].valueAsInteger());

  ASSERT_EQ(VariantType::Double, rows[2][1].variantType().value());
  EXPECT_EQ(2.2, rows[1][1].valueAsDouble());

  ASSERT_EQ(VariantType::Double, rows[2][2].variantType().value());
  EXPECT_EQ(0.33, rows[2][2].valueAsDouble());

  CSVFile csvFile2(csvFile.string());
  EXPECT_EQ(3, csvFile2.numRows());
  EXPECT_EQ(3, csvFile2.numColumns());
  EXPECT_EQ(csvFile.string(), csvFile2.string());

  csvFile.clear();
  EXPECT_EQ(0, csvFile.numRows());
  EXPECT_EQ(0, csvFile.numColumns());

  row.clear();
  row.push_back(Variant(0.33));
  row.push_back(Variant(2.2));
  row.push_back(Variant(1));
  csvFile.addRow(row);

  row.clear();
  row.push_back(Variant(0.33));
  row.push_back(Variant(2.2));
  csvFile.addRow(row);

  row.clear();
  row.push_back(Variant(0.33));
  csvFile.addRow(row);

  ASSERT_EQ(3, csvFile.numRows());
  ASSERT_EQ(3, csvFile.numColumns());

  rows = csvFile.rows();
  for (const auto& r : rows) {
    ASSERT_EQ(csvFile.numColumns(), r.size());
  }
}

TEST(Filetypes, CSVFile_Load) {
  path p = resourcesPath() / toPath("utilities/Filetypes/test_csv.csv");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/TDV_2008_kBtu_CZ13.csv");
  path p3 = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
  // path p4 = resourcesPath() / toPath("utilities/Filetypes/min.osw");

  boost::optional<CSVFile> csvFile;

  // test_csv.csv
  csvFile = CSVFile::load(p);
  ASSERT_TRUE(csvFile);
  ASSERT_EQ(7u, csvFile->numRows());
  ASSERT_EQ(3u, csvFile->numColumns());
  auto rows = csvFile->rows();
  for (const auto& row : rows) {
    ASSERT_EQ(csvFile->numColumns(), row.size());
  }

  // row 1
  ASSERT_EQ(VariantType::Integer, rows[0][0].variantType().value());
  EXPECT_EQ(1, rows[0][0].valueAsInteger());

  ASSERT_EQ(VariantType::Integer, rows[0][1].variantType().value());
  EXPECT_EQ(2, rows[0][1].valueAsInteger());

  ASSERT_EQ(VariantType::Integer, rows[0][2].variantType().value());
  EXPECT_EQ(3, rows[0][2].valueAsInteger());

  // row 2
  ASSERT_EQ(VariantType::String, rows[1][0].variantType().value());
  EXPECT_EQ("", rows[1][0].valueAsString());

  ASSERT_EQ(VariantType::String, rows[1][1].variantType().value());
  EXPECT_EQ("A CSV File", rows[1][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[1][2].variantType().value());
  EXPECT_EQ("", rows[1][2].valueAsString());

  // row 3
  ASSERT_EQ(VariantType::String, rows[2][0].variantType().value());
  EXPECT_EQ("", rows[2][0].valueAsString());

  ASSERT_EQ(VariantType::String, rows[2][1].variantType().value());
  EXPECT_EQ("", rows[2][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[2][2].variantType().value());
  EXPECT_EQ("CSV, File", rows[2][2].valueAsString());

  // row 4
  ASSERT_EQ(VariantType::String, rows[3][0].variantType().value());
  EXPECT_EQ("", rows[3][0].valueAsString());

  ASSERT_EQ(VariantType::String, rows[3][1].variantType().value());
  EXPECT_EQ("", rows[3][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[3][2].variantType().value());
  EXPECT_EQ("", rows[3][2].valueAsString());

  // row 5
  ASSERT_EQ(VariantType::String, rows[4][0].variantType().value());
  EXPECT_EQ("A", rows[4][0].valueAsString());

  ASSERT_EQ(VariantType::String, rows[4][1].variantType().value());
  EXPECT_EQ("", rows[4][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[4][2].variantType().value());
  EXPECT_EQ("", rows[4][2].valueAsString());

  // row 6
  ASSERT_EQ(VariantType::String, rows[5][0].variantType().value());
  EXPECT_EQ("A", rows[5][0].valueAsString());

  ASSERT_EQ(VariantType::String, rows[5][1].variantType().value());
  EXPECT_EQ("B", rows[5][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[5][2].variantType().value());
  EXPECT_EQ("", rows[5][2].valueAsString());

  // row 7
  ASSERT_EQ(VariantType::String, rows[6][0].variantType().value());
  EXPECT_EQ("A", rows[6][0].valueAsString());

  ASSERT_EQ(VariantType::String, rows[6][1].variantType().value());
  EXPECT_EQ("B", rows[6][1].valueAsString());

  ASSERT_EQ(VariantType::String, rows[6][2].variantType().value());
  EXPECT_EQ("C", rows[6][2].valueAsString());

  // more files
  csvFile = CSVFile::load(p2);
  ASSERT_TRUE(csvFile);

  csvFile = CSVFile::load(p3);
  ASSERT_TRUE(csvFile);

  //csvFile = CSVFile::load(p4);
  //EXPECT_FALSE(csvFile);
}

TEST(Filetypes, CSVFile_AddAndGetColumn) {
  CSVFile csvFile;

  // std::vector<DateTime>
  Date startDate(Date(MonthOfYear(MonthOfYear::Jan), 1));
  DateTime startDateTime(startDate, Time(0, 1, 0, 0));
  Date endDate(Date(MonthOfYear(MonthOfYear::Jan), 1));
  DateTime endDateTime(endDate, Time(0, 3, 0, 0));
  Time delta(0, 1, 0, 0);
  std::vector<DateTime> dateTimes;
  for (openstudio::DateTime current = startDateTime; current <= endDateTime; current += delta) {
    dateTimes.push_back(current);
  }
  csvFile.addColumn(dateTimes);

  // Vector
  Vector values = linspace(1, 3, 3);
  csvFile.addColumn(values);

  // std::vector<double>
  std::vector<double> col3;
  col3.push_back(1);
  col3.push_back(2.2);
  col3.push_back(0.33);
  csvFile.addColumn(col3);

  ASSERT_EQ(3, csvFile.numRows());
  ASSERT_EQ(3, csvFile.numColumns());

  // std::vector<std::string>
  std::vector<std::string> col4;
  col4.push_back("1");
  col4.push_back("2.2");
  col4.push_back("0.33");
  csvFile.addColumn(col4);

  ASSERT_EQ(3, csvFile.numRows());
  ASSERT_EQ(4, csvFile.numColumns());

  // getColumnAsDateTimes
  std::vector<DateTime> getCol1 = csvFile.getColumnAsDateTimes(0);
  EXPECT_EQ(startDateTime, getCol1[0]);
  EXPECT_EQ(startDateTime + delta, getCol1[1]);
  EXPECT_EQ(endDateTime, getCol1[2]);

  // getColumnAsDoubleVector
  std::vector<double> getCol2 = csvFile.getColumnAsDoubleVector(1);
  EXPECT_EQ(1, getCol2[0]);
  EXPECT_EQ(2, getCol2[1]);
  EXPECT_EQ(3, getCol2[2]);

  std::vector<double> getCol3 = csvFile.getColumnAsDoubleVector(2);
  EXPECT_EQ(1, getCol3[0]);
  EXPECT_EQ(2.2, getCol3[1]);
  EXPECT_EQ(0.33, getCol3[2]);

  // getColumnAsStringVector
  std::vector<std::string> getCol4 = csvFile.getColumnAsStringVector(3);
  EXPECT_EQ("1", getCol4[0]);
  EXPECT_EQ("2.2", getCol4[1]);
  EXPECT_EQ("0.33", getCol4[2]);
}
