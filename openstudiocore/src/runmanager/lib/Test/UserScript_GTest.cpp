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
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <runmanager/lib/JobFactory.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/LocalProcessCreator.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <model/Model.hpp>
#include <model/WeatherFile.hpp>

#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObject.hpp>
#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>


#include <utilities/idd/OS_TimeDependentValuation_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <boost/filesystem/path.hpp>

#include <ruleset/OSArgument.hpp>

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
  ASSERT_EQ(3u, outfiles.size());
  
  openstudio::runmanager::FileInfo fi2 = outfiles[0];
  //Make sure epw got copied from input to output
  EXPECT_EQ("out.osm", fi2.filename);
  ASSERT_EQ(1u, fi2.requiredFiles.size());
  ASSERT_EQ("in.epw", openstudio::toString(fi2.requiredFiles[0].second));

}
