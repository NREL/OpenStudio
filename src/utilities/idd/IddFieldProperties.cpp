/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "IddFieldProperties.hpp"

#include "../core/Containers.hpp"

#include <sstream>

namespace openstudio {

/// default constructor
IddFieldProperties::IddFieldProperties()
  : type(IddFieldType::UnknownType),
    required(false),
    autosizable(false),
    autocalculatable(false),
    retaincase(false),
    deprecated(false),
    beginExtensible(false),
    minBoundType(Unbounded),
    maxBoundType(Unbounded) {}

bool IddFieldProperties::isGeneric() const {
  IddFieldProperties genericProperties;
  genericProperties.type = type;
  return (*this == genericProperties);
}

bool IddFieldProperties::operator==(const IddFieldProperties& other) const {
  /** Only compare one or other of string versus numeric. */
  if (this == &other) {
    return true;
  }

  if (type != other.type) {
    return false;
  }
  if (note != other.note) {
    return false;
  }
  if (required != other.required) {
    return false;
  }
  if (autosizable != other.autosizable) {
    return false;
  }
  if (autocalculatable != other.autocalculatable) {
    return false;
  }
  if (retaincase != other.retaincase) {
    return false;
  }
  if (deprecated != other.deprecated) {
    return false;
  }
  if (units != other.units) {
    return false;
  }
  if (ipUnits != other.ipUnits) {
    return false;
  }
  if (minBoundType != other.minBoundType) {
    return false;
  }
  if (minBoundValue != other.minBoundValue) {
    return false;
  }
  if (maxBoundType != other.maxBoundType) {
    return false;
  }
  if (maxBoundValue != other.maxBoundValue) {
    // std::string myBound, theirBound;
    // if (maxBoundText) {
    //   myBound = *maxBoundText;
    // }
    // if (other.maxBoundText) {
    //   theirBound = *(other.maxBoundText);
    // }
    return false;
  }
  if (stringDefault != other.stringDefault) {
    return false;
  }
  if (objectLists != other.objectLists) {
    return false;
  }
  if (externalLists != other.externalLists) {
    return false;
  }
  if (references != other.references) {
    return false;
  }
  if (referenceClassNames != other.referenceClassNames) {
    return false;
  }

  return true;
}

bool IddFieldProperties::operator!=(const IddFieldProperties& other) const {
  return !((*this) == other);
}

std::ostream& IddFieldProperties::print(std::ostream& os) const {
  if (!note.empty()) {
    std::stringstream ss(note);
    std::string line;
    while (!ss.eof()) {
      std::getline(ss, line);
      os << "       \\note " << line << std::endl;
    }
  }

  if (type == IddFieldType::IntegerType) {
    os << "       \\type integer" << std::endl;
  } else if (type == IddFieldType::RealType) {
    os << "       \\type real" << std::endl;
  } else if (type == IddFieldType::AlphaType) {
    os << "       \\type alpha" << std::endl;
  } else if (type == IddFieldType::ChoiceType) {
    os << "       \\type choice" << std::endl;
  } else if (type == IddFieldType::NodeType) {
    os << "       \\type node" << std::endl;
  } else if (type == IddFieldType::ObjectListType) {
    os << "       \\type object-list" << std::endl;
  } else if (type == IddFieldType::ExternalListType) {
    os << "       \\type external-list" << std::endl;
  } else if (type == IddFieldType::URLType) {
    os << "       \\type url" << std::endl;
  } else if (type == IddFieldType::HandleType) {
    os << "       \\type handle" << std::endl;
  }

  if (required) {
    os << "       \\required-field" << std::endl;
  }
  if (autosizable) {
    os << "       \\autosizable" << std::endl;
  }
  if (autocalculatable) {
    os << "       \\autocalculatable" << std::endl;
  }

  if (retaincase) {
    os << "       \\retaincase" << std::endl;
  }
  if (deprecated) {
    os << "       \\deprecated" << std::endl;
  }
  if (beginExtensible) {
    os << "       \\begin-extensible" << std::endl;
  }
  if (units) {
    os << "       \\units " << *units << std::endl;
  }
  if (ipUnits) {
    os << "       \\ip-units " << *ipUnits << std::endl;
  }
  if (minBoundType == IddFieldProperties::InclusiveBound && minBoundValue) {
    os << "       \\minimum " << *minBoundValue << std::endl;
  }
  if (minBoundType == IddFieldProperties::ExclusiveBound && minBoundValue) {
    os << "       \\minimum> " << *minBoundValue << std::endl;
  }
  if (maxBoundType == IddFieldProperties::InclusiveBound && maxBoundValue) {
    os << "       \\maximum " << *maxBoundValue << std::endl;
  }
  if (maxBoundType == IddFieldProperties::ExclusiveBound && maxBoundValue) {
    os << "       \\maximum< " << *maxBoundValue << std::endl;
  }
  if (stringDefault) {
    os << "       \\default " << *stringDefault << std::endl;
  }
  if (!objectLists.empty()) {
    for (const std::string& objectList : objectLists) {
      os << "       \\object-list " << objectList << std::endl;
    }
  }
  if (!references.empty()) {
    for (const std::string& reference : references) {
      os << "       \\reference " << reference << std::endl;
    }
  }
  if (!references.empty()) {
    for (const std::string& reference : referenceClassNames) {
      os << "       \\reference-class-name " << reference << std::endl;
    }
  }
  if (!externalLists.empty()) {
    for (const std::string& externalList : externalLists) {
      os << "       \\external-list " << externalList << std::endl;
    }
  }

  return os;
}

StringVector intersectReferenceLists(const StringVector& list1, const StringVector& list2) {
  IStringSet set1(list1.begin(), list1.end());
  IStringSet set2(list2.begin(), list2.end());
  IStringSet intersection;
  std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(intersection, intersection.begin()),
                        IStringSet::key_compare());
  return StringVector(intersection.begin(), intersection.end());
}

}  // namespace openstudio
