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

#ifndef UTILITIES_CORE_SERIALIZATION_HPP
#define UTILITIES_CORE_SERIALIZATION_HPP

// disable warning in serialization code
#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

// there are bug reports that you have to include the archives
// before you include serialization
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/tracking.hpp>

#ifdef WIN32
#pragma warning( pop )
#endif

#include <boost/config.hpp>
#include <boost/filesystem/path.hpp>

/** MAKE_PTR_SERIALIZABLE(T) macro is placed inside a publically visiable portion of class T.
*   The macro provides serialization routines.  The class must inherit from SharedFromThis<T>
*   in Macro.h.
*
*   Example usage:
*   class Model : public ModelObject{
*   public:
*     MAKE_PTR_SERIALIZABLE(Model)
* ...
*/
#define MAKE_PTR_SERIALIZABLE(__T__) \
friend class boost::serialization::access; \
virtual std::ostream& toText(std::ostream& os) const { \
  boost::archive::text_oarchive out(os); \
  ConstPtr ptr = boost::dynamic_pointer_cast<const __T__>(share()); \
  out << ptr; \
  return os; \
} \
virtual std::string toText() const { std::stringstream os; toText(os); return os.str();} \
virtual std::ostream& toXml(std::ostream& os) const { \
  boost::archive::xml_oarchive out(os); \
  ConstPtr ptr = boost::dynamic_pointer_cast<const __T__>(share()); \
  out << boost::serialization::make_nvp(#__T__, ptr); \
  return os; \
} \
virtual std::string toXml() const { std::stringstream os; toXml(os); return os.str();} \
virtual std::ostream& toBinary(std::ostream& os) const { \
  boost::archive::binary_oarchive out(os); \
  ConstPtr ptr = boost::dynamic_pointer_cast<const __T__>(share()); \
  out << ptr; \
  return os; \
} \
virtual std::string toBinary() const { std::stringstream os; toBinary(os); return os.str();} \
static Ptr fromText(std::istream& is){ \
  Ptr object; \
  try{ \
    boost::archive::text_iarchive in(is); \
    in >> object; \
  }catch(...){ \
  } \
  return object; \
} \
static Ptr fromText(const std::string& str) {std::stringstream ss; ss << str; return fromText(ss);} \
static Ptr fromXml(std::istream& is){ \
  Ptr object; \
  try{ \
    boost::archive::xml_iarchive in(is); \
    in >> boost::serialization::make_nvp(#__T__, object); \
  }catch(...){ \
  } \
  return object; \
} \
static Ptr fromXml(const std::string& str) {std::stringstream ss; ss << str; return fromXml(ss);} \
static Ptr fromBinary(std::istream& is){ \
  Ptr object; \
  try{ \
    boost::archive::binary_iarchive in(is); \
    in >> object; \
  }catch(...){ \
  } \
  return object; \
} \
static Ptr fromBinary(const std::string& str) {std::stringstream ss; ss << str; return fromBinary(ss);} \


/** MAKE_POD_SERIALIZABLE(T) macro is placed inside a publically visiable portion of class T.
*   The macro provides serialization routines.
*
*   Example usage:
*   class Model : public ModelObject{
*   public:
*     MAKE_POD_SERIALIZABLE(Model)
* ...
*/
#define MAKE_POD_SERIALIZABLE(__T__) \
friend class boost::serialization::access; \
virtual std::ostream& toText(std::ostream& os) const{ \
  boost::archive::text_oarchive out(os); \
  out << *this; \
  return os; \
} \
virtual std::string toText() const{ std::stringstream os; toText(os); return os.str();} \
virtual std::ostream& toXml(std::ostream& os) const{ \
  boost::archive::xml_oarchive out(os); \
  out << boost::serialization::make_nvp(#__T__, *this); \
  return os; \
} \
virtual std::string toXml() const{ std::stringstream os; toXml(os); return os.str();} \
virtual std::ostream& toBinary(std::ostream& os) const{ \
  boost::archive::binary_oarchive out(os); \
  out << *this; \
  return os; \
} \
virtual std::string toBinary() const{ std::stringstream os; toBinary(os); return os.str();} \
static __T__ fromText(std::istream& is){ \
  __T__ object; \
  try{ \
    boost::archive::text_iarchive in(is); \
    in >> object; \
  }catch(...){ \
  } \
  return object; \
} \
static __T__ fromText(const std::string& str) {std::stringstream ss; ss << str; return fromText(ss);} \
static __T__ fromXml(std::istream& is){ \
  __T__ object; \
  try{ \
    boost::archive::xml_iarchive in(is); \
    in >> boost::serialization::make_nvp(#__T__, object); \
  }catch(...){ \
  } \
  return object; \
} \
static __T__ fromXml(const std::string& str) {std::stringstream ss; ss << str; return fromXml(ss);} \
static __T__ fromBinary(std::istream& is){ \
  __T__ object; \
  try{ \
    boost::archive::binary_iarchive in(is); \
    in >> object; \
  }catch(...){ \
  } \
  return object; \
} \
static __T__ fromBinary(const std::string& str) {std::stringstream ss; ss << str; return fromBinary(ss);} \


#endif // UTILITIES_CORE_SERIALIZATION_HPP
