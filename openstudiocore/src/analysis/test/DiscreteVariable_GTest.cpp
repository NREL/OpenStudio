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
#include "AnalysisFixture.hpp"

#include "../MeasureGroup.hpp"
#include "../Measure.hpp"
#include "../NullMeasure.hpp"
#include "../RubyMeasure.hpp"
#include "../UncertaintyDescription.hpp"

#include "../../utilities/core/FileReference.hpp"

#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, MeasureGroup_Constructors) {
  MeasureVector measures;

  // At most one null measure is allowed.
  measures.push_back(NullMeasure());
  measures.push_back(NullMeasure());
  measures.push_back(NullMeasure());
  measures.push_back(NullMeasure());
  MeasureGroup variable("Variable",measures);
  EXPECT_EQ(1u,variable.numMeasures(false));

  // deserialization constructor
  UUID uuid = createUUID();
  UUID versionUUID = createUUID();
  measures = variable.measures(false);
  variable = MeasureGroup(uuid,versionUUID,"Variable","","",boost::none,measures);
  EXPECT_EQ("Variable",variable.name());
  EXPECT_TRUE(variable.uuid() == uuid);
  EXPECT_TRUE(variable.versionUUID() == versionUUID);
  EXPECT_TRUE(variable.measures(false) == measures);

  // Inconsistent file types in measures (should throw)
  measures.clear();
  openstudio::path rubyScriptPath = toPath(rubyLibDir()) / 
                                    toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::IDF));
  EXPECT_THROW(MeasureGroup("Variable 2",measures),std::exception);

  // Inconsistent file types in measures (should throw)
  measures.clear();
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::IDF,
                                           FileReferenceType::IDF));
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  EXPECT_THROW(MeasureGroup("Variable",measures),std::exception);

}

TEST_F(AnalysisFixture, MeasureGroup_DeselectMeasures) {
  MeasureVector measures;

  // null, ruby, ruby
  measures.push_back(NullMeasure());
  openstudio::path rubyScriptPath = toPath(rubyLibDir()) / 
                                    toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  measures.push_back(RubyMeasure(rubyScriptPath,
                                 FileReferenceType::OSM,
                                 FileReferenceType::OSM));
  measures.push_back(RubyMeasure(rubyScriptPath,
                                 FileReferenceType::OSM,
                                 FileReferenceType::OSM));

  MeasureGroup variable("Variable",measures);
  EXPECT_EQ(3u,variable.numMeasures(true));
  EXPECT_EQ(3u,variable.numMeasures(false));

  measures[1].setIsSelected(false);
  EXPECT_EQ(2u,variable.numMeasures(true));
  ASSERT_EQ(2u,variable.measures(true).size());
  EXPECT_TRUE(variable.measures(true)[1] == measures[2]);
  EXPECT_EQ(3u,variable.numMeasures(false));

  measures[0].setIsSelected(false);
  measures[2].setIsSelected(false);
  EXPECT_EQ(0u,variable.numMeasures(true));
  EXPECT_EQ(3u,variable.numMeasures(false));
}
