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
#include <utilities/bcl/test/BCLFixture.hpp>
#include <resources.hxx>

#include <utilities/bcl/BCLFileReference.hpp>
#include <utilities/bcl/BCLMeasure.hpp>

#include <QFile>

using namespace openstudio;

TEST_F(BCLFixture, BCLMeasure)
{
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade/");
  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);

  EXPECT_EQ("Set Window to Wall Ratio by Facade", measure->name());
  EXPECT_EQ("f347ae80-48b4-4c40-bfd4-6c5139b38136", measure->uid());
  EXPECT_EQ("Description should be asked for upon creation.", measure->description());
  EXPECT_EQ("Modeler description needs to be hand-edited.", measure->modelerDescription());

  EXPECT_EQ(MeasureType::ModelMeasure, measure->measureType().value());
  EXPECT_FALSE(measure->usesSketchUpAPI());
  EXPECT_TRUE(measure->primaryRubyScriptPath());
  EXPECT_EQ("Envelope", measure->taxonomyTag());

  EXPECT_EQ(4u, measure->files().size());
  Q_FOREACH(BCLFileReference file, measure->files()){
    EXPECT_TRUE(exists(file.path()));
    EXPECT_FALSE(file.checkForUpdate());
  }

  openstudio::path dir2 = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade2/");
  if (exists(dir2)){
    boost::filesystem::remove_all(dir2);
  }
  ASSERT_FALSE(exists(dir2));

  boost::optional<BCLMeasure> measure2 = measure->clone(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdates());
  EXPECT_TRUE(*measure == *measure2);
  EXPECT_FALSE(measure->directory() == measure2->directory());
  EXPECT_TRUE(dir2 == measure2->directory());
  EXPECT_EQ(4u, measure2->files().size());
  
  measure2->setName("New Measure");
  EXPECT_FALSE(measure2->checkForUpdates());
  EXPECT_FALSE(*measure == *measure2);
  measure2->save();
  EXPECT_FALSE(measure2->checkForUpdates());

  measure2 = BCLMeasure::load(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdates());
  EXPECT_FALSE(*measure == *measure2);
  EXPECT_EQ("New Measure", measure2->name());
  ASSERT_TRUE(measure2->primaryRubyScriptPath());
  EXPECT_EQ(4u, measure2->files().size());

  QFile file(toQString(measure2->primaryRubyScriptPath().get()));
  bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
  ASSERT_TRUE(opened);
  QTextStream textStream(&file);
  textStream << "Hi";
  file.close();
  EXPECT_TRUE(measure2->checkForUpdates());

  measure2.reset();
  ASSERT_TRUE(exists(dir2));
  boost::filesystem::remove_all(dir2);
  ASSERT_FALSE(exists(dir2));

  std::string className = BCLMeasure::className("Another Measure");
  EXPECT_EQ("AnotherMeasure", className);

  EXPECT_NO_THROW( measure2 = BCLMeasure("Another Measure", className, dir2, "Envelope", 
                                          MeasureType::ModelMeasure, true) );
  ASSERT_TRUE(measure2);
  ASSERT_TRUE(exists(dir2));
  EXPECT_EQ("Another Measure", measure2->name());
  EXPECT_TRUE(measure2->primaryRubyScriptPath());

  measure2 = BCLMeasure::load(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdates());
  ASSERT_TRUE(measure2->primaryRubyScriptPath());
}

TEST_F(BCLFixture, BCLMeasure_CTor)
{
  openstudio::path dir = boost::filesystem::system_complete(toPath("./TestMeasure/"));
  if(exists(dir)){
    boost::filesystem::remove_all(dir);
  }
  ASSERT_FALSE(exists(dir));

  std::string className = BCLMeasure::className(" _ $ Test & Measure");
  EXPECT_EQ("ATestMeasure", className);

  try{
    BCLMeasure measure("Test Measure", className, dir, "Envelope.Fenestration", 
                       MeasureType::ModelMeasure, false);
  }catch(std::exception&){
    ASSERT_TRUE(false);
  }
  ASSERT_TRUE(exists(dir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);
}
