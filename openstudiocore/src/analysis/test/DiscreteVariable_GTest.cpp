/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
