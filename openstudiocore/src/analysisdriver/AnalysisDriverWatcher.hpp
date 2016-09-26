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

#ifndef ANALYSISDRIVER_ANALYSISDRIVERWATCHER_HPP
#define ANALYSISDRIVER_ANALYSISDRIVERWATCHER_HPP

#include "AnalysisDriverAPI.hpp"
#include "AnalysisDriver.hpp"

#include "../utilities/core/UUID.hpp"
#include "../utilities/core/Logger.hpp"

#include <QObject>

#include <vector>

namespace openstudio {
namespace analysis {
  class Analysis;
  class DataPoint;
}

namespace analysisdriver {

/** AnalysisDriverWatcher is a base class with virtual methods that are called when certain signals
 *  from the watched AnalysisDriver are emitted. C++ and Ruby classes can be derived from 
 *  AnalysisDriverWatcher and thereby used to provide user-custom functionalities that execute at 
 *  specific points in time during the course of running an analysis. */
class ANALYSISDRIVER_API AnalysisDriverWatcher : public QObject {
  Q_OBJECT;
 public:
  /** @name Constructors and Destructors */
  //@{

  AnalysisDriverWatcher(const AnalysisDriver& analysisDriver);

  virtual ~AnalysisDriverWatcher() {}

  //@}
  /** @name Actions */
  //@{

  /** Must be called to specify that analysis should be watched. */
  void watch(const openstudio::UUID& analysis);

  /** Method that is called whenever the AnalysisDriver emits a dataPointQueued signal for
   *  an analysis that is being watched. Base class implementation does nothing. */
  virtual void onDataPointQueued(const openstudio::UUID& dataPoint);

  /** Method that is called whenever the AnalysisDriver emits a dataPointComplete signal for
   *  an analysis that is being watched. Base class implementation does nothing. */
  virtual void onDataPointComplete(const openstudio::UUID& dataPoint);

  /** Method that is called whenever the AnalysisDriver emits an AnalysisComplete signal for
   *  an analysis that is being watched. Base class implementation does nothing. */
  virtual void onAnalysisComplete(const openstudio::UUID& analysis);

  //@}
  /** @name Helper Methods */
  //@{

  /** Method to help derived classes get the analysis::DataPoint that corresponds to the 
   *  dataPoint UUID. */
  openstudio::analysis::DataPoint getDataPoint(const openstudio::UUID& dataPoint) const;

  /** Method to help derived classes get the analysis::Analysis that corresponds to the 
   *  analysis UUID. */
  openstudio::analysis::Analysis getAnalysis(const openstudio::UUID& analysis) const;

  //@}
 public slots:

  void dataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

  void dataPointComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

  void analysisComplete(const openstudio::UUID& analysis);

 protected:
  AnalysisDriver analysisDriver() const;

 private:
  AnalysisDriver m_analysisDriver;
  std::vector<openstudio::UUID> m_analyses;

  REGISTER_LOGGER("openstudio.analysisdriver.AnalysisDriverWatcher");

  bool watching(const openstudio::UUID& analysis) const;
};

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_ANALYSISDRIVERWATCHER_HPP
