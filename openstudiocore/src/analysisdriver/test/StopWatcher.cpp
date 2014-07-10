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
