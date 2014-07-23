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

#include <QElapsedTimer>

#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../LocalProcessCreator.hpp"
#include "../RubyJobUtils.hpp"
#include "../WorkItem.hpp"
#include "../MergedJobResults.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/filetypes/EpwFile.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"
#include "../../../utilities/bcl/BCLMeasure.hpp"
#include "../../../utilities/core/ApplicationPathHelpers.hpp"


#include <utilities/idd/OS_TimeDependentValuation_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <boost/filesystem/path.hpp>

#include "../../../ruleset/OSArgument.hpp"

#include <resources.hxx>

using namespace openstudio;

void assertEqual(const ruleset::OSArgument &t_lhs, const ruleset::OSArgument &t_rhs)
{
  ASSERT_EQ(t_lhs.uuid(), t_rhs.uuid());
  ASSERT_EQ(t_lhs.versionUUID(), t_rhs.versionUUID());
  ASSERT_EQ(t_lhs.name(), t_rhs.name());
  ASSERT_EQ(t_lhs.displayName(), t_rhs.displayName());
  ASSERT_EQ(t_lhs.type(), t_rhs.type());
  ASSERT_EQ(t_lhs.required(), t_rhs.required());
  ASSERT_EQ(t_lhs.isRead(), t_rhs.isRead());
  ASSERT_EQ(t_lhs.extension(), t_rhs.extension());
  ASSERT_EQ(t_lhs.choiceValues(), t_rhs.choiceValues());
  ASSERT_EQ(t_lhs.choiceValueDisplayNames(), t_rhs.choiceValueDisplayNames());
  ASSERT_EQ(t_lhs.hasValue(), t_rhs.hasValue());
  ASSERT_EQ(t_lhs.hasDefaultValue(), t_rhs.hasDefaultValue());
  ASSERT_EQ(t_lhs.hasValue(), t_rhs.hasValue());
  ASSERT_EQ(t_lhs.hasDefaultValue(), t_rhs.hasDefaultValue());
  ASSERT_EQ(t_lhs.valueAsQVariant(), t_rhs.valueAsQVariant());
  ASSERT_EQ(t_lhs.defaultValueAsQVariant(), t_rhs.defaultValueAsQVariant());
  ASSERT_EQ(t_lhs.domainType(), t_rhs.domainType());
  ASSERT_EQ(t_lhs.hasDomain(), t_rhs.hasDomain());
  if (t_lhs.hasDomain()) {
    ASSERT_EQ(t_lhs.domainAsString(), t_rhs.domainAsString());
  }
}

TEST_F(RunManagerTestFixture, UserScript_Factory)
{
  runmanager::Job j = runmanager::JobFactory::createUserScriptJob(openstudio::toPath("my.rb"),
      std::vector<ruleset::OSArgument>());


  ASSERT_EQ(j.jobType(), openstudio::runmanager::JobType::UserScript);

}

TEST_F(RunManagerTestFixture, UserScript_Build)
{
  runmanager::RubyJobBuilder rjb = runmanager::RubyJobBuilder::createUserScriptRubyJob(openstudio::toPath("somepath"),
      std::vector<ruleset::OSArgument>(),
      openstudio::path(),
      false);

  runmanager::Workflow wf;
  rjb.addToWorkflow(wf);

  openstudio::runmanager::Job j = wf.create();

  ASSERT_EQ(j.jobType(), openstudio::runmanager::JobType::UserScript);
}


TEST_F(RunManagerTestFixture, UserScript_Params)
{
  UUID uuid;
  UUID versionUuid;
  std::string name = "name";
  std::string displayName = "displayName";
  ruleset::OSArgumentType type = ruleset::OSArgumentType::Double;
  bool required = false;
  std::string value = toString(10.0);
  std::string defaultValue = toString(11.89);
  std::vector<std::string> choices, choiceDisplayNames;
  choices.push_back("11.0"); choiceDisplayNames.push_back("Low Value");
  choices.push_back("89.6"); choiceDisplayNames.push_back("High Value");
  bool isRead = true;
  std::string extension = "ext";
  ruleset::OSDomainType domainType = ruleset::OSDomainType::Interval;
  std::vector<std::string> domain;
  domain.push_back("0.0");
  domain.push_back("100.0");

  ruleset::OSArgument arg(
      uuid,
      versionUuid,
      name,
      displayName,
      type,
      required,
      value,
      defaultValue,
      domainType,
      domain,
      choices,
      choiceDisplayNames,
      isRead,
      extension);

  std::vector<ruleset::OSArgument> args;
  args.push_back(arg);

  runmanager::JobParams argParams = runmanager::RubyJobBuilder::toJobParams(args);
  std::vector<ruleset::OSArgument> args2 = runmanager::RubyJobBuilder::toOSArguments(argParams);

  ASSERT_FALSE(args2.empty());
  assertEqual(args.at(0), args2.at(0));
}

