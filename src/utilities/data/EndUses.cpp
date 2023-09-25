/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EndUses.hpp"
#include "../core/Assert.hpp"

namespace openstudio {

struct FuelTypeAttributeSorter
{
  bool operator()(const Attribute& leftFuelTypeAttribute, const Attribute& rightFuelTypeAttribute) const {
    EndUseFuelType leftFuelType(leftFuelTypeAttribute.name());
    EndUseFuelType rightFuelType(rightFuelTypeAttribute.name());
    return (leftFuelType.value() < rightFuelType.value());
  }
};

struct CategoryAttributeSorter
{
  bool operator()(const Attribute& leftCategoryAttribute, const Attribute& rightCategoryAttribute) const {
    EndUseCategoryType leftCategory(leftCategoryAttribute.name());
    EndUseCategoryType rightCategory(rightCategoryAttribute.name());
    return (leftCategory.value() < rightCategory.value());
  }
};

struct SubCategoryAttributeSorter
{
  bool operator()(const Attribute& leftSubCategoryAttribute, const Attribute& rightSubCategoryAttribute) const {
    return (leftSubCategoryAttribute.name() < rightSubCategoryAttribute.name());
  }
};

std::string EndUses::attributeName() {
  return "EndUses";
}

std::string EndUses::getUnitsForFuelType(const EndUseFuelType& fuelType) {
  if (fuelType == EndUseFuelType::Water) {
    return "m3";
  }
  return "GJ";
}

std::vector<EndUseFuelType> EndUses::fuelTypes() {
  std::vector<EndUseFuelType> result;

  std::map<int, std::string> endUseFuelTypes = EndUseFuelType::getNames();
  auto endUseFuelTypesIt = endUseFuelTypes.begin();
  auto endUseFuelTypesItEnd = endUseFuelTypes.end();

  for (; endUseFuelTypesIt != endUseFuelTypesItEnd; ++endUseFuelTypesIt) {
    result.push_back(EndUseFuelType(endUseFuelTypesIt->first));
  }

  return result;
}

std::vector<EndUseCategoryType> EndUses::categories() {
  std::vector<EndUseCategoryType> result;

  std::map<int, std::string> endUseCategoryTypes = EndUseCategoryType::getNames();
  auto endUseCategoryTypesIt = endUseCategoryTypes.begin();
  auto endUseCategoryTypesItEnd = endUseCategoryTypes.end();

  for (; endUseCategoryTypesIt != endUseCategoryTypesItEnd; ++endUseCategoryTypesIt) {
    result.push_back(EndUseCategoryType(endUseCategoryTypesIt->first));
  }

  return result;
}

boost::optional<EndUses> EndUses::fromAttribute(const Attribute& attribute) {
  if (attribute.name() != EndUses::attributeName()) {
    LOG(Warn, "Cannot create EndUses from Attribute because attribute name is '" << attribute.name() << "'.");
    return boost::none;
  }

  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG(Warn,
        "Cannot create EndUses from Attribute because attribute is a " << attribute.valueType().valueDescription() << ", not an AttributeVector.");
    return boost::none;
  }

  if (attribute.units()) {
    LOG(Warn, "Cannot create EndUses from Attribute because attribute has units '" << attribute.units().get() << "'.");
    return boost::none;
  }

