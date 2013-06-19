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
#include <project/DiscreteVariableRecord_Impl.hpp>
#include <project/NullPerturbationRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/RubyPerturbationRecord.hpp>
#include <project/Test/ProjectFixture.hpp>

#include <runmanager/lib/Workflow.hpp>

#include <utilities/core/Checksum.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Path.hpp>

#include <resources.hxx>

#include <OpenStudio.hxx>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::runmanager;

TEST_F(ProjectFixture, ProblemRecord)
{
  //Logger::instance().logLevel(Debug);

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  // Workflow
  Workflow workflow;

  // Variables
  VariableVector variables;

  // Perturbations
  DiscretePerturbationVector perturbations1;
  DiscretePerturbationVector perturbations2;

  // Discrete Variable Records
  InputVariableRecordVector discreteVariableRecords;

  {
    // Problem
    Problem problem("0 Variables",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    InputVariableRecordVector discreteVariableRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(0u,discreteVariableRecords.size());
  }

  {
    perturbations1.push_back(NullPerturbation());

    variables.push_back(analysis::DiscreteVariable("",perturbations1));

    // Problem
    Problem problem("perturbations1",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    discreteVariableRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(1u,discreteVariableRecords.size());

    // Discrete Variable Record
    DiscreteVariableRecord discreteVariableRecord = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();

    EXPECT_EQ(1u,discreteVariableRecord.discretePerturbationRecordIds(true).size());
    EXPECT_EQ(1u,discreteVariableRecord.discretePerturbationRecords(true).size());
  }

  {
    variables.push_back(DiscreteVariable("Wall Construction",perturbations1));

    // Problem
    Problem problem("perturbations1",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    discreteVariableRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(2u,discreteVariableRecords.size());

    // Discrete Variable Record
    DiscreteVariableRecord variable1 = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();

    EXPECT_EQ(static_cast<unsigned>(1), variable1.discretePerturbationRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(1), variable1.discretePerturbationRecords(false).size());
    EXPECT_EQ(static_cast<unsigned>(2), problemRecord.inputVariableRecords().size());
  }

  {
    perturbations2.push_back(NullPerturbation());

    variables.push_back(DiscreteVariable("Roof Construction",perturbations2));

    // Problem
    Problem problem("perturbations2",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    discreteVariableRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(3u,discreteVariableRecords.size());

    // Discrete Variable Record
    DiscreteVariableRecord variable1 = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();
    DiscreteVariableRecord variable2 = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();
    DiscreteVariableRecord variable3 = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();

    EXPECT_EQ(static_cast<size_t>(1), variable1.discretePerturbationRecords(true).size());
    EXPECT_EQ(static_cast<size_t>(1), variable1.discretePerturbationRecords(false).size());
    EXPECT_EQ(static_cast<size_t>(3), problemRecord.inputVariableRecords().size());

    UUID uuid1;
    UUID versionUUID1;
    NullPerturbation nullPerturbation1(uuid1,versionUUID1,"","","",true);
    NullPerturbationRecord nullPerturbationRecord1(nullPerturbation1,variable1,0);

    EXPECT_EQ(static_cast<unsigned>(2), variable1.discretePerturbationRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable1.discretePerturbationRecords(false).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.discretePerturbationRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.discretePerturbationRecords(false).size());

    UUID uuid2;
    UUID versionUUID2;
    NullPerturbation nullPerturbation2(uuid2,versionUUID2,"","","",true);
    NullPerturbationRecord nullPerturbationRecord2(nullPerturbation2,variable1,0);

    EXPECT_EQ(static_cast<unsigned>(2), variable1.discretePerturbationRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable1.discretePerturbationRecords(false).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.discretePerturbationRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.discretePerturbationRecords(false).size());

    DiscretePerturbationVector perturbations;
    DiscreteVariable discreteVariable("Wall Construction",perturbations);
    DiscreteVariableRecord discreteVariableRecord(discreteVariable, problemRecord,0);

    openstudio::path rubyPath = resourcesPath() / openstudio::toPath("project/rubyscripts/*.rb");
    openstudio::path perturbScript = rubyPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
    RubyPerturbation rubyPerturbation(perturbScript,FileReferenceType::OSM,FileReferenceType::OSM);
    RubyPerturbationRecord rubyPerturbationRecord1(rubyPerturbation,discreteVariableRecord,0);

    EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.numPerturbations(true));
    EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.discretePerturbationRecordIds(true).size());
    EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.discretePerturbationRecords(true).size());

    RubyPerturbationRecord rubyPerturbationRecord(rubyPerturbation,discreteVariableRecord,0);

    EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.numPerturbations(true));
    EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.discretePerturbationRecordIds(true).size());
    EXPECT_EQ(rubyPerturbationRecord.id(), discreteVariableRecord.discretePerturbationRecordIds(true)[0]);
    EXPECT_EQ(static_cast<unsigned>(1), discreteVariableRecord.discretePerturbationRecords(true).size());
    EXPECT_EQ(rubyPerturbationRecord.handle(), discreteVariableRecord.discretePerturbationRecords(true)[0].handle());
  }
}
