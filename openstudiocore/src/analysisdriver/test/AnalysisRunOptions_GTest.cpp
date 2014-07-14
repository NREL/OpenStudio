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
#include "AnalysisDriverFixture.hpp"

#include "../AnalysisDriver.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../project/ProjectDatabase.hpp"

#include <runmanager/Test/ToolBin.hxx>

#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::analysisdriver;

std::pair<int,int> numParamsResultsFiles(const openstudio::path& dakotaDir) {
  return std::pair<int,int>(0,0);
}

TEST_F(AnalysisDriverFixture,AnalysisRunOptions_DakotaFileSave_True) {
  // run a small analysis with dakota. verify that the params.in and results.out files
  // are there after completion


}

TEST_F(AnalysisDriverFixture,AnalysisRunOptions_DakotaFileSave_False) {
  // run a small analysis with dakota. verify that there are no params.in and results.out files
  // there after completion

}

TEST_F(AnalysisDriverFixture,AnalysisRunOptions_QueueSize_NoLimit_SequentialSearch) {
  // run a small analysis and verify that it completes successfully
}

TEST_F(AnalysisDriverFixture,AnalysisRunOptions_QueueSize_Limited_SequentialSearch) {
  // run a small analysis with a much reduced queue size and verify that it completes 
  // successfully
}

TEST_F(AnalysisDriverFixture,AnalysisRunOptions_QueueSize_NoLimit_Dakota) {
  // run a small analysis with dakota with dakotaFileSave == false. during runtime, verify that
  // at some point the params.in value returned by numParamsResultsFiles equals the total number
  // of samples

}

TEST_F(AnalysisDriverFixture,AnalysisRunOptions_QueueSize_Limited_Dakota) {
  // run a small analysis with dakota with dakotaFileSave == false. during runtime, verify that
  // numParamsResultsFiles never exceed the queue limit

}

