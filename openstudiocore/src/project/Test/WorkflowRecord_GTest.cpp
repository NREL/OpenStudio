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
#include "ProjectFixture.hpp"

#include "../ProjectDatabase.hpp"
#include "../WorkflowRecord.hpp"
#include "../ProblemRecord.hpp"

#include "../../analysis/Problem.hpp"
#include "../../analysis/WorkflowStep.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/core/System.hpp"

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, WorkflowRecord_Base) {
  ProjectDatabase database = getCleanDatabase("WorkflowRecord_Base");
  EXPECT_EQ(database.timestampCreate(), database.timestampLast());
  EXPECT_EQ(database.handle(), database.uuidLast());

  openstudio::System::msleep(1000);

  EXPECT_EQ(static_cast<unsigned>(0), WorkflowRecord::getWorkflowRecords(database).size());

  openstudio::runmanager::Workflow rmWorkflow;
  rmWorkflow.addJob(openstudio::runmanager::JobType::Null, "");

  // Fake ProblemRecord so can test WorkflowRecord
  Problem problem("Dummy",WorkflowStepVector());
  ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

  WorkflowRecord workflowRecord(rmWorkflow, problemRecord, 0);
  EXPECT_EQ("", workflowRecord.name());
  EXPECT_EQ("", workflowRecord.displayName());
  EXPECT_EQ("", workflowRecord.description());
  EXPECT_TRUE(workflowRecord.timestampCreate() > database.timestampCreate());
  EXPECT_EQ(workflowRecord.timestampCreate(), workflowRecord.timestampLast());
  EXPECT_EQ(workflowRecord.handle(), rmWorkflow.uuid());

  EXPECT_EQ(database.timestampLast(), database.timestampCreate());
  EXPECT_EQ(database.handle(), database.uuidLast());

  openstudio::System::msleep(1000);

  database.save();

  EXPECT_TRUE(database.timestampLast() > database.timestampCreate());
  EXPECT_NE(database.handle(), database.uuidLast());

  DateTime databaseTimestampLast = database.timestampLast();
  UUID databaseUUIDLast = database.uuidLast();

  openstudio::System::msleep(1000);

  database.save();

  EXPECT_EQ(database.timestampLast(), databaseTimestampLast);
  EXPECT_EQ(database.uuidLast(), databaseUUIDLast);

  EXPECT_EQ(static_cast<unsigned>(1), WorkflowRecord::getWorkflowRecords(database).size());

  EXPECT_EQ(database.timestampLast(), databaseTimestampLast);
  EXPECT_EQ(database.uuidLast(), databaseUUIDLast);
  EXPECT_EQ(workflowRecord.timestampCreate(), workflowRecord.timestampLast());
  EXPECT_EQ(workflowRecord.handle(), rmWorkflow.uuid());

  EXPECT_TRUE(workflowRecord.setDescription("A simple workflowRecord"));

  EXPECT_EQ(database.timestampLast(), databaseTimestampLast);
  EXPECT_EQ(database.uuidLast(), databaseUUIDLast);
  EXPECT_TRUE(workflowRecord.timestampCreate() < workflowRecord.timestampLast());
  EXPECT_NE(workflowRecord.handle(), workflowRecord.uuidLast());

  openstudio::System::msleep(1000);

  database.save();

  EXPECT_TRUE(databaseTimestampLast < database.timestampLast());
  EXPECT_NE(database.uuidLast(), databaseUUIDLast);
  EXPECT_TRUE(workflowRecord.timestampCreate() < workflowRecord.timestampLast());
  EXPECT_NE(workflowRecord.handle(), workflowRecord.uuidLast());

  databaseTimestampLast = database.timestampLast();
  databaseUUIDLast = database.uuidLast();
  DateTime workflowTimestampLast = workflowRecord.timestampLast();
  UUID workflowUUIDLast = workflowRecord.uuidLast();

  EXPECT_EQ(static_cast<unsigned>(1), WorkflowRecord::getWorkflowRecords(database).size());

  EXPECT_EQ(database.timestampLast(), databaseTimestampLast);
  EXPECT_EQ(database.uuidLast(), databaseUUIDLast);
  EXPECT_EQ(workflowRecord.timestampLast(), workflowTimestampLast);
  EXPECT_EQ(workflowRecord.uuidLast(), workflowUUIDLast);

  EXPECT_TRUE(database.removeRecord(workflowRecord));

  EXPECT_EQ(static_cast<unsigned>(0), WorkflowRecord::getWorkflowRecords(database).size());

  EXPECT_EQ(database.timestampLast(), databaseTimestampLast);
  EXPECT_EQ(database.uuidLast(), databaseUUIDLast);
  EXPECT_EQ(workflowTimestampLast, workflowRecord.timestampLast());
  EXPECT_EQ(workflowUUIDLast, workflowRecord.uuidLast());

  openstudio::System::msleep(1000);

  database.save();

  EXPECT_TRUE(databaseTimestampLast < database.timestampLast());
  EXPECT_NE(database.uuidLast(), databaseUUIDLast);
  EXPECT_EQ(workflowTimestampLast, workflowRecord.timestampLast());
  EXPECT_EQ(workflowUUIDLast, workflowRecord.uuidLast());

}

