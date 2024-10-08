/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_STRINGSTREAMLOGSINK_IMPL_HPP
#define UTILITIES_CORE_STRINGSTREAMLOGSINK_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "LogSink_Impl.hpp"

namespace openstudio {

namespace detail {

  class UTILITIES_API StringStreamLogSink_Impl : public LogSink_Impl
  {
   public:
    /// constructor makes a new string stream to write to and registers in the global logger
    StringStreamLogSink_Impl();

    /// destructor, disables log sink
    virtual ~StringStreamLogSink_Impl() override;

    /// get the string stream's content
    std::string string() const;

    /// get messages out of the string stream's content
    std::vector<LogMessage> logMessages() const;

    /// reset the string stream's content
    void resetStringStream();

   private:
    boost::shared_ptr<std::stringstream> m_stringstream;
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_CORE_STRINGSTREAMLOGSINK_IMPL_HPP