TEST_F(RunManagerTestFixture, UserScript_WorkItem)
{
  runmanager::RubyJobBuilder rjb = runmanager::RubyJobBuilder::createUserScriptRubyJob(openstudio::toPath("somepath"),
      std::vector<ruleset::OSArgument>(),
      openstudio::path(),
      false);

  openstudio::runmanager::WorkItem wi = rjb.toWorkItem();

  runmanager::RubyJobBuilder rjb2(wi);

  openstudio::runmanager::WorkItem wi2 = rjb2.toWorkItem();

  EXPECT_EQ(wi.type, wi2.type);
  EXPECT_EQ(wi.files, wi2.files);
  EXPECT_EQ(wi.params, wi2.params);
  EXPECT_EQ(wi.tools, wi2.tools);
  EXPECT_EQ(wi, wi2);
}



TEST_F(RunManagerTestFixture, UserScript_WorkItemWithArg)
{
  UUID uuid;
  UUID versionUuid;
  std::string name = "name";
  std::string displayName = "displayName";
  ruleset::OSArgumentType type = ruleset::OSArgumentType::Path;
  bool required = false;
  std::string value = "test.osm";
  std::string defaultValue = "empty.osm";
  bool isRead = true;
  std::string extension = "osm";
  ruleset::OSDomainType domainType = ruleset::OSDomainType::Interval;
  std::vector<std::string> domain;
  domain.push_back("0.0");
  domain.push_back("100.0");

  ruleset::OSArgument arg(
      uuid,
      versionUuid,
      name,
      displayName,
      type,
      required,
      value,
      defaultValue,
      domainType,
      domain,
      std::vector<std::string>(),
      std::vector<std::string>(),
      isRead,
      extension);

  std::vector<ruleset::OSArgument> args;
  args.push_back(arg);

  runmanager::RubyJobBuilder rjb = runmanager::RubyJobBuilder::createUserScriptRubyJob(openstudio::toPath("somepath"),
      args,
      openstudio::path(),
      false);

  rjb.addRequiredFile(openstudio::toPath("some.osm"), openstudio::toPath("in.osm"));

  openstudio::runmanager::WorkItem wi = rjb.toWorkItem();

  runmanager::RubyJobBuilder rjb2(wi);

  openstudio::runmanager::WorkItem wi2 = rjb2.toWorkItem();

  EXPECT_EQ(wi.type, wi2.type);
  EXPECT_EQ(wi.files, wi2.files);
  EXPECT_EQ(wi.params, wi2.params);
  EXPECT_EQ(wi.tools, wi2.tools);
  EXPECT_EQ(wi, wi2);

}

openstudio::runmanager::Job buildScriptMergingWorkflow(const openstudio::path &t_outdir)
{
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade");
  openstudio::path osm = resourcesPath() / toPath("/runmanager/SimpleModel.osm");
  openstudio::path epw = resourcesPath() / toPath("/runmanager/USA_CO_Golden-NREL.724666_TMY3.epw");

  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);

  openstudio::runmanager::Workflow wf;

  std::vector<openstudio::ruleset::OSArgument> args;
  args.push_back(openstudio::ruleset::OSArgument::makeDoubleArgument("wwr"));
  args.push_back(openstudio::ruleset::OSArgument::makeDoubleArgument("sillHeight"));
  args.push_back(openstudio::ruleset::OSArgument::makeStringArgument("facade"));

  args[0].setValue(0.1);
  args[1].setValue(0.2);
  args[2].setValue("North");

  // Add job one
  openstudio::runmanager::RubyJobBuilder rubyjobbuilder(*measure, args);
  rubyjobbuilder.setIncludeDir(getOpenStudioRubyIncludePath());
  wf.addJob(rubyjobbuilder.toWorkItem());

  // add job two
  args[2].setValue("East");
  openstudio::runmanager::RubyJobBuilder rubyjobbuilder2(*measure, args);
  rubyjobbuilder2.setIncludeDir(getOpenStudioRubyIncludePath());
  wf.addJob(rubyjobbuilder2.toWorkItem());
  
  // add job three, applying a new value for the "east" to make sure that the jobs
  // are applied in the proper order
  args[2].setValue("East");
  args[1].setValue(0.3);
  openstudio::runmanager::RubyJobBuilder rubyjobbuilder3(*measure, args);
  rubyjobbuilder3.setIncludeDir(getOpenStudioRubyIncludePath());
  wf.addJob(rubyjobbuilder3.toWorkItem());


  wf.addJob(openstudio::runmanager::JobType::ModelToIdf);

  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), 
        rubyExePath().parent_path(), openstudio::path(),
        openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path());

  wf.add(tools);
  wf.addParam(runmanager::JobParam("flatoutdir"));

  openstudio::runmanager::Job j = wf.create(t_outdir, osm, epw);

  return j;
}

