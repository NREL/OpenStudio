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

#include "ReverseTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include "../osversion/VersionTranslator.hpp"

#include <QThread>
#include <iostream>

namespace openstudio {
namespace bimserver {

  ReverseTranslator::ReverseTranslator()
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.bimserver\\.ReverseTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  ReverseTranslator::~ReverseTranslator()
  {
  }

  boost::optional<openstudio::model::Model> ReverseTranslator::loadModel(const std::string osmString, ProgressBar* progressBar)
  {
    m_progressBar = progressBar;

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    // use versiontranslator to load model
    std::stringstream stringStream(osmString);

    openstudio::osversion::VersionTranslator vt;

    return vt.loadModel(stringStream, m_progressBar);
  }

  std::vector<LogMessage> ReverseTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::errors() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  

} // bimserver
} // openstudio
