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

#include "AnalysisDriverWatcher.hpp"
#include "AnalysisDriver_Impl.hpp"

#include "../project/ProjectDatabase.hpp"
#include "../project/AnalysisRecord.hpp"
#include "../project/AnalysisRecord_Impl.hpp"
#include "../project/DataPointRecord.hpp"
#include "../project/DataPointRecord_Impl.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/System.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysisdriver {

AnalysisDriverWatcher::AnalysisDriverWatcher(const AnalysisDriver& analysisDriver)
 : m_analysisDriver(analysisDriver)
{
  LOG(Debug,"Constructing AnalysisDriverWatcher.");
  // make sure a QApplication exists
  openstudio::Application::instance().application();

  // connect signals and slots
  bool ok;
  ok = m_analysisDriver.connect(SIGNAL(dataPointQueued(const openstudio::UUID&,const openstudio::UUID&)),
                                this,
                                SLOT(dataPointQueued(const openstudio::UUID&,const openstudio::UUID&)));
  OS_ASSERT(ok);
  ok = m_analysisDriver.connect(SIGNAL(dataPointComplete(const openstudio::UUID&,const openstudio::UUID&)),
                                this,
                                SLOT(dataPointComplete(const openstudio::UUID&,const openstudio::UUID&)));
  OS_ASSERT(ok);
  ok = m_analysisDriver.connect(SIGNAL(analysisComplete(const openstudio::UUID&)),
                                this,
                                SLOT(analysisComplete(const openstudio::UUID&)));
  OS_ASSERT(ok);
}

void AnalysisDriverWatcher::watch(const openstudio::UUID& analysis) {
  // register uuid
  m_analyses.push_back(analysis);
}

void AnalysisDriverWatcher::onDataPointQueued(const openstudio::UUID& dataPoint) {
  LOG(Debug,"Base class onDataPointQueued.");
}

void AnalysisDriverWatcher::onDataPointComplete(const openstudio::UUID& dataPoint) {
  LOG(Debug,"Base class onDataPointComplete.");
}

void AnalysisDriverWatcher::onAnalysisComplete(const openstudio::UUID& analysis) {
  LOG(Debug,"Base class onAnalysisComplete.");
}

void AnalysisDriverWatcher::dataPointQueued(const openstudio::UUID& analysis,
    const openstudio::UUID& dataPoint)
{
  try {
    if (watching(analysis)) {
      this->onDataPointQueued(dataPoint);
    }
  } catch (const std::exception &e) {
    LOG(Error, "Error while processing dataPointQueued: " << e.what());
  }
}

void AnalysisDriverWatcher::dataPointComplete(const openstudio::UUID& analysis,
    const openstudio::UUID& dataPoint)
{
  try {
    LOG(Debug,"Received dataPointComplete signal.");
    if (watching(analysis)) {
      LOG(Debug,"Calling onDataPointComplete.");
      this->onDataPointComplete(dataPoint);
    }
  } catch (const std::exception &e) {
    LOG(Error, "Error while processing dataPointComplete: " << e.what());
  }
}

void AnalysisDriverWatcher::analysisComplete(const openstudio::UUID& analysis) {
  try {
    LOG(Debug,"Received analysisComplete signal.");
    if (watching(analysis)) {
      LOG(Debug,"Calling on AnalysisComplete.");
      this->onAnalysisComplete(analysis);
      auto it = std::find(m_analyses.begin(),m_analyses.end(),analysis);
      m_analyses.erase(it);
    }
  } catch (const std::exception &e) {
    LOG(Error, "Error while processing analysisComplete: " << e.what());
  }
}

openstudio::analysis::DataPoint AnalysisDriverWatcher::getDataPoint(const openstudio::UUID& dataPoint) const {
  return m_analysisDriver.database().getObjectRecordByHandle<project::DataPointRecord>(dataPoint).get().dataPoint();
}

openstudio::analysis::Analysis AnalysisDriverWatcher::getAnalysis(const openstudio::UUID& analysis) const {
  return m_analysisDriver.database().getObjectRecordByHandle<project::AnalysisRecord>(analysis).get().analysis();
}

AnalysisDriver AnalysisDriverWatcher::analysisDriver() const {
  return m_analysisDriver;
}

bool AnalysisDriverWatcher::watching(const openstudio::UUID& analysis) const {
  return (std::find(m_analyses.begin(),m_analyses.end(),analysis) != m_analyses.end());
}

} // analysisdriver
} // openstudio
