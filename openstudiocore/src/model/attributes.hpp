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

#ifndef MODEL_ATTRIBUTES_HPP
#define MODEL_ATTRIBUTES_HPP

#include <boost/preprocessor/control/if.hpp>
#include <boost/optional.hpp>
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

//ATTRIBUTE_DEFINITION(IS_OPTIONAL,IS_AUTOSIZEABLE,HAS_DEFAULT,ATTRIBUTE_NAME,ACCESSOR_BASE_NAME)
//ATTRIBUTE_IMPLEMENTATION(IS_OPTIONAL,HAS_DEFAULT,IS_AUTOSIZEABLE,
//                         ATTRIBUTE_NAME,ACCESSOR_BASE_NAME,
//                         CLASS_NAME,
//                         DEFAULT_IMPLEMENTATIOIN,IDD_OBJECT_NAME,IDD_FIELD_NAME)

// ATTRIBUTE_DEFINITION

#define ATTRIBUTE_DEFINITION(IS_OPTIONAL,HAS_DEFAULT,IS_AUTOSIZEABLE,ATTRIBUTE_NAME,ACCESSOR_BASE_NAME) \
\
BOOST_PP_IF(BOOST_PP_OR(IS_OPTIONAL,IS_AUTOSIZEABLE), \
\
bool set##ACCESSOR_BASE_NAME(boost::optional<double> value); \
bool set##ACCESSOR_BASE_NAME(const OSOptionalQuantity & value); \
OSOptionalQuantity ATTRIBUTE_NAME##_SI() const; \
OSOptionalQuantity ATTRIBUTE_NAME##_IP() const; \
OSOptionalQuantity get##ACCESSOR_BASE_NAME(bool returnIP = false) const; \
boost::optional<double> ATTRIBUTE_NAME() const; \
BOOST_PP_IF(HAS_DEFAULT, \
void reset##ACCESSOR_BASE_NAME(); \
bool is##ACCESSOR_BASE_NAME##Defaulted() const; \
,) \
BOOST_PP_IF(IS_AUTOSIZEABLE, \
void autosize##ACCESSOR_BASE_NAME(); \
bool is##ACCESSOR_BASE_NAME##Autosized() const; \
,) \
,\
bool set##ACCESSOR_BASE_NAME(double value); \
bool set##ACCESSOR_BASE_NAME(const Quantity & value); \
Quantity ATTRIBUTE_NAME##_SI() const; \
Quantity ATTRIBUTE_NAME##_IP() const; \
Quantity get##ACCESSOR_BASE_NAME(bool returnIP = false) const; \
double ATTRIBUTE_NAME() const; \
BOOST_PP_IF(HAS_DEFAULT, \
void reset##ACCESSOR_BASE_NAME(); \
bool is##ACCESSOR_BASE_NAME##Defaulted() const; \
,) \
)

// ATTRIBUTE_IMPLEMENTATION

#define ATTRIBUTE_IMPLEMENTATION(IS_OPTIONAL,HAS_DEFAULT,IS_AUTOSIZEABLE, \
                                 ATTRIBUTE_NAME,ACCESSOR_BASE_NAME, \
                                 CLASS_NAME, \
                                 DEFAULT_IMPLEMENTATIOIN,IDD_OBJECT_NAME,IDD_FIELD_NAME) \
