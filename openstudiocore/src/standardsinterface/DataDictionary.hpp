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

#ifndef STANDARDSINTERFACE_DATADICTIONARY_HPP
#define STANDARDSINTERFACE_DATADICTIONARY_HPP

#include <standardsinterface/StandardsInterfaceAPI.hpp>

#include <standardsinterface/ObjectType.hxx>
#include <standardsinterface/BooleanAttribute.hxx>
#include <standardsinterface/IntegerAttribute.hxx>
#include <standardsinterface/DoubleAttribute.hxx>
#include <standardsinterface/EnumAttribute.hxx>
#include <standardsinterface/EnumAttributeValues.hxx>

#include <utilities/core/Enum.hpp>

#include <boost/optional.hpp>

namespace openstudio {
namespace standardsinterface {

/** \class PartsOfSpeech 
 *
 *  \relates DataDictionary */
OPENSTUDIO_ENUM( PartsOfSpeech,
  ((ObjectType))
  ((Boolean))
  ((Integer))
  ((Double))
  ((Enum)) );

/** \relates PartsOfSpeech */
typedef boost::optional<PartsOfSpeech> OptionalPartsOfSpeech;

/** Provides programmatic access to the Data Dictionary. The Data Dictionary is a series of 
 *  enumerations (OPENSTUDIO_ENUM, see utilities/core/Enum.hpp) semi-auto-generated from the CEC 
 *  Controlled Vocabulary Sharepoint Site administered by L'Monte Information Services. The terms
 *  are broken down into five PartsOfSpeech: ObjectType, and then four types of attributes 
 *  differentiated by the data type used to represent them (Boolean, Integer, Double, Enum). */
struct STANDARDSINTERFACE_API DataDictionary {
  /** @name Getters */
  //@{

  /** Returns the available \link ObjectType ObjectTypes \endlink. */
  static std::vector<ObjectType> objectTypes();

  /** Returns the \link ObjectType ObjectTypes \endlink for which a call to 
   *  AgnosticInterface::getObjects should return 0-1 objects. */
  static std::vector<ObjectType> objectTypesUniqueWithinOneModel();

  /** Returns the available \link BooleanAttribute BooleanAttributes \endlink. */
  static std::vector<BooleanAttribute> booleanAttributes();

  /** Returns the available \link IntegerAttribute IntegerAttributes \endlink. */
  static std::vector<IntegerAttribute> integerAttributes();

  /** Returns the available \link DoubleAttribute DoubleAttributes \endlink. */
  static std::vector<DoubleAttribute> doubleAttributes();

  /** Returns the available \link EnumAttribute EnumAttributes \endlink. */
  static std::vector<EnumAttribute> enumAttributes();

  /** Returns all values associated with the specified enum attribute. */
  static std::vector<std::string> enumValues(const EnumAttribute& ddEnumAttribute);

  /** Returns the standard string representation of candidate if 
   *  DataDictionary::isEnumValue(ddEnumAttribute,candidate). The return value is .empty() 
   *  otherwise. This method is primarily used to make it easier to programmatically match
   *  enumeration value strings. */
  static std::string standardEnumValue(const EnumAttribute& ddEnumAttribute, 
                                       const std::string& candidate);

  //@}
  /** @name Queries */
  //@{

  /** Returns true if candidate can be identified as an ObjectType. */
  static bool isObjectType(const std::string& candidate);

  /** Returns true if candidate can be identified as a BooleanAttribute. */
  static bool isBooleanAttribute(const std::string& candidate);

  /** Returns true if candidate can be identified as an IntegerAttribute. */
  static bool isIntegerAttribute(const std::string& candidate);

  /** Returns true if candidate can be identified as a DoubleAttribute. */
  static bool isDoubleAttribute(const std::string& candidate);

  /** Returns true if candidate can be identified as an EnumAttribute. */
  static bool isEnumAttribute(const std::string& candidate);

  /** Returns true if candidate can be identified as a valid value of ddEnumAttribute. */
  static bool isEnumValue(const EnumAttribute& ddEnumAttribute, const std::string& candidate);

  /** Returns true if isEnumValue(ddEnumAttribute,value1) and 
   *  standardEnumValue(ddEnumAttribute,value1) == standardEnumValue(ddEnumAttribute,value2). */
  static bool areEquivalentEnumValues(const EnumAttribute& ddEnumAttribute,
                                      const std::string& value1,
                                      const std::string& value2);

  /** Returns the PartsOfSpeech ddTerm belongs to if DataDictionary::isXXXXX(ddTerm) returns true
   *  for at least one such method other than isEnumValue. Returns boost::none (and evaluates to 
   *  false) otherwise. */
  static boost::optional<PartsOfSpeech> partOfSpeech(const std::string& ddTerm);

  //@}

 private:
  DataDictionary();
};

} // standardsinterface
} // openstudio

#endif // STANDARDSINTERFACE_DATADICTIONARY_HPP
