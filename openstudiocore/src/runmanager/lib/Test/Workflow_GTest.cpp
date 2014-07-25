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
*  Likey = cense along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>
#include "../JobFactory.hpp"
#include <boost/filesystem/path.hpp>
#include "../RunManager.hpp"
#include "../Workflow.hpp"
#include "../WorkItem.hpp"
#include <QDir>
#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"

#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace openstudio;
using namespace openstudio::runmanager;

TEST_F(RunManagerTestFixture, Workflow_PersistTest)
{
  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("EnergyPlusJobRunDB");
  openstudio::runmanager::RunManager kit(db, true);

  openstudio::runmanager::Workflow wf("modeltoidf->energyplus->ruby");
  std::string key = kit.persistWorkflow(wf);

  openstudio::runmanager::Workflow wf2 = kit.loadWorkflow(key);

  EXPECT_TRUE(wf == wf2);
}


TEST_F(RunManagerTestFixture, Workflow_CloneTest)
{
  openstudio::runmanager::Workflow wf("modeltoidf->energyplus->ruby");

  openstudio::runmanager::Workflow wf2(wf);

  EXPECT_TRUE(wf==wf2);

  wf.addJob(openstudio::runmanager::JobType::EnergyPlus);

  EXPECT_FALSE(wf==wf2);
}

TEST_F(RunManagerTestFixture, Workflow_AssignTest)
{
  openstudio::runmanager::Workflow wf("modeltoidf->energyplus->ruby");

  openstudio::runmanager::Workflow wf2;
 
  wf2 = wf;

  EXPECT_TRUE(wf==wf2);

  wf.addJob(openstudio::runmanager::JobType::EnergyPlus);

  EXPECT_FALSE(wf==wf2);
}

TEST_F(RunManagerTestFixture, Workflow_Swap)
{
  openstudio::runmanager::Workflow wf("modeltoidf->energyplus->ruby");
  openstudio::runmanager::Workflow wf2("modeltoidf->ruby->energyplus");
 
  wf.swap(wf2);

  EXPECT_TRUE(wf==openstudio::runmanager::Workflow("modeltoidf->ruby->energyplus"));
  EXPECT_TRUE(wf2==openstudio::runmanager::Workflow("modeltoidf->energyplus->ruby"));
}

TEST_F(RunManagerTestFixture, Workflow_StdSwap)
{
  openstudio::runmanager::Workflow wf("modeltoidf->energyplus->ruby");
  openstudio::runmanager::Workflow wf2("modeltoidf->ruby->energyplus");

  std::swap(wf, wf2); 

  EXPECT_TRUE(wf==openstudio::runmanager::Workflow("modeltoidf->ruby->energyplus"));
  EXPECT_TRUE(wf2==openstudio::runmanager::Workflow("modeltoidf->energyplus->ruby"));
}

TEST_F(RunManagerTestFixture, Workflow_ReplaceTest)
{
  openstudio::runmanager::Workflow wf;
  wf.addJob(openstudio::runmanager::JobType::ModelToIdf);
  wf.addJob(openstudio::runmanager::JobType::Null, "JobToReplace");
  wf.addJob(openstudio::runmanager::JobType::Ruby);
  wf.addJob(openstudio::runmanager::JobType::Null, "JobToReplace");

  wf.replaceJobs("JobToReplace", openstudio::runmanager::JobType::ExpandObjects);

  EXPECT_TRUE(wf == openstudio::runmanager::Workflow("ModelToIdf->ExpandObjects->Ruby->ExpandObjects"));
}

TEST_F(RunManagerTestFixture, Workflow_ReplaceWithWorkflowTest)
{
  openstudio::runmanager::Workflow wf;
  wf.addJob(openstudio::runmanager::JobType::ModelToIdf);
  wf.addJob(openstudio::runmanager::JobType::Null, "JobToReplace");
  wf.addJob(openstudio::runmanager::JobType::Ruby);
  wf.addJob(openstudio::runmanager::JobType::Null, "JobToReplace");

  wf.replaceJobs("JobToReplace", openstudio::runmanager::Workflow("ModelToIdf->ExpandObjects")); // replace a key with an entire workflow
                                                                                                 // it's inserted into the middle with the children
                                                                                                 // attached to the "last" job of the new chain
                                                                                                 // (this can be tricky if the workflow inserted is
                                                                                                 // not a simple linear one

  EXPECT_TRUE(wf == openstudio::runmanager::Workflow("ModelToIdf->ModelToIdf->ExpandObjects->Ruby->ModelToIdf->ExpandObjects"));


}


