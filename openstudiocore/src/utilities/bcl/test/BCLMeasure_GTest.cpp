/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "BCLFixture.hpp"
#include "../../core/PathHelpers.hpp"
#include <resources.hxx>

#include "../BCLFileReference.hpp"
#include "../BCLMeasure.hpp"

#include <QFile>

using namespace openstudio;

TEST_F(BCLFixture, BCLMeasure)
{
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/");
  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);

  EXPECT_EQ("Set Window to Wall Ratio by Facade", measure->name());
  EXPECT_EQ("f347ae80-48b4-4c40-bfd4-6c5139b38136", measure->uid());
  EXPECT_EQ("a0e33012-0183-45a0-8898-11f93f55164e", measure->versionId());
  EXPECT_EQ("This measure will set the window to wall ratio for exterior surfaces with a specified orientation. If one or more windows exist on an affected wall, they will be removed and replaced with a single ribbon window. Doors will not be removed. If the requested ratio can't be achieved then the wall will remain untouched.", measure->description());
  EXPECT_EQ("This measure identifies exterior surfaces of the proper orientation. Then it runs a method that removes existing windows and applies a new window with a specified window to wall ratio and sill height. The construction chosen for the new window is defaulted to what is assigned to the space, or inherited from a higher level object, such as the building. If the baseline model uses hard assigned constructions you may not get the expected results. The measure doesn't have any cost or lifecycle arguments, however if lifecycle objects exist for exterior wall and window constructions, then this measure will be able to calculate the economic impact of change in window to wall ratio.", measure->modelerDescription());

  EXPECT_EQ(MeasureType::ModelMeasure, measure->measureType().value());
  EXPECT_TRUE(measure->primaryRubyScriptPath());
  EXPECT_EQ("Envelope.Fenestration", measure->taxonomyTag());

  EXPECT_EQ(6u, measure->files().size());
  for (BCLFileReference file : measure->files()) {
    EXPECT_TRUE(exists(file.path()));
    EXPECT_FALSE(file.checkForUpdate());
  }

  openstudio::path dir2 = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade2/");
  if (QFile::exists(toQString(dir2))){
    ASSERT_TRUE(removeDirectory(dir2));
  }
  // If this assertion fails, check that you don't have an Explorer window opened to the SetWindowToWallRatioByFacade2 directory
  ASSERT_FALSE(QFile::exists(toQString(dir2)));

  boost::optional<BCLMeasure> measure2 = measure->clone(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());
  EXPECT_TRUE(*measure == *measure2);
  EXPECT_FALSE(measure->directory() == measure2->directory());
  EXPECT_TRUE(dir2 == measure2->directory());
  EXPECT_EQ(6u, measure2->files().size());
  
  measure2->setName("New Measure"); // this would normally be initiated by a change from the measure
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML()); // name change does not trigger xml update
  EXPECT_FALSE(*measure == *measure2);
  measure2->save();
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());

  measure2 = BCLMeasure::load(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());
  EXPECT_FALSE(*measure == *measure2);
  EXPECT_EQ("New Measure", measure2->name());
  ASSERT_TRUE(measure2->primaryRubyScriptPath());
  EXPECT_EQ(6u, measure2->files().size());

  QFile file(toQString(measure2->primaryRubyScriptPath().get()));
  bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
  ASSERT_TRUE(opened);
  QTextStream textStream(&file);
  textStream << "Hi";
  file.close();
  EXPECT_FALSE(measure2->checkForUpdatesXML());
  EXPECT_TRUE(measure2->checkForUpdatesFiles());

  measure2.reset();
  ASSERT_TRUE(exists(dir2));
  ASSERT_TRUE(removeDirectory(dir2));
  ASSERT_FALSE(exists(dir2));

  std::string className = BCLMeasure::makeClassName("Another Measure");
  EXPECT_EQ("AnotherMeasure", className);

  EXPECT_NO_THROW( measure2 = BCLMeasure("Another Measure", className, dir2, "Envelope.Fenestration", 
    MeasureType::ReportingMeasure, "Description", "Modeler Description") );
  ASSERT_TRUE(measure2);
  ASSERT_TRUE(exists(dir2));
  EXPECT_EQ("another_measure", measure2->name());
  EXPECT_EQ("Another Measure", measure2->displayName());
  EXPECT_EQ("AnotherMeasure", measure2->className());
  EXPECT_TRUE(measure2->primaryRubyScriptPath());

  measure2 = BCLMeasure::load(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_TRUE(measure2->checkForUpdatesXML());
  ASSERT_TRUE(measure2->primaryRubyScriptPath());
}

TEST_F(BCLFixture, BCLMeasure_CTor)
{
  openstudio::path dir = boost::filesystem::system_complete(toPath("./TestMeasure/"));
  if(exists(dir)){
    removeDirectory(dir);
  }
  ASSERT_FALSE(exists(dir));

  std::string className = BCLMeasure::makeClassName(" _ $ Test & Measure");
  EXPECT_EQ("ATestMeasure", className);

  try{
    BCLMeasure measure("Test Measure", className, dir, "Envelope.Fenestration", 
                       MeasureType::ModelMeasure, "Description", "Modeler Description");
  }catch(std::exception&){
    ASSERT_TRUE(false);
  }
  ASSERT_TRUE(exists(dir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);
}

TEST_F(BCLFixture, PatApplicationMeasures)
{
  std::vector<BCLMeasure> patApplicationMeasures = BCLMeasure::patApplicationMeasures();
  ASSERT_EQ(5u, patApplicationMeasures.size());

  bool filesUpdated;
  bool xmlUpdated;

  BCLMeasure alternativeModelMeasure = BCLMeasure::alternativeModelMeasure();
  ASSERT_TRUE(alternativeModelMeasure.primaryRubyScriptPath());
  filesUpdated = alternativeModelMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = alternativeModelMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    alternativeModelMeasure.save();
  }

  BCLMeasure reportRequestMeasure = BCLMeasure::reportRequestMeasure();
  ASSERT_TRUE(reportRequestMeasure.primaryRubyScriptPath());
  filesUpdated = reportRequestMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = reportRequestMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    reportRequestMeasure.save();
  }

  BCLMeasure standardReportMeasure = BCLMeasure::standardReportMeasure();
  ASSERT_TRUE(standardReportMeasure.primaryRubyScriptPath());
  filesUpdated = standardReportMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = standardReportMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    standardReportMeasure.save();
  }

  BCLMeasure calibrationReportMeasure = BCLMeasure::calibrationReportMeasure();
  ASSERT_TRUE(calibrationReportMeasure.primaryRubyScriptPath());
  filesUpdated = calibrationReportMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = calibrationReportMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    calibrationReportMeasure.save();
  }

  BCLMeasure radianceMeasure = BCLMeasure::radianceMeasure();
  ASSERT_TRUE(radianceMeasure.primaryRubyScriptPath());
  filesUpdated = radianceMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = radianceMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    radianceMeasure.save();
  }

}
