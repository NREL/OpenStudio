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

#ifndef ANALYSISDRIVER_TEST_STOPWATCHER_HPP
#define ANALYSISDRIVER_TEST_STOPWATCHER_HPP

#include <gtest/gtest.h>

#include "../AnalysisDriverWatcher.hpp"

#include "../../utilities/time/Time.hpp"

class StopWatcher : public openstudio::analysisdriver::AnalysisDriverWatcher {
 public:
  StopWatcher(const openstudio::analysisdriver::AnalysisDriver& analysisDriver, 
              int stopNum=2);

  virtual ~StopWatcher() {}

  virtual void onDataPointComplete(const openstudio::UUID &dataPoint);

  int stopNum() const;

  int nComplete() const;

  openstudio::Time stoppingTime() const;

 private:
  int m_stopNum;
  int m_nComplete;
  openstudio::Time m_stoppingTime;
};

#endif // ANALYSISDRIVER_TEST_STOPWATCHER_HPP
