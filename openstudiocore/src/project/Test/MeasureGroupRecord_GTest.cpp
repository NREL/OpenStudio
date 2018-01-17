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

#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/NullMeasure.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Problem_Impl.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyMeasure_Impl.hpp"
#include "../MeasureGroupRecord.hpp"
#include "../FileReferenceRecord.hpp"
#include "../NullMeasureRecord.hpp"
#include "../ProblemRecord.hpp"
#include "../ProblemRecord_Impl.hpp"
#include "../ProjectDatabase.hpp"
#include "../RubyMeasureRecord.hpp"
#include "ProjectFixture.hpp"

#include "../../runmanager/lib/RunManager.hpp"
#include "../../runmanager/lib/Workflow.hpp"

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, MeasureGroupRecord)
{
  return;

  ProjectDatabase database = getCleanDatabase("MeasureGroupRecord");

  openstudio::path rubyPath = resourcesPath() / openstudio::toPath("project/rubyscripts/*.rb");

  openstudio::path perturbScript = rubyPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  RubyMeasure rubyMeasure(perturbScript,FileReferenceType::OSM,FileReferenceType::OSM);
  std::vector<Variable> variables;
  std::string name("name");
  Problem problem(name,variables,database.runManager().loadWorkflows().at(0));
  ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);
  MeasureVector measures;
  MeasureGroup measureGroup("Wall Construction",measures);
  MeasureGroupRecord measureGroupRecord(measureGroup, problemRecord,0);

  EXPECT_EQ(static_cast<unsigned>(0), measureGroupRecord.numMeasures(true));
  EXPECT_EQ(static_cast<unsigned>(0), measureGroupRecord.measureRecordIds(true).size());
  EXPECT_EQ(static_cast<unsigned>(0), measureGroupRecord.measureRecords(true).size());
  // can't do this yet, would like to on construction eventually
  //EXPECT_EQ(static_cast<unsigned>(0), measureGroupRecord.discoverArguments().size());
  //EXPECT_EQ(static_cast<unsigned>(0), measureGroupRecord.arguments().size());

  // create a real measure
  //RubyMeasureRecord rubyMeasureRecord1("Test Measure 1", rubyPath, measureGroupRecord);
  RubyMeasureRecord rubyMeasureRecord1(rubyMeasure,measureGroupRecord,0);
// TODO  EXPECT_EQ(measureGroupRecord.id(), rubyMeasureRecord1.variableRecord().id());
// TODO  EXPECT_EQ(measureGroupRecord.handle(), rubyMeasureRecord1.variableRecord().handle());
  EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.numMeasures(true));
  ASSERT_EQ(static_cast<unsigned>(1), measureGroupRecord.measureRecordIds(true).size());
  EXPECT_EQ(rubyMeasureRecord1.id(), measureGroupRecord.measureRecordIds(true)[0]);
  ASSERT_EQ(static_cast<unsigned>(1), measureGroupRecord.measureRecords(true).size());
  EXPECT_EQ(rubyMeasureRecord1.handle(), measureGroupRecord.measureRecords(true)[0].handle());

  // create another real measure
  //RubyMeasureRecord rubyMeasureRecord2("Test Measure 2", rubyPath, measureGroupRecord);
  RubyMeasureRecord rubyMeasureRecord2(rubyMeasure,measureGroupRecord,0);
// TODO  EXPECT_EQ(measureGroupRecord.id(), rubyMeasureRecord2.variableRecord().id());
// TODO  EXPECT_EQ(measureGroupRecord.handle(), rubyMeasureRecord2.variableRecord().handle());
  EXPECT_EQ(static_cast<unsigned>(2), measureGroupRecord.numMeasures(true));
  ASSERT_EQ(static_cast<unsigned>(2), measureGroupRecord.measureRecordIds(true).size());
  EXPECT_EQ(rubyMeasureRecord1.id(), measureGroupRecord.measureRecordIds(true)[0]);
  EXPECT_EQ(rubyMeasureRecord2.id(), measureGroupRecord.measureRecordIds(true)[1]);
  ASSERT_EQ(static_cast<unsigned>(2), measureGroupRecord.measureRecords(true).size());
  EXPECT_EQ(rubyMeasureRecord1.handle(), measureGroupRecord.measureRecords(true)[0].handle());
  EXPECT_EQ(rubyMeasureRecord2.handle(), measureGroupRecord.measureRecords(true)[1].handle());

  // create a null measure
  UUID uuid;
  UUID versionUUID;
  NullMeasure nullMeasure(uuid,versionUUID,"","","",true);
  NullMeasureRecord nullMeasureRecord(nullMeasure,measureGroupRecord,0);
