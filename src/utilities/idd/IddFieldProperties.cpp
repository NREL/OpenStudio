/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      os << "       \\note " << line << '\n';
    }
  }

  if (type == IddFieldType::IntegerType) {
    os << "       \\type integer" << '\n';
  } else if (type == IddFieldType::RealType) {
    os << "       \\type real" << '\n';
  } else if (type == IddFieldType::AlphaType) {
    os << "       \\type alpha" << '\n';
  } else if (type == IddFieldType::ChoiceType) {
    os << "       \\type choice" << '\n';
  } else if (type == IddFieldType::NodeType) {
    os << "       \\type node" << '\n';
  } else if (type == IddFieldType::ObjectListType) {
    os << "       \\type object-list" << '\n';
  } else if (type == IddFieldType::ExternalListType) {
    os << "       \\type external-list" << '\n';
  } else if (type == IddFieldType::URLType) {
    os << "       \\type url" << '\n';
  } else if (type == IddFieldType::HandleType) {
    os << "       \\type handle" << '\n';
  }

  if (required) {
    os << "       \\required-field" << '\n';
  }
  if (autosizable) {
    os << "       \\autosizable" << '\n';
  }
  if (autocalculatable) {
    os << "       \\autocalculatable" << '\n';
  }

  if (retaincase) {
    os << "       \\retaincase" << '\n';
  }
  if (deprecated) {
    os << "       \\deprecated" << '\n';
  }
  if (beginExtensible) {
    os << "       \\begin-extensible" << '\n';
  }
  if (units) {
    os << "       \\units " << *units << '\n';
  }
  if (ipUnits) {
    os << "       \\ip-units " << *ipUnits << '\n';
  }
  if (minBoundType == IddFieldProperties::InclusiveBound && minBoundValue) {
    os << "       \\minimum " << *minBoundValue << '\n';
  }
  if (minBoundType == IddFieldProperties::ExclusiveBound && minBoundValue) {
    os << "       \\minimum> " << *minBoundValue << '\n';
  }
  if (maxBoundType == IddFieldProperties::InclusiveBound && maxBoundValue) {
    os << "       \\maximum " << *maxBoundValue << '\n';
  }
  if (maxBoundType == IddFieldProperties::ExclusiveBound && maxBoundValue) {
    os << "       \\maximum< " << *maxBoundValue << '\n';
  }
  if (stringDefault) {
    os << "       \\default " << *stringDefault << '\n';
  }
  if (!objectLists.empty()) {
    for (const std::string& objectList : objectLists) {
      os << "       \\object-list " << objectList << '\n';
    }
  }
  if (!references.empty()) {
    for (const std::string& reference : references) {
      os << "       \\reference " << reference << '\n';
    }
  }
  if (!referenceClassNames.empty()) {
    for (const std::string& reference : referenceClassNames) {
      os << "       \\reference-class-name " << reference << '\n';
    }
  }
  if (!externalLists.empty()) {
    for (const std::string& externalList : externalLists) {
      os << "       \\external-list " << externalList << '\n';
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
  return {intersection.begin(), intersection.end()};
}

}  // namespace openstudio
