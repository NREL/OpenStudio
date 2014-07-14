/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
