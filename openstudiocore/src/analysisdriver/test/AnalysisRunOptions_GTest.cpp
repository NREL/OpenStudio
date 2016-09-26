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

