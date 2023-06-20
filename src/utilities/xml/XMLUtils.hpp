/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_XML_XMLUTILS_HPP
#define UTILITIES_XML_XMLUTILS_HPP

#include <climits>
#include <libxml/globals.h>
#include <libxml/xmlstring.h>
#include <stdexcept>
#include <string>

using xmlError = struct _xmlError;

namespace openstudio {

namespace detail {

  inline const xmlChar* xml_string(const std::string& s) {
    return reinterpret_cast<const xmlChar*>(s.c_str());
  }

  // Cast size_t to int safely, i.e. throw an exception in case of an overflow.
  inline int checked_int_cast(std::size_t len) {
    if (len > INT_MAX) {
      throw std::invalid_argument("data too long");
    }

    return static_cast<int>(len);
  }

  // Cast int to size_t safely, checking that it's non-negative (we assume that
  // size_t is always big enough to contain INT_MAX, which is true for all
  // currently existing architectures).
  inline std::size_t checked_size_t_cast(int len) {
    if (len < 0) {
      throw std::runtime_error("length value unexpectedly negative");
    }

    return static_cast<std::size_t>(len);
  }

  // exception safe wrapper around xmlChar*s that are returned from some
  // of the libxml functions that the user must free.
  class xmlchar_helper
  {
   public:
    xmlchar_helper(xmlChar* ptr) : ptr_(ptr) {}

    ~xmlchar_helper() {
      if (ptr_) {
        xmlFree(ptr_);
      }
    }

    const char* get() const {
      return reinterpret_cast<const char*>(ptr_);
    }

   private:
    xmlChar* ptr_;
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_XML_XMLUTILS_HPP