// TODO  EXPECT_EQ(measureGroupRecord.id(), nullMeasureRecord.variableRecord().id());
// TODO  EXPECT_EQ(measureGroupRecord.handle(), nullMeasureRecord.variableRecord().handle());
  EXPECT_EQ(static_cast<unsigned>(3), measureGroupRecord.numMeasures(true));
  ASSERT_EQ(static_cast<unsigned>(3), measureGroupRecord.measureRecordIds(true).size());
  EXPECT_EQ(rubyMeasureRecord1.id(), measureGroupRecord.measureRecordIds(true)[0]);
  EXPECT_EQ(rubyMeasureRecord2.id(), measureGroupRecord.measureRecordIds(true)[1]);
  EXPECT_EQ(nullMeasureRecord.id(), measureGroupRecord.measureRecordIds(true)[2]);
  ASSERT_EQ(static_cast<unsigned>(3), measureGroupRecord.measureRecords(true).size());
  EXPECT_EQ(rubyMeasureRecord1.handle(), measureGroupRecord.measureRecords(true)[0].handle());
  EXPECT_EQ(rubyMeasureRecord2.handle(), measureGroupRecord.measureRecords(true)[1].handle());
  EXPECT_EQ(nullMeasureRecord.handle(), measureGroupRecord.measureRecords(true)[2].handle());

  EXPECT_TRUE(rubyMeasureRecord2.setIsSelected(false));

  EXPECT_EQ(static_cast<unsigned>(2), measureGroupRecord.numMeasures(true));
  ASSERT_EQ(static_cast<unsigned>(2), measureGroupRecord.measureRecordIds(true).size());
  EXPECT_EQ(static_cast<unsigned>(3), measureGroupRecord.numMeasures(false));
  ASSERT_EQ(static_cast<unsigned>(3), measureGroupRecord.measureRecordIds(false).size());
  EXPECT_EQ(rubyMeasureRecord1.id(), measureGroupRecord.measureRecordIds(true)[0]);
  EXPECT_EQ(nullMeasureRecord.id(), measureGroupRecord.measureRecordIds(true)[1]);
  EXPECT_EQ(rubyMeasureRecord1.id(), measureGroupRecord.measureRecordIds(false)[0]);
  EXPECT_EQ(rubyMeasureRecord2.id(), measureGroupRecord.measureRecordIds(false)[1]);
  EXPECT_EQ(nullMeasureRecord.id(), measureGroupRecord.measureRecordIds(false)[2]);
  ASSERT_EQ(static_cast<unsigned>(2), measureGroupRecord.measureRecords(true).size());
  EXPECT_EQ(rubyMeasureRecord1.handle(), measureGroupRecord.measureRecords(true)[0].handle());
  EXPECT_EQ(nullMeasureRecord.handle(), measureGroupRecord.measureRecords(true)[1].handle());
  ASSERT_EQ(static_cast<unsigned>(3), measureGroupRecord.measureRecords(false).size());
  EXPECT_EQ(rubyMeasureRecord1.handle(), measureGroupRecord.measureRecords(false)[0].handle());
  EXPECT_EQ(rubyMeasureRecord2.handle(), measureGroupRecord.measureRecords(false)[1].handle());
  EXPECT_EQ(nullMeasureRecord.handle(), measureGroupRecord.measureRecords(false)[2].handle());
}
