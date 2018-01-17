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
  openstudio::Application::instance().application(false);

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
