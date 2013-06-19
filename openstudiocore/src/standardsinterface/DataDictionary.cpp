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

#include <standardsinterface/DataDictionary.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Compare.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace standardsinterface {

// GETTERS

std::vector<ObjectType> DataDictionary::objectTypes() {
  ObjectTypeVector result;
  IntSet otvs = ObjectType::getValues();
  BOOST_FOREACH(int v,otvs) { result.push_back(ObjectType(v)); }
  return result;
}

std::vector<ObjectType> DataDictionary::objectTypesUniqueWithinOneModel() {
  ObjectTypeVector result;
  result.push_back(ObjectType(ObjectType::Building));
  return result;
}

std::vector<BooleanAttribute> DataDictionary::booleanAttributes() {
  BooleanAttributeVector result;
  IntSet bavs = BooleanAttribute::getValues();
  BOOST_FOREACH(int v,bavs) { result.push_back(BooleanAttribute(v)); }
  return result;
}

std::vector<IntegerAttribute> DataDictionary::integerAttributes() {
  IntegerAttributeVector result;
  IntSet iavs = IntegerAttribute::getValues();
  BOOST_FOREACH(int v,iavs) { result.push_back(IntegerAttribute(v)); }
  return result;
}

std::vector<DoubleAttribute> DataDictionary::doubleAttributes() {
  DoubleAttributeVector result;
  IntSet davs = DoubleAttribute::getValues();
  BOOST_FOREACH(int v,davs) { result.push_back(DoubleAttribute(v)); }
  return result;
}

std::vector<EnumAttribute> DataDictionary::enumAttributes() {
  EnumAttributeVector result;
  IntSet eavs = EnumAttribute::getValues();
  BOOST_FOREACH(int v,eavs) { result.push_back(EnumAttribute(v)); }
  return result;
}

std::string DataDictionary::standardEnumValue(const EnumAttribute& ddEnumAttribute, 
                                              const std::string& candidate) 
{
  StringVector standardValueStrings = enumValues(ddEnumAttribute);
  BOOST_FOREACH(const std::string& standardString,standardValueStrings) {
    if (areEquivalentEnumValues(ddEnumAttribute,candidate,standardString)) {
      return standardString;
    }
  }
  return std::string();
}

bool DataDictionary::isObjectType(const std::string& candidate) {
  try {
    ObjectType ot(candidate);
    return true;
  }
  catch (...) {}
  return false;
}

bool DataDictionary::isBooleanAttribute(const std::string& candidate) {
  try {
    BooleanAttribute e(candidate);
    return true;
  }
  catch (...) {}
  return false;
}

bool DataDictionary::isIntegerAttribute(const std::string& candidate) {
  try {
    IntegerAttribute i(candidate);
    return true;
  }
  catch (...) {}
  return false;
}

bool DataDictionary::isDoubleAttribute(const std::string& candidate) {
  try {
    DoubleAttribute d(candidate);
    return true;
  }
  catch (...) {}
  return false;
}

bool DataDictionary::isEnumAttribute(const std::string& candidate) {
  try {
    EnumAttribute e(candidate);
    return true;
  }
  catch (...) {}
  return false;
}

OptionalPartsOfSpeech DataDictionary::partOfSpeech(const std::string& ddTerm) {
  if (isObjectType(ddTerm)) { return PartsOfSpeech(PartsOfSpeech::ObjectType); }
  if (isBooleanAttribute(ddTerm)) { return PartsOfSpeech(PartsOfSpeech::Boolean); }
  if (isIntegerAttribute(ddTerm)) { return PartsOfSpeech(PartsOfSpeech::Integer); }
  if (isDoubleAttribute(ddTerm)) { return PartsOfSpeech(PartsOfSpeech::Double); }
  if (isEnumAttribute(ddTerm)) { return PartsOfSpeech(PartsOfSpeech::Enum); }
  return boost::none;
}

} // standardsinterface
} // openstudio
