/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
