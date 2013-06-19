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

#include <utilities/document/test/DocumentFixture.hpp>
#include <utilities/document/Section.hpp>
#include <utilities/document/Table.hpp>
#include <utilities/document/Text.hpp>

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;
using openstudio::Section;
using openstudio::Text;
using openstudio::Table;

void DocumentFixture::SetUp() {}

void DocumentFixture::TearDown() {}

void DocumentFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./DocumentFixture.log"));
  logFile->setLogLevel(Info);

  // set up doc
  doc.setTitle("Excerpt from 2009 Grocery TSD");
  doc.addAuthor("Matthew Leach");
  doc.addAuthor("Elaine Hale");
  doc.addAuthor("Adam Hirsch");
  doc.addAuthor("Paul Torcellini");
  doc.setTopHeadingLevel(1u);
  Section section = doc.appendSection("Building Economic Parameters");
  Text txt;
  std::stringstream ss;
  ss << "Our statement of work mandates that the design recommendations be analyzed for cost "
     << "effectiveness based on a five-year analysis period, which is assumed acceptable to a "
     << "majority of developers and owners. The other basic economic parameters required for the "
     << "5-TLCC calculation were taken from RSMeans and the Office of Management and Budget "
     << "(OMB) (CITATIONS).";
  txt.append(ss.str()); ss.str("");
  ss << "This analysis uses the real discount rate, which accounts for the projected rate of "
     << "general inflation found in the Report of the President's Economic Advisors, Analytical "
     << "Perpectives, and is equal to 2.3% for a five-year analysis period (CITATION). By using "
     << "this rate, we do not have to explicitly account for energy and product inflation rates.";
  txt.append(ss.str()); ss.str("");
  ss << "Regional capital cost modifiers are used to convert national averages to regional "
     << "values. These are available from the RSMeans data sets and are applied before any of the "
     << "additional fees listed in TABLE REF, three of which are also provided by RSMeans "
     << "(CITATION).";
  txt.append(ss.str()); ss.str("");
  Table tbl;
  tbl.setTitle("Economic Parameter Values");
  std::vector<std::string> row;
  row.push_back("Economic Parameter");
  row.push_back("Value");
  row.push_back("Data Source");
  tbl.appendRow(row);
  tbl.setNHead(1);
  row[0] = "Analysis Period";            row[1] = "5 Years"; row[2] = "DOE";        tbl.appendRow(row);
  row[0] = "Discount Rate";              row[1] = "2.3%";    row[2] = "OMB";        tbl.appendRow(row);
  row[0] = "O&M Cost Inflation";         row[1] = "0%";      row[2] = "OMB";        tbl.appendRow(row);
  row[0] = "Gas Cost Inflation";         row[1] = "0%";      row[2] = "OMB";        tbl.appendRow(row);
  row[0] = "Electricity Cost Inflation"; row[1] = "0%";      row[2] = "OMB";        tbl.appendRow(row);
  row[0] = "Bond Fee";                   row[1] = "10%";     row[2] = "RSMeans";    tbl.appendRow(row);
  row[0] = "Contractor Fee";             row[1] = "10%";     row[2] = "RSMeans";    tbl.appendRow(row);
  row[0] = "Contingency Fee";            row[1] = "12%";     row[2] = "RSMeans";    tbl.appendRow(row);
  row[0] = "Commissioning Fee";          row[1] = "0.5%";    row[2] = "Assumption"; tbl.appendRow(row);
  section.append(txt);
  section.append(tbl);
}

void DocumentFixture::TearDownTestCase() {
  logFile->disable();
}

openstudio::Document DocumentFixture::doc;
boost::optional<openstudio::FileLogSink> DocumentFixture::logFile;
