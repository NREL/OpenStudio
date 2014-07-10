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
