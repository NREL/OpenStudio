#ifndef UTILITIES_CORE_LOGGER_I
#define UTILITIES_CORE_LOGGER_I

#if defined(SWIGCSHARP)

  // Avoid triggering a SWIG warning: 'string' is a C# keyword
  %rename(toString) openstudio::StringStreamLogSink::string;

#endif


%{
  #include <utilities/core/LogMessage.hpp>
  #include <utilities/core/LogSink.hpp>
  #include <utilities/core/FileLogSink.hpp>
  #include <utilities/core/StringStreamLogSink.hpp>
  #include <utilities/core/Logger.hpp>
%}

// instantiate templates
%template(OptionalLogLevel) boost::optional<LogLevel>;

%ignore std::vector<openstudio::LogMessage>::vector(size_type);
%ignore std::vector<openstudio::LogMessage>::resize(size_type);
%ignore openstudio::LoggerSingleton::loggerFromChannel;
%ignore openstudio::LogSink::setFormatter;

%template(LogMessageVector) std::vector<openstudio::LogMessage>;
%template(OptionalLogMessage) boost::optional<openstudio::LogMessage>;

// Ignore std::thread::id stuff
%ignore openstudio::LogSink::threadId;
%ignore openstudio::LogSink::setThreadId;
%ignore openstudio::LogSink::resetThreadId;

%include <utilities/core/LogMessage.hpp>
%include <utilities/core/LogSink.hpp>
%include <utilities/core/FileLogSink.hpp>
%include <utilities/core/StringStreamLogSink.hpp>
%include <utilities/core/Logger.hpp>

#endif //UTILITIES_CORE_LOGGER_I
