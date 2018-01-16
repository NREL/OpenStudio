/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "StringStreamLogSink.hpp"
#include "StringStreamLogSink_Impl.hpp"

#include "Assert.hpp"

#include <boost/lexical_cast.hpp>

#include <sstream>

#include <QReadWriteLock>
#include <QWriteLocker>

namespace openstudio{

  namespace detail{

    StringStreamLogSink_Impl::StringStreamLogSink_Impl()
      : m_stringstream(new std::stringstream)
    {
      this->setStream(m_stringstream);
      this->enable();
    }

    StringStreamLogSink_Impl::~StringStreamLogSink_Impl()
    {
      this->disable();

      // already called
      //LogSink_Impl::~LogSink_Impl();
    }

    std::string StringStreamLogSink_Impl::string() const
    {
      QReadLocker l(m_mutex);

      return m_stringstream->str();
    }

    std::vector<LogMessage> StringStreamLogSink_Impl::logMessages() const
    {
      return LogMessage::parseLogText(this->string());
    }

    void StringStreamLogSink_Impl::resetStringStream()
    {
      QWriteLocker l(m_mutex);

      m_stringstream->str("");
    }

  }

  StringStreamLogSink::StringStreamLogSink()
    : LogSink(boost::shared_ptr<detail::StringStreamLogSink_Impl>(new detail::StringStreamLogSink_Impl()))
  {
    OS_ASSERT(getImpl<detail::StringStreamLogSink_Impl>());
  }

  std::string StringStreamLogSink::string() const
  {
    return this->getImpl<detail::StringStreamLogSink_Impl>()->string();
  }

  std::vector<LogMessage> StringStreamLogSink::logMessages() const
  {
    return this->getImpl<detail::StringStreamLogSink_Impl>()->logMessages();
  }

  void StringStreamLogSink::resetStringStream()
  {
    this->getImpl<detail::StringStreamLogSink_Impl>()->resetStringStream();
  }

} // openstudio