TEST_F(RunManagerTestFixture, UserScriptJobMerging)
{
  std::string originalosm;
  std::string mergedosm;
  std::string unmergedosm;

  std::string mergedidf;
  std::string unmergedidf;

  {
    boost::optional<openstudio::model::Model> m = openstudio::model::Model::load(resourcesPath() / toPath("/runmanager/SimpleModel.osm"));
    ASSERT_TRUE(m);
    std::stringstream ss;
    m->toIdfFile().print(ss);
    originalosm = ss.str();
  }


  QElapsedTimer timer;
  timer.start();

  { 
    openstudio::runmanager::RunManager rm(openstudio::tempDir() / openstudio::toPath("UserScriptJobMergeUnMerged.db"), true, true);
    openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("UserScriptJobMergeUnMerged");

    boost::filesystem::remove_all(outdir); // Clean up test dir before starting

    openstudio::runmanager::Job j = buildScriptMergingWorkflow(outdir);

    ASSERT_EQ(1u, j.children().size());
    ASSERT_EQ(1u, j.children()[0].children().size());
    ASSERT_EQ(1u, j.children()[0].children()[0].children().size());
    ASSERT_TRUE(j.children()[0].children()[0].children()[0].children().empty());

    rm.enqueue(j, true);
    EXPECT_EQ(4u, rm.getJobs().size());
    rm.setPaused(false);
    rm.waitForFinished();

    EXPECT_TRUE(j.treeErrors().succeeded());
    openstudio::runmanager::FileInfo fi = j.treeOutputFiles().getLastByExtension("osm");
    boost::optional<openstudio::model::Model> m = openstudio::model::Model::load(fi.fullPath);
    ASSERT_TRUE(m);
    std::stringstream ss;
    m->toIdfFile().print(ss);
    unmergedosm = ss.str();

    openstudio::runmanager::FileInfo idf = j.treeOutputFiles().getLastByExtension("idf");
    boost::optional<openstudio::IdfFile> f = openstudio::IdfFile::load(idf.fullPath);
    EXPECT_TRUE(f);

    if (f)
    {
      ss.str("");
      f->print(ss);
      unmergedidf = ss.str();
    }

    EXPECT_EQ(3u, j.treeOutputFiles().getAllByFilename("result.ossr").files().size());
  }

  qint64 unmergedtime = timer.restart();

  { 
    openstudio::runmanager::RunManager rm(openstudio::tempDir() / openstudio::toPath("UserScriptJobMergeMerged.db"), true, true);
    openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("UserScriptJobMergeMerged");

    boost::filesystem::remove_all(outdir); // Clean up test dir before starting

    openstudio::runmanager::Job j = buildScriptMergingWorkflow(outdir);

    ASSERT_EQ(1u, j.children().size());
    ASSERT_EQ(1u, j.children()[0].children().size());
    ASSERT_EQ(1u, j.children()[0].children()[0].children().size());
    ASSERT_TRUE(j.children()[0].children()[0].children()[0].children().empty());

    openstudio::UUID job1uuid = j.uuid();
    openstudio::UUID job2uuid = j.children()[0].uuid();
    openstudio::UUID job3uuid = j.children()[0].children()[0].uuid();

    openstudio::runmanager::JobFactory::optimizeJobTree(j);

    EXPECT_EQ(1u, j.children().size());
    EXPECT_TRUE(j.children()[0].children().empty());

    rm.enqueue(j, true);
    EXPECT_EQ(2u, rm.getJobs().size());
    rm.setPaused(false);
    rm.waitForFinished();

    EXPECT_TRUE(j.treeErrors().succeeded());
    openstudio::runmanager::FileInfo fi = j.treeOutputFiles().getLastByExtension("osm");
    boost::optional<openstudio::model::Model> m = openstudio::model::Model::load(fi.fullPath);
    ASSERT_TRUE(m);
    std::stringstream ss;
    m->toIdfFile().print(ss);
    mergedosm = ss.str();

    openstudio::runmanager::FileInfo idf = j.treeOutputFiles().getLastByExtension("idf");
    boost::optional<openstudio::IdfFile> f = openstudio::IdfFile::load(idf.fullPath);
    EXPECT_TRUE(f);
    if (f)
    {
      ss.str("");
      f->print(ss);
      mergedidf = ss.str();
    }


    EXPECT_EQ(3u, j.treeOutputFiles().getAllByFilename("result.ossr").files().size());

    ASSERT_TRUE(j.hasMergedJobs());
    
    std::vector<openstudio::runmanager::MergedJobResults> mergedResults = j.mergedJobResults();
    ASSERT_EQ(3u, mergedResults.size());

    EXPECT_EQ(job1uuid, mergedResults[0].uuid);
    EXPECT_EQ(job2uuid, mergedResults[1].uuid);
    EXPECT_EQ(job3uuid, mergedResults[2].uuid);

    EXPECT_TRUE(mergedResults[0].errors.succeeded());
    EXPECT_TRUE(mergedResults[1].errors.succeeded());
    EXPECT_TRUE(mergedResults[2].errors.succeeded());

    ASSERT_FALSE(mergedResults[0].outputFiles.files().empty());
    ASSERT_FALSE(mergedResults[1].outputFiles.files().empty());
    ASSERT_FALSE(mergedResults[2].outputFiles.files().empty());

    EXPECT_EQ(openstudio::toPath("mergedjob-0"), mergedResults[0].outputFiles.getLastByFilename("result.ossr").fullPath.parent_path().filename());
    EXPECT_EQ(openstudio::toPath("mergedjob-1"), mergedResults[1].outputFiles.getLastByFilename("result.ossr").fullPath.parent_path().filename());
    EXPECT_EQ(openstudio::toPath("mergedjob-2"), mergedResults[2].outputFiles.getLastByFilename("result.ossr").fullPath.parent_path().filename());
  }

  qint64 mergedtime = timer.restart();

  EXPECT_FALSE(originalosm.empty());
  EXPECT_FALSE(mergedosm.empty());
  EXPECT_FALSE(unmergedosm.empty());

  EXPECT_NE(originalosm, unmergedosm);
  EXPECT_NE(originalosm, mergedosm);

  EXPECT_FALSE(mergedidf.empty());
  EXPECT_FALSE(unmergedidf.empty());

  // expected a merged run to be at least 2x faster than an unmerged run
  EXPECT_LT(2*mergedtime, unmergedtime);

  std::cout << "MergedTime: " << mergedtime << " UnmergedTime " << unmergedtime << std::endl;
}

