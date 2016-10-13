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

#include "StopWatcher.hpp"
#include "../CurrentAnalysis.hpp"

using namespace openstudio;
using namespace openstudio::analysisdriver;

StopWatcher::StopWatcher(const AnalysisDriver& analysisDriver, int stopNum)
  : AnalysisDriverWatcher(analysisDriver),
    m_stopNum(stopNum),
    m_nComplete(0),
    m_stoppingTime(0,0,0,0)
{}

void StopWatcher::onDataPointComplete(const UUID &dataPoint) {
  ++m_nComplete;
  if (m_nComplete == m_stopNum) {
    AnalysisDriver driver = analysisDriver();
    CurrentAnalysisVector currentAnalyses = driver.currentAnalyses();
    ASSERT_FALSE(currentAnalyses.empty());
    EXPECT_EQ(1u,currentAnalyses.size());
    CurrentAnalysis currentAnalysis = currentAnalyses[0];
    openstudio::Time start = openstudio::Time::currentTime();
    driver.stop(currentAnalysis);
    m_stoppingTime = openstudio::Time::currentTime() - start;
  }
}

int StopWatcher::stopNum() const {
  return m_stopNum;
}

int StopWatcher::nComplete() const {
  return m_nComplete;
}

openstudio::Time StopWatcher::stoppingTime() const {
  return m_stoppingTime;
}
