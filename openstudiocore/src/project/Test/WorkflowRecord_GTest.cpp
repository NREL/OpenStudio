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