\
BOOST_PP_IF(BOOST_PP_OR(IS_OPTIONAL,IS_AUTOSIZEABLE), \
\
OSOptionalQuantity CLASS_NAME##_Impl::ATTRIBUTE_NAME##_SI() const { \
  return get##ACCESSOR_BASE_NAME(false); \
} \
\
OSOptionalQuantity CLASS_NAME##_Impl::ATTRIBUTE_NAME##_IP() const { \
  return get##ACCESSOR_BASE_NAME(true); \
} \
\
OSOptionalQuantity CLASS_NAME##_Impl::get##ACCESSOR_BASE_NAME(bool returnIP) const { \
  OptionalDouble value = ATTRIBUTE_NAME(); \
  return getQuantityFromDouble(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, value, returnIP); \
} \
\
bool CLASS_NAME##_Impl::set##ACCESSOR_BASE_NAME(const OSOptionalQuantity& quantity) { \
  bool result(false); \
  OptionalDouble value; \
  if (quantity.isSet()) { \
    value = getDoubleFromQuantity(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME,quantity.get()); \
    if (value) { \
      result = set##ACCESSOR_BASE_NAME(value); \
    } \
  } \
  else { \
    result = set##ACCESSOR_BASE_NAME(value); \
  } \
  return result; \
} \
\
BOOST_PP_IF(DEFAULT_IMPLEMENTATIOIN, \
void CLASS_NAME##_Impl::set##ACCESSOR_BASE_NAME(boost::optional<double> value) { \
  bool result = false; \
  if (value) { \
    result = setDouble(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, value.get()); \
  } else { \
    result = setString(IDD_FIELD_NAME##Fields::IDD_FIELD_NAME, ""); \
  } \
  OS_ASSERT(result); \
} \
\
boost::optional<double> CLASS_NAME##_Impl::ATTRIBUTE_NAME() const { \
  OptionalDouble value = getDouble(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, true); \
  return value; \
} \
,)\
\
BOOST_PP_IF(HAS_DEFAULT, \
void CLASS_NAME##_Impl::reset##ATTRIBUTE_NAME() { \
  bool result = setString(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, ""); \
  OS_ASSERT(result); \
} \
,) \
BOOST_PP_IF(IS_AUTOSIZEABLE, \
bool CLASS_NAME##_Impl::is##ACCESSOR_BASE_NAME##Autosized() const { \
  bool result = false; \
  boost::optional<std::string> value = getString(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, true); \
  if (value) { \
    result = openstudio::istringEqual(value.get(), "autosize"); \
  } \
  return result; \
} \
void CLASS_NAME##_Impl::autosize##ACCESSOR_BASE_NAME() { \
  bool result = setString(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, "autosize"); \
  OS_ASSERT(result); \
} \
,) \
,\
Quantity CLASS_NAME##_Impl::get##ACCESSOR_BASE_NAME(bool returnIP) const { \
  OptionalDouble value = ATTRIBUTE_NAME(); \
  OSOptionalQuantity result = getQuantityFromDouble(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, value, returnIP); \
  OS_ASSERT(result.isSet()); \
  return result.get(); \
} \
\
bool CLASS_NAME##_Impl::set##ACCESSOR_BASE_NAME(const Quantity& quantity) { \
  OptionalDouble value = getDoubleFromQuantity(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME,quantity); \
  if (!value) { \
    return false; \
  } \
  return set##ACCESSOR_BASE_NAME(value.get()); \
} \
\
Quantity CLASS_NAME##_Impl::ATTRIBUTE_NAME##_SI() const { \
  return get##ACCESSOR_BASE_NAME(false); \
} \
\
Quantity CLASS_NAME##_Impl::ATTRIBUTE_NAME##_IP() const { \
  return get##ACCESSOR_BASE_NAME(true); \
} \
\
BOOST_PP_IF(DEFAULT_IMPLEMENTATIOIN, \
double CLASS_NAME##_Impl::ATTRIBUTE_NAME() const { \
  OptionalDouble value = getDouble(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, true); \
  OS_ASSERT(value); \
  return value.get(); \
} \
\
bool CLASS_NAME##_Impl::set##ACCESSOR_BASE_NAME(double value) { \
  bool result = false; \
  result = setDouble(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, value); \
  return result; \
} \
, ) \
\
BOOST_PP_IF(HAS_DEFAULT, \
void CLASS_NAME##_Impl::reset##ACCESSOR_BASE_NAME() { \
  bool result = setString(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME, ""); \
  OS_ASSERT(result); \
} \
bool CLASS_NAME##_Impl::is##ACCESSOR_BASE_NAME##Defaulted() const { \
  return isEmpty(IDD_OBJECT_NAME##Fields::IDD_FIELD_NAME); \
} \
,) \
)

#endif // MODEL_ATTRIBUTES_HPP
