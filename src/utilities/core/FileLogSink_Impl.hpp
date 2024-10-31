/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_FILELOGSINK_IMPL_HPP
#define UTILITIES_CORE_FILELOGSINK_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "LogSink_Impl.hpp"
#include "FileLogSink.hpp"

namespace openstudio {

namespace detail {

  class UTILITIES_API FileLogSink_Impl : public LogSink_Impl
  {
   public:
    /// constructor takes path of file, opens in write mode positioned at file beginning
    /// and registers in the global logger
    FileLogSink_Impl(const openstudio::path& path);

    /// destructor, does not disable log sink
    virtual ~FileLogSink_Impl() override;

    /// returns the path that log messages are written to
    openstudio::path path() const;

    /// get messages out of the file content
    std::vector<LogMessage> logMessages() const;

   private:
    openstudio::path m_path;
    boost::shared_ptr<openstudio::filesystem::ofstream> m_ofs;
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_CORE_FILELOGSINK_IMPL_HPP
