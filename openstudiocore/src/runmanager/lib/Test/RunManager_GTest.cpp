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
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>

#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../Workflow.hpp"
#include "../RubyJobUtils.hpp"

#include "../../../ruleset/OSArgument.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"
#include "../../../utilities/filetypes/EpwFile.hpp"

#include <QDir>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

#ifdef _MSC_VER
#include <Windows.h>
#endif

TEST_F(RunManagerTestFixture, EnergyPlusJobRun)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("EnergyPlusJobRun");

  openstudio::path idf = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("5ZoneWarmest.idf"));
  openstudio::path epw = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw"));

  openstudio::runmanager::Job j = openstudio::runmanager::JobFactory::createEnergyPlusJob(
    openstudio::runmanager::ToolInfo(energyPlusExePath()),
    energyPlusIDDPath(),
    idf,
    epw,
    outdir);

  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("EnergyPlusJobRunDB");

  openstudio::runmanager::RunManager kit(db, true);
  kit.enqueue(j, false);

  ASSERT_TRUE(kit.workPending());
  EXPECT_EQ(kit.getJobs().size(), static_cast<size_t>(1));
  ASSERT_TRUE(kit.getJob(j.uuid()) ==  j);

  j.waitForFinished();
  kit.waitForFinished();
  ASSERT_FALSE(kit.workPending());
  ASSERT_FALSE(j.running());
}

TEST_F(RunManagerTestFixture, EnergyPlusJobRunWithNullJobs)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("EnergyPlusJobRunNull");

  openstudio::path idf = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("5ZoneWarmest.idf"));
  openstudio::path epw = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw"));

  openstudio::runmanager::Workflow wf("null->null->null->null->EnergyPlus->null->null->null->null");

  openstudio::runmanager::Tools tools
    = openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(),
          openstudio::path(),
          openstudio::path(),
          openstudio::path(),
          openstudio::path());

  wf.add(tools);
  openstudio::runmanager::Job j = wf.create(outdir, idf, epw);

  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("EnergyPlusJobRunNullDB");

  openstudio::runmanager::RunManager kit(db, true);
  kit.enqueue(j, true);

  ASSERT_TRUE(kit.workPending());
  EXPECT_EQ(kit.getJobs().size(), static_cast<size_t>(9));
  ASSERT_TRUE(kit.getJob(j.uuid()) ==  j);

  j.waitForFinished();
  kit.waitForFinished();
  ASSERT_FALSE(kit.workPending());
  ASSERT_FALSE(j.running());

  openstudio::runmanager::FileInfo sql = j.treeOutputFiles().getLastByExtension("err");
  EXPECT_EQ(sql.filename, "eplusout.err");
  EXPECT_EQ(openstudio::toString(sql.fullPath), openstudio::toString(outdir / openstudio::toPath("Null/Null-0/Null-0/Null-0/EnergyPlus-0/eplusout.err")));
}

