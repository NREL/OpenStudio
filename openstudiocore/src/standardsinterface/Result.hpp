/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef STANDARDSINTERFACE_RESULT_HPP
#define STANDARDSINTERFACE_RESULT_HPP

#include <exception>
#include <string>

#include <standardsinterface/StandardsInterfaceAPI.hpp>
#include <boost/optional.hpp>

#include <stdexcept>

namespace openstudio {
namespace standardsinterface {

namespace result {
  /** Describes the Result type. Any Code other than Good indicates that there is no return 
   *  value in the Result object. */
  enum Code { Good, 
              SimulationRequired,
              IncompleteModel,
              ModelError,
              RequestUnmapped, 
              DataDictionaryError};

  struct none {};
} // result

namespace detail {

  class STANDARDSINTERFACE_API Result_Base {
   public:

    Result_Base(result::Code code,const std::string& msg=std::string());

    bool good() const;

    result::Code code() const;

    std::string what() const;

   private:
    // Implicit conversion to bool
    struct Conversion { int unspecified_bool; };

   public:
    typedef int Conversion::*unspecified_bool;
    operator unspecified_bool() const { return good()? &Conversion::unspecified_bool : 0; }

   private:
     result::Code m_code;
     std::string m_message;
  };

} // detail

/** Return type for the AgnosticInterface. Best practice usage, example with double:
 *
 *  \code
 *  AgnosticInterface model;  // invalid code---replace AgnosticInterface with concrete derived
 *                            // class name, such as OpenStudioStandardsInterface
 *  Result<double> result = model.getDoubleAttribute("zone",myZone,"lighting power density")
 *  double value;
 *  if (result) {
 *    value = *result; // or value = result.get();
 *  }
 *  else {
 *    // handle error
 *    result::Code errorCode = result.code();
 *    std::string message = result.what();
 *  }
 *  \endcode
 */
template <class T>
class Result : public detail::Result_Base {
 public:
  Result() : detail::Result_Base(result::RequestUnmapped) {}
  Result(const T& value) : detail::Result_Base(result::Good), m_value(value) {}
  Result(result::Code code, const std::string& msg) 
    : detail::Result_Base(code, msg) {}

  const T& operator*() const {
    if (good()) { return *m_value; }
    else { 
      throw std::runtime_error("Uninitialized openstudio::standardsinterface::Result. Call .code() or .what() for more information."); 
    }
  }

  const T& get() const {
    return this->operator*();
  }

 private:
   boost::optional<T> m_value;
};

/** Return type for functions in the AgnosticInterface that do not return values. Best
 *  practice usage:
 *
 *  \code
 *  AgnosticInterface model;  // invalid b/c AgnosticInterface is abstract class
 *  Result<result::none> result = model.setAttribute(...)
 *  if (!result) {
 *    // handle error
 *    result::Code errCode = result.code();
 *    std::string msg = result.what();
 *  }
 *  \endcode
 */
template<>
class Result<result::none> : public detail::Result_Base {
 public:
  Result(result::Code code=result::Good, const std::string& msg=std::string())
    : detail::Result_Base(code,msg)
  {}
  
  bool get() { return good(); }
};

/** \relates Result */
typedef Result<result::none> VoidResult;

/** \relates Result */
typedef Result<bool>         BoolResult;

/** \relates Result */
typedef Result<int>          IntegerResult;

/** \relates Result */
typedef Result<double>       DoubleResult;

/** \relates Result */
typedef Result<std::string>  StringResult;

} // standardsinterface
} // openstudio

#endif // STANDARDSINTERFACE_RESULT_HPP