  for (const Attribute& fuelTypeAttribute : attribute.valueAsAttributeVector()) {

    EndUseFuelType endUseFuelType(fuelTypeAttribute.name());
    if (fuelTypeAttribute.valueType() != AttributeValueType::AttributeVector) {
      LOG(Warn, "Cannot create EndUses from Attribute because fuel type attribute '" << fuelTypeAttribute.name() << "' is a "
                                                                                     << fuelTypeAttribute.valueType() << ", not an AttributeVector.");
      return boost::none;
    }

    for (const Attribute& categoryAttribute : fuelTypeAttribute.valueAsAttributeVector()) {

      EndUseCategoryType endUseCategoryType(categoryAttribute.name());
      if (categoryAttribute.valueType() != AttributeValueType::AttributeVector) {
        LOG(Warn, "Cannot create EndUses from Attribute because category attribute '"
                    << categoryAttribute.name() << "' is a " << categoryAttribute.valueType() << ", not an AttributeVector.");
        return boost::none;
      }

      for (const Attribute& subCategoryAttribute : categoryAttribute.valueAsAttributeVector()) {
        if (subCategoryAttribute.valueType() != AttributeValueType::Double) {
          LOG(Warn, "Cannot create EndUses from Attribute because sub-category attribute '" << categoryAttribute.name() << "' is a "
                                                                                            << subCategoryAttribute.valueType() << ", not a Double.");
          return boost::none;
        }
        if (!subCategoryAttribute.units()) {
          LOG(Warn, "Cannot create EndUses from Attribute because sub-category attribute '" << categoryAttribute.name() << "' does not have units.");
          return boost::none;
        }
        if (subCategoryAttribute.units().get() != getUnitsForFuelType(endUseFuelType)) {
          LOG(Warn, "Cannot create EndUses from Attribute because sub-category attribute '"
                      << categoryAttribute.name() << "' has units '" << subCategoryAttribute.units().get() << "', not '"
                      << getUnitsForFuelType(endUseFuelType) << "', as expected for fuel type " << endUseFuelType.valueDescription() << ".");
          return boost::none;
        }
      }
    }
  }

  return EndUses(attribute);
}

EndUses::EndUses() : m_attribute(EndUses::attributeName(), std::vector<openstudio::Attribute>(), boost::none) {}

EndUses::EndUses(const Attribute& attribute) : m_attribute(attribute) {}

Attribute EndUses::attribute() const {
  return m_attribute;
}

void EndUses::addEndUse(double value, const EndUseFuelType& fuelType, const EndUseCategoryType& category, const std::string& subCategory) {
  std::string units = this->getUnitsForFuelType(fuelType);

  // Try to find an existing fuelType attribute
  boost::optional<Attribute> fuelTypeAttribute = m_attribute.findChildByName(fuelType.valueName());
  if (!fuelTypeAttribute) {
    // If you can't, then add a new Attribute to the std::vector of Attribute
    std::vector<Attribute> fuelTypeAttributes = m_attribute.valueAsAttributeVector();
    fuelTypeAttributes.push_back(Attribute(fuelType.valueName(), std::vector<Attribute>()));
    std::sort(fuelTypeAttributes.begin(), fuelTypeAttributes.end(), FuelTypeAttributeSorter());
    m_attribute.setValue(fuelTypeAttributes);
    fuelTypeAttribute = m_attribute.findChildByName(fuelType.valueName());
  }
  OS_ASSERT(fuelTypeAttribute);

  // Within the FuelTypeAttribute, we look if the EndUseCategory actually exists
  boost::optional<Attribute> categoryAttribute = fuelTypeAttribute->findChildByName(category.valueName());
  if (!categoryAttribute) {
    // Otherwise we add it
    std::vector<Attribute> categoryAttributes = fuelTypeAttribute->valueAsAttributeVector();
    categoryAttributes.push_back(Attribute(category.valueName(), std::vector<Attribute>()));
    std::sort(categoryAttributes.begin(), categoryAttributes.end(), CategoryAttributeSorter());
    fuelTypeAttribute->setValue(categoryAttributes);
    categoryAttribute = fuelTypeAttribute->findChildByName(category.valueName());
  }
  OS_ASSERT(categoryAttribute);

  // Within that category, if we find an existing SubCategory, then we add our value that category
  bool found = false;
  std::vector<Attribute> subCategories = categoryAttribute->valueAsAttributeVector();
  std::vector<Attribute> newSubCategories = subCategories;
  for (unsigned i = 0; i < subCategories.size(); ++i) {
    if (subCategories[i].name() == subCategory) {
      OS_ASSERT(!found);
      newSubCategories[i] = Attribute(subCategory, subCategories[i].valueAsDouble() + value, subCategories[i].units());
      found = true;
    }
  }

  // If we didn't find the proper Subcategory, we create it
  if (!found) {
    newSubCategories.push_back(Attribute(subCategory, value, units));
    std::sort(newSubCategories.begin(), newSubCategories.end(), SubCategoryAttributeSorter());
  }

  categoryAttribute->setValue(newSubCategories);
}

