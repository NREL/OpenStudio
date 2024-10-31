/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "XMLErrors.hpp"

#include <libxml/xmlerror.h>
#include "../core/LogMessage.hpp"
#include "../core/ASCIIStrings.hpp"

#include <cstdarg>  // For va_list
#include <cstdio>   // std::vsnprintf
#include <vector>

namespace openstudio {

namespace detail {

  std::string build_structured_message(const std::string& levelName, const xmlError& error) {
    std::ostringstream oss;

    // We currently don't decode domain and code to their symbolic
    // representation as it doesn't seem to be worth it in practice, the error
    // message is usually clear enough, while these numbers can be used for
    // automatic classification of messages.

    // Message is '\n' terminated, so remove that
    std::string message(error.message);
    ascii_trim(message);

    oss << "XML " << levelName << " " << error.domain << "." << error.code << ": " << message;

    if (error.file) {
      oss << "\n    at " << error.file;
      if (error.line > 0) {
        oss << ":" << error.line;

        // Column information, if available, is passed in the second int
        // field (first one is used with the first string field, see below).
        if (error.int2 > 0) {
          oss << "," << error.int2;
        }
      }
    }

    if (error.str1) {
      oss << " while processing \"" << error.str1 << "\"";
      if (error.int1 > 0) {
        oss << " at position " << error.int1;
      }
    }

    return oss.str();
  }

  // cppcheck-suppress constParameterPointer
  void callback_messages_structured_error(void* errorCollectorVoidPtr, xmlError* error) {
    // This shouldn't happen, but better be safe than sorry
    if (!error) {
      return;
    }

    if (error->message) {
      LogLevel level = LogLevel::Trace;
      std::string levelName;
      if (error->level == XML_ERR_NONE) {
        levelName = "None";
      } else if (error->level == XML_ERR_WARNING) {
        // Some libxml warnings are pretty fatal errors, e.g. failing
        // to open the input file is reported as a warning with code
        // XML_IO_LOAD_ERROR, so treat them as such.
        if (error->code == XML_IO_LOAD_ERROR) {
          level = LogLevel::Error;
          levelName = "error";
        } else {
          level = LogLevel::Warn;
          levelName = "warning";
        }

      } else if (error->level == XML_ERR_ERROR) {
        level = LogLevel::Error;
        levelName = "error";
      } else if (error->level == XML_ERR_FATAL) {
        level = LogLevel::Fatal;
        levelName = "fatal error";
      }

      auto* errorCollector = static_cast<ErrorCollector*>(errorCollectorVoidPtr);
      errorCollector->logMessages.emplace_back(level, "XMLValidator", build_structured_message(levelName, *error));
    }
  }

  std::string get_string_from_va(const char* fmt, va_list args) {
    // Make a second list. We calc std::vsnprintf with a buf_size of 0, so nothing is written but the number of bytes that would be written **not**
    // including the null terminator is still calculated and returned
    va_list args_for_size_calc;
    va_copy(args_for_size_calc, args);
    const int nchars = std::vsnprintf(nullptr, 0, fmt, args_for_size_calc) + 1;
    va_end(args_for_size_calc);

    std::vector<char> buf(nchars);
    std::vsnprintf(buf.data(), buf.size(), fmt, args);

    return std::string(buf.begin(), buf.end());
  }

  void callback_messages_warning(void* errorCollectorVoidPtr, const char* fmt, ...) {
    // cf: https://en.cppreference.com/w/cpp/io/c/vfprintf

    // Make a va_list
    va_list args;
    // Setup args by using the `fmt` as the last know explicit argument
    // The libxml2 documentation is lacking, but the first argument is a format-string, the rest of the va_args are the values to put in there
    va_start(args, fmt);

    std::string message = get_string_from_va(fmt, args);
    va_end(args);

    auto* errorCollector = static_cast<ErrorCollector*>(errorCollectorVoidPtr);
    errorCollector->logMessages.emplace_back(LogLevel::Warn, "XMLValidator", message);
  }

  void callback_messages_error(void* errorCollectorVoidPtr, const char* fmt, ...) {
    // cf: https://en.cppreference.com/w/cpp/io/c/vfprintf

    // Make a va_list
    va_list args;
    // Setup args by using the `fmt` as the last know explicit argument
    // The libxml2 documentation is lacking, but the first argument is a format-string, the rest of the va_args are the values to put in there
    va_start(args, fmt);

    std::string message = get_string_from_va(fmt, args);
    va_end(args);

    auto* errorCollector = static_cast<ErrorCollector*>(errorCollectorVoidPtr);
    errorCollector->logMessages.emplace_back(LogLevel::Error, "XMLValidator", message);
  }

}  // namespace detail

}  // namespace openstudio