TEST_F(RunManagerTestFixture, Workflow_MultipleReplace)
{
  openstudio::runmanager::Workflow wf;
  wf.addJob(openstudio::runmanager::JobType::ModelToIdf);
  wf.addJob(openstudio::runmanager::JobType::Null, "JobToReplace");
  wf.addJob(openstudio::runmanager::JobType::Ruby);
  wf.addJob(openstudio::runmanager::JobType::Null, "JobToReplace2");

  wf.replaceJobs("JobToReplace", openstudio::runmanager::JobType::ExpandObjects);
  wf.replaceJobs("JobToReplace2", openstudio::runmanager::JobType::EnergyPlus);

  EXPECT_TRUE(wf == openstudio::runmanager::Workflow("ModelToIdf->ExpandObjects->Ruby->EnergyPlus"));
}


TEST_F(RunManagerTestFixture, Workflow_ReplaceHandSetKey)
{
  openstudio::runmanager::Workflow wf;
  openstudio::runmanager::JobParams p;
  p.append("workflowjobkey", "JobToReplace"); // note that the key is just a param
  wf.addJob(openstudio::runmanager::JobType::Null, openstudio::runmanager::Tools(), p, openstudio::runmanager::Files() );

  openstudio::runmanager::Workflow wf2;
  wf2.addJob(openstudio::runmanager::JobType::Null, "JobToReplace");
  EXPECT_TRUE(wf == wf2); // compare handbuilt to api built


  wf.replaceJobs("JobToReplace", openstudio::runmanager::Workflow("ExpandObjects"));

  EXPECT_TRUE(wf == openstudio::runmanager::Workflow("ExpandObjects"));
}

TEST_F(RunManagerTestFixture, Workflow_FromJobTree)
{
  openstudio::runmanager::Workflow wf("ModelToIdf->EnergyPlus");
  openstudio::runmanager::Job j(wf.create());
  openstudio::runmanager::Workflow wf2(j); // construct workflow from Job

  EXPECT_TRUE(wf == wf2);
}

TEST_F(RunManagerTestFixture, Workflow_Append)
{
  openstudio::runmanager::Workflow wf("ModelToIdf->EnergyPlus");
  wf.addWorkflow(openstudio::runmanager::Workflow("Null->ExpandObjects"));

  openstudio::runmanager::Workflow wf2("ModelToIdf->EnergyPlus->Null->ExpandObjects");

  EXPECT_TRUE(wf == wf2);
}

TEST_F(RunManagerTestFixture, Workflow_AdvancedTree)
{
  openstudio::runmanager::Workflow advancedWorkflow;

  { // make a scope for this just to keep things clean
    // create structure:
    // modeltoidf->Null
    //           ->Null
    //           ->Null
    //           ->Null
    //       (finishedjob) -> Null 
    openstudio::runmanager::Job parent = openstudio::runmanager::Workflow("ModelToIdf").create();

    for (int i = 0; i < 4; ++i)
    {
      openstudio::runmanager::Workflow wf;
      wf.addJob(openstudio::runmanager::JobType::Null, "childjob");
      parent.addChild(wf.create());
    }

    openstudio::runmanager::Workflow finishedwf;
    finishedwf.addJob(openstudio::runmanager::JobType::Null, "finishedjob");
    parent.setFinishedJob(finishedwf.create());

    advancedWorkflow = openstudio::runmanager::Workflow(parent); // Create a new workflow from job tree we just created
  }

  // Now that we have our structure, we will replace the "childjob"s with Ruby->EnergyPlus
  // and replace "finishedjob" with Ruby
  advancedWorkflow.replaceJobs("childjob", openstudio::runmanager::Workflow("Ruby->EnergyPlus"));
  advancedWorkflow.replaceJobs("finishedjob", openstudio::runmanager::Workflow("Ruby"));

  // Now we should have a workflow that contains the following tree:
  //
  // modeltoidf->Ruby->EnergyPlus
  //           ->Ruby->EnergyPlus
  //           ->Ruby->EnergyPlus
  //           ->Ruby->EnergyPlus
  //       (finishedjob) -> Ruby
  //
  
  // We will now create the full job tree and verify that we have what we think we have

  openstudio::runmanager::Job jobtree = advancedWorkflow.create();

  EXPECT_EQ(jobtree.jobType(), openstudio::runmanager::JobType::ModelToIdf);

  ASSERT_TRUE(jobtree.finishedJob());
  EXPECT_EQ(jobtree.finishedJob()->jobType(), openstudio::runmanager::JobType::Ruby);
  EXPECT_FALSE(jobtree.finishedJob()->finishedJob());
  EXPECT_EQ(jobtree.finishedJob()->children().size(), static_cast<size_t>(0));

  std::vector<openstudio::runmanager::Job> children = jobtree.children();
  EXPECT_EQ(children.size(), static_cast<size_t>(4));

  for (size_t i = 0; i < children.size(); ++i)
  {
    EXPECT_FALSE(children.at(i).finishedJob());
    EXPECT_EQ(children.at(i).jobType(), openstudio::runmanager::JobType::Ruby);
    ASSERT_EQ(children.at(i).children().size(), static_cast<size_t>(1));
    openstudio::runmanager::Job child = children.at(i).children().at(0);
    EXPECT_EQ(child.jobType(), openstudio::runmanager::JobType::EnergyPlus);
    EXPECT_EQ(child.children().size(), static_cast<size_t>(0));
    EXPECT_FALSE(child.finishedJob());

  }
}