double EndUses::getEndUse(const EndUseFuelType& fuelType, const EndUseCategoryType& category, const std::string& subCategory) const {
  double result = 0;

  boost::optional<Attribute> fuelTypeAttribute = m_attribute.findChildByName(fuelType.valueName());
  if (fuelTypeAttribute) {
    boost::optional<Attribute> categoryAttribute = fuelTypeAttribute->findChildByName(category.valueName());
    if (categoryAttribute) {
      std::vector<Attribute> subCategories = categoryAttribute->valueAsAttributeVector();
      for (unsigned i = 0; i < subCategories.size(); ++i) {
        if (subCategories[i].name() == subCategory) {
          result = subCategories[i].valueAsDouble();
          break;
        }
      }
    }
  }

  return result;
}

double EndUses::getEndUse(const EndUseFuelType& fuelType, const EndUseCategoryType& category) const {
  double result = 0;

  boost::optional<Attribute> fuelTypeAttribute = m_attribute.findChildByName(fuelType.valueName());
  if (fuelTypeAttribute) {
    boost::optional<Attribute> categoryAttribute = fuelTypeAttribute->findChildByName(category.valueName());
    if (categoryAttribute) {
      std::vector<Attribute> subCategories = categoryAttribute->valueAsAttributeVector();
      for (unsigned i = 0; i < subCategories.size(); ++i) {
        result += subCategories[i].valueAsDouble();
      }
    }
  }

  return result;
}

double EndUses::getEndUseByCategory(const EndUseCategoryType& category, const std::string& subCategory) const {
  double result = 0;
  std::map<int, std::string> endUseFuelTypes = EndUseFuelType::getNames();
  auto endUseFuelTypesIt = endUseFuelTypes.begin();
  auto endUseFuelTypesItEnd = endUseFuelTypes.end();

  for (; endUseFuelTypesIt != endUseFuelTypesItEnd; ++endUseFuelTypesIt) {
    result += getEndUse(EndUseFuelType(endUseFuelTypesIt->second), category, subCategory);
  }

  return result;
}

double EndUses::getEndUseByCategory(const EndUseCategoryType& category) const {
  double result = 0;
  std::map<int, std::string> endUseFuelTypes = EndUseFuelType::getNames();
  auto endUseFuelTypesIt = endUseFuelTypes.begin();
  auto endUseFuelTypesItEnd = endUseFuelTypes.end();

  for (; endUseFuelTypesIt != endUseFuelTypesItEnd; ++endUseFuelTypesIt) {
    result += getEndUse(EndUseFuelType(endUseFuelTypesIt->second), category);
  }

  return result;
}

double EndUses::getEndUseByFuelType(const EndUseFuelType& fuelType) const {
  double result = 0;
  std::map<int, std::string> endUseCategoryTypes = EndUseCategoryType::getNames();
  auto endUseCategoryTypesIt = endUseCategoryTypes.begin();
  auto endUseCategoryTypesItEnd = endUseCategoryTypes.end();

  for (; endUseCategoryTypesIt != endUseCategoryTypesItEnd; ++endUseCategoryTypesIt) {
    result += getEndUse(fuelType, EndUseCategoryType(endUseCategoryTypesIt->second));
  }

  return result;
}

std::vector<std::string> EndUses::subCategories() const {
  std::set<std::string> subCategories;
  for (const Attribute& fuelType : m_attribute.valueAsAttributeVector()) {
    for (const Attribute& category : fuelType.valueAsAttributeVector()) {
      for (const Attribute& subCategory : category.valueAsAttributeVector()) {
        subCategories.insert(subCategory.name());
      }
    }
  }

  return {subCategories.begin(), subCategories.end()};
}

}  // namespace openstudio