TEST_F(RunManagerTestFixture, RubyNoSwap)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("RubyNoSwap");

  openstudio::path idf = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("5ZoneWarmest.idf"));
  openstudio::path perturbScript = openstudio::toPath(rubyLibDir() + "openstudio/runmanager/rubyscripts/PerturbObject.rb");

  openstudio::runmanager::Workflow wf;

  // make a new ruby job
  openstudio::runmanager::RubyJobBuilder rubyJobBuilder;
  rubyJobBuilder.setScriptFile(perturbScript);
  rubyJobBuilder.addScriptParameter("inputPath", "5ZoneWarmest.idf");
  rubyJobBuilder.addScriptParameter("outputPath", "out.idf");
  rubyJobBuilder.addScriptParameter("objectType", "Building");
  rubyJobBuilder.addScriptParameter("nameRegex", ".*");
  rubyJobBuilder.addScriptParameter("field", "0");
  rubyJobBuilder.addScriptParameter("value", "My Building");
  rubyJobBuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
                              openstudio::runmanager::FileSource("All"),
                              ".*\\.idf",
                              "5ZoneWarmest.idf");
  rubyJobBuilder.addToolArgument("-I" + rubyOpenStudioDir()) ;

  // create a dummy workflow and add ruby job
  openstudio::runmanager::Workflow rubyJob;
  rubyJobBuilder.addToWorkflow(wf);

  openstudio::runmanager::Tools tools
    = openstudio::runmanager::ConfigOptions::makeTools(
        energyPlusExePath().parent_path(),
        openstudio::path(),
        openstudio::path(),
        rubyExePath().parent_path(),
        openstudio::path());

  wf.add(tools);

  openstudio::runmanager::Job j = wf.create(outdir, idf, openstudio::path());

  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("RubyNoSwapDB");

  openstudio::runmanager::RunManager kit(db, true);
  kit.enqueue(j, true);

  ASSERT_TRUE(kit.workPending());
  EXPECT_EQ(kit.getJobs().size(), static_cast<size_t>(1));
  ASSERT_TRUE(kit.getJob(j.uuid()) ==  j);

  j.waitForFinished();
  kit.waitForFinished();
  ASSERT_FALSE(kit.workPending());
  ASSERT_FALSE(j.running());

  openstudio::runmanager::FileInfo inputIdf = j.allInputFiles().getLastByFilename("5ZoneWarmest.idf");
  EXPECT_EQ(inputIdf.filename, "5ZoneWarmest.idf");
  EXPECT_EQ(openstudio::toString(inputIdf.fullPath), openstudio::toString(idf));

  openstudio::runmanager::FileInfo outputIdf = j.treeOutputFiles().getLastByFilename("out.idf");
  EXPECT_EQ(outputIdf.filename, "out.idf");
  EXPECT_EQ(openstudio::toString(outputIdf.fullPath), openstudio::toString(outdir / openstudio::toPath("Ruby/out.idf")));
}

TEST_F(RunManagerTestFixture, RubyFromDir)
{
  std::vector<openstudio::runmanager::RubyJobBuilder> jobs
    = openstudio::runmanager::RubyJobBuilder::createRubyJobsFromFolder(
        resourcesPath() / openstudio::toPath("runmanager"),
        std::vector<openstudio::ruleset::UserScriptInfo>(),
      "osm");

  openstudio::runmanager::Workflow wf;

  std::vector<openstudio::path> requiredfiles;
  requiredfiles.push_back(resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("test.xml"));

  openstudio::runmanager::RubyJobBuilder::addJobsToWorkflow(wf, jobs, openstudio::toPath("mypath"), requiredfiles);

  openstudio::runmanager::Job jobtree = wf.create();

  EXPECT_EQ(jobtree.jobType(), openstudio::runmanager::JobType::Ruby);


  std::vector<openstudio::runmanager::FileInfo> files = jobtree.inputFiles();

  ASSERT_EQ(files.size(), 1u);

  std::vector<std::pair<QUrl, openstudio::path> > requiredFiles = files[0].requiredFiles;

  ASSERT_EQ(requiredFiles.size(), 1u);
  EXPECT_EQ(openstudio::toString(requiredFiles[0].second), openstudio::toString(openstudio::toPath("test.xml")));

}