TEST_F(RunManagerTestFixture, BCLMeasureRubyScript)
{
  openstudio::path dir = resourcesPath() / toPath("/runmanager/DummyMeasure");
  openstudio::path osm = resourcesPath() / toPath("/runmanager/SimpleModel.osm");
  openstudio::path epw = resourcesPath() / toPath("/runmanager/USA_CO_Golden-NREL.724666_TMY3.epw");

  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);

  openstudio::runmanager::RunManager rm(openstudio::tempDir() / openstudio::toPath("BCLMeasureRubyScript.db"), true, true);
  openstudio::runmanager::Workflow wf;
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("BCLMeasureRubyScript");

  openstudio::runmanager::RubyJobBuilder rubyjobbuilder(*measure, std::vector<openstudio::ruleset::OSArgument>());
  rubyjobbuilder.setIncludeDir(getOpenStudioRubyIncludePath());

  wf.addJob(rubyjobbuilder.toWorkItem());
  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), 
        rubyExePath().parent_path(), openstudio::path(),
        openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path());

  wf.add(tools);

  boost::filesystem::remove_all(outdir); // Clean up test dir before starting

  openstudio::runmanager::Job j = wf.create(outdir, osm, epw);
  rm.enqueue(j, true);

  std::vector<openstudio::runmanager::FileInfo> infiles = j.inputFiles();
  ASSERT_EQ(2u, infiles.size());
  
  openstudio::runmanager::FileInfo fi = infiles[1];
  //Make sure epw got attached properly
  EXPECT_EQ("SimpleModel.osm", fi.filename);
  ASSERT_EQ(1u, fi.requiredFiles.size());
  ASSERT_EQ("in.epw", openstudio::toString(fi.requiredFiles[0].second));

  rm.setPaused(false);

  rm.waitForFinished();

  // DLM: is this failing because we are in normal cleanup mode?
  std::vector<openstudio::runmanager::FileInfo> outfiles = j.outputFiles();
  ASSERT_EQ(4u, outfiles.size());
  
  openstudio::runmanager::FileInfo fi2 = outfiles[0];
  //Make sure epw got copied from input to output
  EXPECT_EQ("out.osm", fi2.filename);
  ASSERT_EQ(1u, fi2.requiredFiles.size());
  ASSERT_EQ("in.epw", openstudio::toString(fi2.requiredFiles[0].second));

}


