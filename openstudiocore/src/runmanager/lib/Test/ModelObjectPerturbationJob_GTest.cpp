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
*  Likey = cense along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>

#include <runmanager/lib/JobFactory.hpp>
#include <runmanager/lib/ModelObjectPerturbationJob.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/ModelObjectActionSetRelationship.hpp>

#include <model/Model.hpp>
#include <model/Component.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/LightsDefinition.hpp>
#include <model/LightsDefinition_Impl.hpp>

#include <boost/filesystem/path.hpp>

#include <QDir>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;

TEST_F(RunManagerTestFixture, ModelObjectPerturbationJob)
{
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) /  openstudio::toPath("ModelObjectPerturbationTest");
  if (boost::filesystem::exists(outdir)) {
    boost::filesystem::remove_all(outdir); // needed because RunManager will not overwrite out.osm
  }
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("ModelObjectPerturbationJobDB");
  openstudio::runmanager::RunManager kit(db, true);

  openstudio::path originalfile = outdir / openstudio::toPath("ModelObjectPerturbationJobDB.osm");

  openstudio::model::Model m = openstudio::model::exampleModel();
  m.save(originalfile, true);

  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager");

  openstudio::path wallComponentPath = openstudio::toPath("./ModelObjectPerturbationJob_wall_construction.osc");
  openstudio::path roofComponentPath = openstudio::toPath("./ModelObjectPerturbationJob_roof_construction.osc");

  {
    openstudio::model::Model model;

    openstudio::model::Construction wallConstruction(model);
    wallConstruction.setName("ModelRulesetWallConstruction");
    wallConstruction.createComponent().save(wallComponentPath, true);

    openstudio::model::Construction roofConstruction(model);
    roofConstruction.setName("ModelRulesetRoofConstruction");
    roofConstruction.createComponent().save(roofComponentPath, true);
  }

  openstudio::ruleset::ModelObjectFilterType typeFilter(openstudio::model::Surface::iddObjectType());
  openstudio::ruleset::ModelObjectFilterStringAttribute isWallFilter("surfaceType", openstudio::ruleset::RulesetStringPredicate::IEquals, "Wall");
  openstudio::ruleset::ModelObjectActionSetRelationship setWallConstruction("construction", wallComponentPath);

  openstudio::ruleset::ModelObjectFilterStringAttribute isRoofFilter("surfaceType", openstudio::ruleset::RulesetStringPredicate::IEquals, "RoofCeiling");
  openstudio::ruleset::ModelObjectActionSetRelationship setRoofConstruction("construction", roofComponentPath);

  openstudio::ruleset::ModelRule wallRule("WallRule");
  wallRule.add(typeFilter);
  wallRule.add(isWallFilter);
  wallRule.add(setWallConstruction);

  openstudio::ruleset::ModelRule roofRule("RoofRule");
  roofRule.add(typeFilter);
  roofRule.add(isRoofFilter);
  roofRule.add(setRoofConstruction);

  openstudio::ruleset::ModelRuleset wallRuleset("WallRuleset");
  wallRuleset.add(wallRule);

  openstudio::ruleset::ModelRuleset roofRuleset("RoofRuleset");
  roofRuleset.add(roofRule);

  // ETH@20130326 - Add third ruleset because merging of jobs seems to breakdown after two
  ModelRule lightsRule("LightsRule");
  lightsRule.add(ModelObjectFilterType(model::LightsDefinition::iddObjectType()));
  lightsRule.add(ModelObjectActionSetAttribute("wattsperSpaceFloorArea",8.0));
  ModelRuleset lightsRuleset("LightsRuleset");
  lightsRuleset.add(lightsRule);
  // check original assumptions about example model
  model::LightsDefinitionVector lightDefinitions = m.getModelObjects<model::LightsDefinition>();
  ASSERT_EQ(1u,lightDefinitions.size());
  ASSERT_EQ("Watts/Area",lightDefinitions[0].designLevelCalculationMethod());
  EXPECT_DOUBLE_EQ(10.0,lightDefinitions[0].wattsperSpaceFloorArea().get());

  // we'll use the constructor for workflow that allows us to create a workflow element from a job
  openstudio::runmanager::Workflow wf(openstudio::runmanager::JobFactory::createModelObjectPerturbationJob(originalfile, wallRuleset));
  wf.addWorkflow(openstudio::runmanager::Workflow(openstudio::runmanager::JobFactory::createModelObjectPerturbationJob(roofRuleset)));
  wf.addJob(WorkItem(lightsRuleset));
  EXPECT_EQ(3u,wf.toWorkItems().size());
  openstudio::runmanager::Job headjob(wf.create(outdir));
  ASSERT_EQ(1u,headjob.children().size());
  ASSERT_EQ(1u,headjob.children()[0].children().size());
  EXPECT_EQ(0u,headjob.children()[0].children()[0].children().size());

  // Let's optimize the job tree to merge the two PerturbationJobs
  // ETH@20130326 - If you comment out the next two lines, the test will pass.
  openstudio::runmanager::JobFactory::optimizeJobTree(headjob);
  EXPECT_EQ(0u,headjob.children().size());

  kit.enqueue(headjob, true);

  kit.waitForFinished();

  // JMT: The job ModelInModelOut base class automatically creates an output file called "out.osm" when the last job completes
  openstudio::runmanager::FileInfo fi = headjob.treeOutputFiles().getLastByExtension("osm");
  EXPECT_EQ(fi.filename, "out.osm");

  boost::optional<openstudio::model::Model> model = openstudio::model::Model::load(fi.fullPath);
  ASSERT_TRUE(model);

  boost::optional<openstudio::model::Construction> wallConstruction = model->getModelObjectByName<openstudio::model::Construction>("ModelRulesetWallConstruction");
  ASSERT_TRUE(wallConstruction);

  boost::optional<openstudio::model::Construction> roofConstruction = model->getModelObjectByName<openstudio::model::Construction>("ModelRulesetRoofConstruction");
  ASSERT_TRUE(roofConstruction);

  unsigned numWalls = 0;
  unsigned numRoofs = 0;
  unsigned numOther = 0;
  BOOST_FOREACH(const openstudio::model::Surface& surface, model->getModelObjects<openstudio::model::Surface>()){
    std::string surfaceType = surface.surfaceType();
    ASSERT_TRUE(surface.construction());

    if (openstudio::istringEqual("RoofCeiling", surface.surfaceType())){
      EXPECT_TRUE(roofConstruction->handle() == surface.construction()->handle());
      ++numRoofs;
    }else if (openstudio::istringEqual("Wall", surface.surfaceType())){
      EXPECT_TRUE(wallConstruction->handle() == surface.construction()->handle());
      ++numWalls;
    }else{
      EXPECT_TRUE(wallConstruction->handle() != surface.construction()->handle());
      EXPECT_TRUE(roofConstruction->handle() != surface.construction()->handle());
      ++numOther;
    }
  }

  EXPECT_TRUE(numWalls > 0);
  EXPECT_TRUE(numRoofs > 0);
  EXPECT_TRUE(numOther > 0);

  // ETH@20130326 - Check that lighting rule was applied successfully.
  lightDefinitions = model->getModelObjects<model::LightsDefinition>();
  ASSERT_EQ(1u,lightDefinitions.size());
  ASSERT_EQ("Watts/Area",lightDefinitions[0].designLevelCalculationMethod());
  EXPECT_DOUBLE_EQ(8.0,lightDefinitions[0].wattsperSpaceFloorArea().get());
}