TEST_F(RunManagerTestFixture, Workflow_PersistDeleteByName)
{
  RunManager rm;
  
  Workflow wf("null->energyplus", "mywfname");
  rm.persistWorkflow(wf);

  Workflow wf2 = rm.loadWorkflowByName("mywfname");
  EXPECT_EQ(wf2.getName(), "mywfname");
  
  Job j = wf2.create();
  EXPECT_EQ(j.jobType(), JobType::Null);
  EXPECT_EQ(j.children().size(), 1u);

  rm.deleteWorkflowByName("mywfname");

  EXPECT_THROW(rm.loadWorkflowByName("mywfname"), std::runtime_error);
}

TEST_F(RunManagerTestFixture, Workflow_ToWorkItemsAndBack)
{
  Workflow wf("null->energyplus");
  wf.addJob(JobType::Null, "myjobname");
  std::vector<WorkItem> wis = wf.toWorkItems();
  EXPECT_EQ("myjobname", wis.at(2).jobkeyname);
  Workflow wf2(wis);

  EXPECT_EQ(wf, wf2);
}

TEST_F(RunManagerTestFixture, Workflow_ToWorkItems)
{
  ToolInfo ti("tool", openstudio::toPath("somepath"));
  Tools tis;
  tis.append(ti);

  JobParam param("param1");
  JobParams params;
  params.append(param);

  FileInfo fi(openstudio::toPath("somefile.txt"), "txt");
  Files fis;
  fis.append(fi);

  Workflow wf("null->energyplus");
  wf.add(params);
  wf.add(tis);
  wf.add(fis);

  std::vector<WorkItem> wis = wf.toWorkItems();

  ASSERT_EQ(2u, wis.size());
  EXPECT_EQ(JobType(JobType::Null), wis[0].type);
  EXPECT_EQ(JobType(JobType::EnergyPlus), wis[1].type);

  EXPECT_EQ(params, wis[0].params);
  EXPECT_EQ(fis, wis[0].files);
  EXPECT_EQ(tis, wis[0].tools);

  EXPECT_TRUE(wis[1].params.params().empty());
  EXPECT_TRUE(wis[1].files.files().empty());
  EXPECT_TRUE(wis[1].tools.tools().empty());
} 

TEST_F(RunManagerTestFixture, Workflow_FromWorkItems)
{
  ToolInfo ti("tool", openstudio::toPath("somepath"));
  Tools tis;
  tis.append(ti);

  JobParam param("param1");
  JobParams params;
  params.append(param);

  FileInfo fi(openstudio::toPath("somefile.txt"), "txt");
  Files fis;
  fis.append(fi);

  std::vector<WorkItem> wi;
  wi.push_back(WorkItem(JobType::Null, tis, params, fis));
  wi.push_back(WorkItem(JobType::EnergyPlus));

  Workflow wf(wi);

  EXPECT_EQ(tis, wf.tools());
  EXPECT_EQ(params, wf.params());
  EXPECT_EQ(fis, wf.files());
} 

TEST_F(RunManagerTestFixture, Workflow_ToWorkItemsFailed)
{
  // the toWorkItems method can only work if the job tree is linear
  Job j = JobFactory::createNullJob();
  Job c1 = JobFactory::createNullJob();
  Job c2 = JobFactory::createNullJob();

  j.addChild(c1);

  Workflow wf(j);
  // first test this without an error
  EXPECT_NO_THROW(wf.toWorkItems());

  j.addChild(c2);
  Workflow wf2(j);

  EXPECT_THROW(wf2.toWorkItems(), std::runtime_error);
}

TEST_F(RunManagerTestFixture, Workflow_AddWorkItem)
{
  WorkItem wi(JobType::Null);

  Workflow wf;
  wf.addJob(wi);

  Job j = wf.create();

  EXPECT_EQ(JobType(JobType::Null), j.jobType());
}