TEST_F(RunManagerTestFixture, RelocateDaylightSimPath)
{
  using namespace openstudio;
  using namespace openstudio::runmanager;

  Workflow wf;

  RubyJobBuilder rjb;
  rjb.setIncludeDir(getOpenStudioRubyIncludePath());
  rjb.addScriptArgument("in.osm");
  rjb.addScriptArgument(openstudio::toString(openstudio::toPath("/path/to/radiance")));
  rjb.addInputFile(FileSelection::Last, FileSource::All, ".*\\.osm", "in.osm");
  openstudio::path scriptsPath = openstudio::toPath("some/random/path") / openstudio::toPath("openstudio/radiance/");
  ASSERT_TRUE(rjb.addRequiredFile(scriptsPath / openstudio::toPath("ModelToRad.rb"), openstudio::toPath("ModelToRad.rb"), openstudio::path(), false));
  ASSERT_TRUE(rjb.addRequiredFile(scriptsPath / openstudio::toPath("DaylightSim-Simple.rb"), openstudio::toPath("DaylightSim-Simple.rb"), openstudio::path(), false));
  ASSERT_TRUE(rjb.addRequiredFile(scriptsPath / openstudio::toPath("MakeSchedules.rb"), openstudio::toPath("MakeSchedules.rb"), openstudio::path(), false));
  ASSERT_TRUE(rjb.addRequiredFile(scriptsPath / openstudio::toPath("DaylightMetrics.rb"), openstudio::toPath("DaylightMetrics.rb"), openstudio::path(), false));
  ASSERT_EQ(rjb.requiredFiles().size(), 4u);
  rjb.copyRequiredFiles("osm", "osm", "in.epw");
  rjb.setScriptFile(scriptsPath / openstudio::toPath("DaylightCalculations.rb"));
  rjb.addToWorkflow(wf);

  WorkItem wi = wf.toWorkItems().at(0);
  wi.jobkeyname = "testjobkeyname";

  LOG_FREE(Debug, "RunManagerTestFixture", "workitem: " << wi.toJSON());

  ASSERT_EQ(wi.files.files().at(0).fullPath, openstudio::toPath("some/random/path/openstudio/radiance/DaylightCalculations.rb"));
  ASSERT_EQ(wi.files.files().at(0).requiredFiles.size(), 4u);
  ASSERT_EQ(openstudio::toPath(wi.files.files().at(0).requiredFiles.at(0).first.toLocalFile()), 
      openstudio::toPath("some/random/path/openstudio/radiance/ModelToRad.rb"));

  ASSERT_FALSE(boost::filesystem::exists(wi.files.files().at(0).fullPath));
  ASSERT_FALSE(boost::filesystem::exists(openstudio::toPath(wi.files.files().at(0).requiredFiles.at(0).first.toLocalFile())));

  Workflow wf2;
  (RubyJobBuilder(wi, openstudio::toPath("we/dont/know/original/base/path"), openstudio::toPath("new/path/doesnt/matter"))).addToWorkflow(wf2);
  WorkItem wi2 = wf2.toWorkItems().at(0);
  LOG_FREE(Debug, "RunManagerTestFixture", "workitem 2: " << wi2.toJSON());



  ASSERT_EQ(wi2.files.files().at(0).fullPath, getOpenStudioRubyScriptsPath() / openstudio::toPath("openstudio/radiance/DaylightCalculations.rb"));
  ASSERT_EQ(openstudio::toPath(wi2.files.files().at(0).requiredFiles.at(0).first.toLocalFile()), getOpenStudioRubyScriptsPath() / openstudio::toPath("openstudio/radiance/ModelToRad.rb"));

  ASSERT_TRUE(boost::filesystem::exists(wi2.files.files().at(0).fullPath));
  ASSERT_EQ(wi2.jobkeyname, "testjobkeyname");
  std::vector<std::pair<QUrl, openstudio::path> > requiredFiles = wi2.files.files().at(0).requiredFiles;

  ASSERT_EQ(requiredFiles.size(), 4u);

  for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
      itr != requiredFiles.end();
      ++itr)
  {
    openstudio::path localPath = openstudio::toPath(itr->first.toLocalFile());
    ASSERT_TRUE(boost::filesystem::exists(localPath));
    ASSERT_EQ(localPath.parent_path(), getOpenStudioRubyScriptsPath() / openstudio::toPath("openstudio/radiance"));
  }

}





