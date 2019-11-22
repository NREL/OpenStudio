/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "FileLogSink.hpp"
#include "FileLogSink_Impl.hpp"

#include "Assert.hpp"

#include <QReadWriteLock>

namespace openstudio{

  namespace detail{

    FileLogSink_Impl::FileLogSink_Impl(const openstudio::path& path)
      : m_path(path)
    {
      m_ofs = boost::shared_ptr<openstudio::filesystem::ofstream>(new openstudio::filesystem::ofstream(path));
      this->setStream(m_ofs);
      this->enable();
    }

    FileLogSink_Impl::~FileLogSink_Impl()
    {
      // already called
      //LogSink_Impl::~LogSink_Impl();
    }

    openstudio::path FileLogSink_Impl::path() const
    {
      QReadLocker l(m_mutex);

      return m_path;
    }

    std::vector<LogMessage> FileLogSink_Impl::logMessages() const
    {
      openstudio::filesystem::ifstream ifs(m_path);
      std::string line;
      std::string text;
      while(std::getline(ifs, line)){
        text += line + "\n";
      }
      return LogMessage::parseLogText(text);
    }
  } // detail

  FileLogSink::FileLogSink(const openstudio::path& path)
    : LogSink(boost::shared_ptr<detail::FileLogSink_Impl>(new detail::FileLogSink_Impl(path)))
  {
    OS_ASSERT(getImpl<detail::FileLogSink_Impl>());
  }

  openstudio::path FileLogSink::path() const
  {
    return this->getImpl<detail::FileLogSink_Impl>()->path();
  }

  std::vector<LogMessage> FileLogSink::logMessages() const
  {
    return this->getImpl<detail::FileLogSink_Impl>()->logMessages();
  }

} // openstudio
