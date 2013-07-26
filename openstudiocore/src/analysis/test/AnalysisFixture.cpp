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

#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/LinearFunction.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/NormalDistribution.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/OutputAttributeVariable.hpp>
#include <analysis/Problem.hpp>
#include <analysis/RubyMeasure.hpp>
#include <analysis/RubyMeasure_Impl.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/TriangularDistribution.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/Containers.hpp>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;

void AnalysisFixture::SetUp() {}

void AnalysisFixture::TearDown() {}

void AnalysisFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./AnalysisFixture.log"));
  logFile->setLogLevel(Debug);  
  openstudio::Logger::instance().standardOutLogger().disable();

  // set up data folder
  if (!boost::filesystem::exists(toPath("AnalysisFixtureData"))) {
    boost::filesystem::create_directory(toPath("AnalysisFixtureData"));
  }
}

void AnalysisFixture::TearDownTestCase() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> AnalysisFixture::logFile;

openstudio::analysis::Analysis AnalysisFixture::analysis1() {
  // Create problem and analysis
  Problem problem("My Problem");

  BCLMeasure bclMeasure(resourcesPath() / toPath("utilities/BCL/Measures/SetWindowToWallRatioByFacade"));
  RubyMeasure measure(bclMeasure);

  // Measure Group
  StringVector choices;
  choices.push_back("North");
  choices.push_back("South");
  choices.push_back("East");
  choices.push_back("West");
  OSArgument facade = OSArgument::makeChoiceArgument("facade",choices);
  OSArgument arg = facade.clone();
  arg.setValue("South");
  measure.setArgument(arg);
  OSArgument wwr = OSArgument::makeDoubleArgument("wwr");
  MeasureVector measures(1u,NullMeasure());
  measures.push_back(measure.clone().cast<Measure>());
  arg = wwr.clone();
  arg.setValue(0.1);
  measures.back().cast<RubyMeasure>().setArgument(arg);
  measures.push_back(measure.clone().cast<Measure>());
  arg = wwr.clone();
  arg.setValue(0.2);
  measures.back().cast<RubyMeasure>().setArgument(arg);
  measures.push_back(measure.clone().cast<Measure>());
  arg = wwr.clone();
  arg.setValue(0.3);
  measures.back().cast<RubyMeasure>().setArgument(arg);
  problem.push(MeasureGroup("South Windows",measures));

  // Continuous Variables Attached to Arguments
  arg = facade.clone();
  arg.setValue("North");
  measure.setArgument(arg);
  arg = wwr.clone();
  RubyContinuousVariable wwrCV("Window to Wall Ratio",arg,measure);
  wwrCV.setMinimum(0.0);
  wwrCV.setMaximum(1.0);
  TriangularDistribution td(0.2,0.0,0.5);
  wwrCV.setUncertaintyDescription(td);
  problem.push(wwrCV);
  OSArgument offset = OSArgument::makeDoubleArgument("offset");
  RubyContinuousVariable offsetCV("Offset",offset,measure);
  offsetCV.setMinimum(0.0);
  offsetCV.setMaximum(1.5);
  NormalDistribution nd(0.9,0.05);
  offsetCV.setUncertaintyDescription(nd);
  problem.push(offsetCV);

  // Simulation
  problem.push(WorkItem(JobType::ModelToIdf));
  problem.push(WorkItem(JobType::EnergyPlusPreProcess));
  problem.push(WorkItem(JobType::EnergyPlus));
  problem.push(WorkItem(JobType::OpenStudioPostProcess));

  // Responses
  LinearFunction response1("Energy Use Intensity",
                           VariableVector(1u,OutputAttributeVariable("EUI","site.eui")));
  problem.pushResponse(response1);
  VariableVector vars;
  vars.push_back(OutputAttributeVariable("Heating Energy","heating.energy.gas"));
  vars.push_back(OutputAttributeVariable("Cooling Energy","cooling.energy.elec"));
  DoubleVector coeffs;
  coeffs.push_back(1.0); // approx. source factor
  coeffs.push_back(2.5); // approx. source factor
  LinearFunction response2("Approximate Source Energy",vars,coeffs);
  problem.pushResponse(response2);
  LinearFunction response3("North WWR",VariableVector(1u,wwrCV)); // input variable as output
  problem.pushResponse(response3);

  Analysis analysis("My Analysis",problem,FileReferenceType::OSM);

  return analysis;
}