TEST_F(RunManagerTestFixture, RubySwapWithNull)
{
  openstudio::path epw = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw"));
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("RubySwapWithNull");

  openstudio::path idf = (resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("5ZoneWarmest.idf"));
  openstudio::path perturbScript = openstudio::toPath(rubyLibDir() + "openstudio/runmanager/rubyscripts/PerturbObject.rb");

  openstudio::runmanager::Workflow wf;
  wf.addJob(openstudio::runmanager::JobType::Null, "ReplaceMe");

  openstudio::runmanager::Tools tools
    = openstudio::runmanager::ConfigOptions::makeTools(
        energyPlusExePath().parent_path(),
        openstudio::path(),
        openstudio::path(),
        rubyExePath().parent_path(),
        openstudio::path());

  wf.add(tools);

  // make a new ruby job
  openstudio::runmanager::RubyJobBuilder rubyJobBuilder;
  rubyJobBuilder.setScriptFile(perturbScript);
  rubyJobBuilder.addScriptParameter("inputPath", "5ZoneWarmest.idf");
  rubyJobBuilder.addScriptParameter("outputPath", "out.idf");
  rubyJobBuilder.addScriptParameter("objectType", "Building");
  rubyJobBuilder.addScriptParameter("nameRegex", ".*");
  rubyJobBuilder.addScriptParameter("field", "0");
  rubyJobBuilder.addScriptParameter("value", "My Building");
  rubyJobBuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
                              openstudio::runmanager::FileSource("All"),
                              ".*\\.idf",
                              "5ZoneWarmest.idf");
  rubyJobBuilder.addToolArgument("-I" + rubyOpenStudioDir()) ;
  rubyJobBuilder.copyRequiredFiles("idf", "idf", "in.epw");

  // create a dummy workflow and add ruby job
  openstudio::runmanager::Workflow rubyJob;
  rubyJobBuilder.addToWorkflow(rubyJob);

  // swap ruby job with all placeholders using variable name
  wf.replaceJobs("ReplaceMe", rubyJob);

  openstudio::runmanager::Job j = wf.create(outdir, idf, epw);

  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("RubySwapWithNullDB");

  openstudio::runmanager::RunManager kit(db, true);
  kit.enqueue(j, true);

  ASSERT_TRUE(kit.workPending());
  EXPECT_EQ(kit.getJobs().size(), static_cast<size_t>(1));
  ASSERT_TRUE(kit.getJob(j.uuid()) ==  j);

  j.waitForFinished();
  kit.waitForFinished();
  ASSERT_FALSE(kit.workPending());
  ASSERT_FALSE(j.running());

  openstudio::runmanager::FileInfo inputIdf = j.allInputFiles().getLastByFilename("5ZoneWarmest.idf");
  EXPECT_EQ(inputIdf.filename, "5ZoneWarmest.idf");
  EXPECT_EQ(openstudio::toString(inputIdf.fullPath), openstudio::toString(idf));

  openstudio::runmanager::FileInfo outputIdf = j.treeOutputFiles().getLastByFilename("out.idf");
  EXPECT_EQ(outputIdf.filename, "out.idf");
  EXPECT_EQ(openstudio::toString(outputIdf.fullPath), openstudio::toString(outdir / openstudio::toPath("Ruby/out.idf")));

  EXPECT_NO_THROW(outputIdf.getRequiredFile(openstudio::toPath("in.epw")));
}

TEST_F(RunManagerTestFixture, OSMWeatherObjectTest)
{
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("OSMWeatherObjectTest");
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("OSMWeatherObjectTestDB");

  openstudio::runmanager::RunManager kit(db, true);

  // parse the string list with the workflow constructor
  openstudio::runmanager::Workflow workflow("modeltoidf");

  // Build list of tools
  openstudio::runmanager::Tools tools;
  tools.append(kit.getConfigOptions().getTools());
  workflow.add(tools);


  openstudio::path infile = outdir / openstudio::toPath("OSMWeatherObjectTest.osm");
  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager");

  openstudio::model::Model m = openstudio::model::exampleModel();

  openstudio::model::WeatherFile::setWeatherFile(m, openstudio::EpwFile(weatherdir / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw")));

  m.save(infile, true);


  workflow.setInputFiles(infile, weatherdir);

  openstudio::runmanager::Job job = workflow.create(openstudio::tempDir() / openstudio::toPath("OSMWeatherObjectTest"));

  kit.enqueue(job, true);

  kit.waitForFinished();

  std::vector<openstudio::runmanager::FileInfo> files = job.outputFiles();


  ASSERT_EQ(static_cast<size_t>(1), files.size());

  openstudio::runmanager::FileInfo fi = files[0];

  std::pair<QUrl, openstudio::path> epw;
  ASSERT_NO_THROW(epw = fi.getRequiredFile(openstudio::toPath("in.epw")));

  std::string infilename = openstudio::toString(openstudio::toPath(epw.first.toString()).filename() );
  std::string outfilename = openstudio::toString(epw.second);


  EXPECT_EQ("USA_CO_Golden-NREL.724666_TMY3.epw", infilename);
  EXPECT_EQ("in.epw", outfilename);
}

