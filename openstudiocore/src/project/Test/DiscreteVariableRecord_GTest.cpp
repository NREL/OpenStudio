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

#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Problem_Impl.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/NullPerturbationRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/ProblemRecord_Impl.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/RubyPerturbationRecord.hpp>
#include <project/Test/ProjectFixture.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>

#include <resources.hxx>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, DiscreteVariableRecord)
{
  return;

  ProjectDatabase database = getCleanDatabase("DiscreteVariableRecord");

  openstudio::path rubyPath = resourcesPath() / openstudio::toPath("project/rubyscripts/*.rb");

  openstudio::path perturbScript = rubyPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  RubyPerturbation rubyPerturbation(perturbScript,FileReferenceType::OSM,FileReferenceType::OSM);
  std::vector<Variable> variables;
  std::string name("name");
  Problem problem(name,variables,database.runManager().loadWorkflows().at(0));
  ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);
  DiscretePerturbationVector perturbations;
  DiscreteVariable discreteVariable("Wall Construction",perturbations);
  DiscreteVariableRecord discreteVariableRecord(discreteVariable, problemRecord,0);

  EXPECT_EQ(static_cast<unsigned>(0), discreteVariableRecord.numPerturbations(true));
  EXPECT_EQ(static_cast<unsigned>(0), discreteVariableRecord.discretePerturbationRecordIds(true).size());
  EXPECT_EQ(static_cast<unsigned>(0), discreteVariableRecord.discretePerturbationRecords(true).size());
  // can't do this yet, would like to on construction eventually
  //EXPECT_EQ(static_cast<unsigned>(0), discreteVariableRecord.discoverArguments().size());
  //EXPECT_EQ(static_cast<unsigned>(0), discreteVariableRecord.arguments().size());

  // create a real perturbation
  //RubyPerturbationRecord rubyPerturbationRecord1("Test Perturbation 1", rubyPath, discreteVariableRecord);
  RubyPerturbationRecord rubyPerturbationRecord1(rubyPerturbation,discreteVariableRecord,0);
// TODO  EXPECT_EQ(discreteVariableRecord.id(), rubyPerturbationRecord1.variableRecord().id());
// TODO  EXPECT_EQ(discreteVariableRecord.handle(), rubyPerturbationRecord1.variableRecord().handle());
  EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.numPerturbations(true));
  ASSERT_EQ(static_cast<unsigned>(1), discreteVariableRecord.discretePerturbationRecordIds(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[0]);
  ASSERT_EQ(static_cast<unsigned>(1), discreteVariableRecord.discretePerturbationRecords(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.handle(), discreteVariableRecord.discretePerturbationRecords(true)[0].handle());

  // create another real perturbation
  //RubyPerturbationRecord rubyPerturbationRecord2("Test Perturbation 2", rubyPath, discreteVariableRecord);
  RubyPerturbationRecord rubyPerturbationRecord2(rubyPerturbation,discreteVariableRecord,0);
// TODO  EXPECT_EQ(discreteVariableRecord.id(), rubyPerturbationRecord2.variableRecord().id());
// TODO  EXPECT_EQ(discreteVariableRecord.handle(), rubyPerturbationRecord2.variableRecord().handle());
  EXPECT_EQ(static_cast<unsigned>(2), discreteVariableRecord.numPerturbations(true));
  ASSERT_EQ(static_cast<unsigned>(2), discreteVariableRecord.discretePerturbationRecordIds(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[0]);
  EXPECT_EQ(rubyPerturbationRecord2.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[1]);
  ASSERT_EQ(static_cast<unsigned>(2), discreteVariableRecord.discretePerturbationRecords(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.handle(), discreteVariableRecord.discretePerturbationRecords(true)[0].handle());
  EXPECT_EQ(rubyPerturbationRecord2.handle(), discreteVariableRecord.discretePerturbationRecords(true)[1].handle());

  // create a null perturbation
  UUID uuid;
  UUID versionUUID;
  NullPerturbation nullPerturbation(uuid,versionUUID,"","","",true);
  NullPerturbationRecord nullPerturbationRecord(nullPerturbation,discreteVariableRecord,0);
// TODO  EXPECT_EQ(discreteVariableRecord.id(), nullPerturbationRecord.variableRecord().id());
// TODO  EXPECT_EQ(discreteVariableRecord.handle(), nullPerturbationRecord.variableRecord().handle());
  EXPECT_EQ(static_cast<unsigned>(3), discreteVariableRecord.numPerturbations(true));
  ASSERT_EQ(static_cast<unsigned>(3), discreteVariableRecord.discretePerturbationRecordIds(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[0]);
  EXPECT_EQ(rubyPerturbationRecord2.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[1]);
  EXPECT_EQ(nullPerturbationRecord.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[2]);
  ASSERT_EQ(static_cast<unsigned>(3), discreteVariableRecord.discretePerturbationRecords(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.handle(), discreteVariableRecord.discretePerturbationRecords(true)[0].handle());
  EXPECT_EQ(rubyPerturbationRecord2.handle(), discreteVariableRecord.discretePerturbationRecords(true)[1].handle());
  EXPECT_EQ(nullPerturbationRecord.handle(), discreteVariableRecord.discretePerturbationRecords(true)[2].handle());

  EXPECT_TRUE(rubyPerturbationRecord2.setIsSelected(false));

  EXPECT_EQ(static_cast<unsigned>(2), discreteVariableRecord.numPerturbations(true));
  ASSERT_EQ(static_cast<unsigned>(2), discreteVariableRecord.discretePerturbationRecordIds(true).size());
  EXPECT_EQ(static_cast<unsigned>(3), discreteVariableRecord.numPerturbations(false));
  ASSERT_EQ(static_cast<unsigned>(3), discreteVariableRecord.discretePerturbationRecordIds(false).size());
  EXPECT_EQ(rubyPerturbationRecord1.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[0]);
  EXPECT_EQ(nullPerturbationRecord.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[1]);
  EXPECT_EQ(rubyPerturbationRecord1.id(), discreteVariableRecord.discretePerturbationRecordIds(false)[0]);
  EXPECT_EQ(rubyPerturbationRecord2.id(), discreteVariableRecord.discretePerturbationRecordIds(false)[1]);
  EXPECT_EQ(nullPerturbationRecord.id(), discreteVariableRecord.discretePerturbationRecordIds(false)[2]);
  ASSERT_EQ(static_cast<unsigned>(2), discreteVariableRecord.discretePerturbationRecords(true).size());
  EXPECT_EQ(rubyPerturbationRecord1.handle(), discreteVariableRecord.discretePerturbationRecords(true)[0].handle());
  EXPECT_EQ(nullPerturbationRecord.handle(), discreteVariableRecord.discretePerturbationRecords(true)[1].handle());
  ASSERT_EQ(static_cast<unsigned>(3), discreteVariableRecord.discretePerturbationRecords(false).size());
  EXPECT_EQ(rubyPerturbationRecord1.handle(), discreteVariableRecord.discretePerturbationRecords(false)[0].handle());
  EXPECT_EQ(rubyPerturbationRecord2.handle(), discreteVariableRecord.discretePerturbationRecords(false)[1].handle());
  EXPECT_EQ(nullPerturbationRecord.handle(), discreteVariableRecord.discretePerturbationRecords(false)[2].handle());
}
