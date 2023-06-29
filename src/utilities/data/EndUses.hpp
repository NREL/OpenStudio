/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_ENDUSES_HPP
#define UTILITIES_DATA_ENDUSES_HPP

#include "../UtilitiesAPI.hpp"
#include "Attribute.hpp"
#include "DataEnums.hpp"

namespace openstudio {

/** EndUses is wrapper around Attribute for storing information about end uses.
*/
class UTILITIES_API EndUses
{
 public:
  /// Returns the expected name for attributes of this type, 'EndUses'.
  static std::string attributeName();

  /// get units for fuel type
  static std::string getUnitsForFuelType(const EndUseFuelType& fuelType);

  /// get list of all fuel types
  static std::vector<EndUseFuelType> fuelTypes();

  /// get list of all categories
  static std::vector<EndUseCategoryType> categories();

  /// Creates an EndUses wrapper around attribute if it is correctly formatted.
  static boost::optional<EndUses> fromAttribute(const Attribute& attribute);

  /// Constructs a new end uses attribute
  EndUses();

  /// Returns the attribute
  Attribute attribute() const;

  /// add an end use
  void addEndUse(double value, const EndUseFuelType& fuelType, const EndUseCategoryType& category, const std::string& subCategory = "General");

  /// get end use value
  double getEndUse(const EndUseFuelType& fuelType, const EndUseCategoryType& category, const std::string& subCategory) const;

  /// get end use value across sub categories
  double getEndUse(const EndUseFuelType& fuelType, const EndUseCategoryType& category) const;

  /// get end use value across fuel types
  double getEndUseByCategory(const EndUseCategoryType& category, const std::string& subCategory) const;

  /// get end use value across fuel types and sub categories
  double getEndUseByCategory(const EndUseCategoryType& category) const;

  /// get end use value across categories and sub categories
  double getEndUseByFuelType(const EndUseFuelType& fuelType) const;

  /// get list of all sub cateogories
  std::vector<std::string> subCategories() const;

 private:
  EndUses(const Attribute& attribute);

  Attribute m_attribute;

  REGISTER_LOGGER("openstudio.EndUses");
};

}  // namespace openstudio

#endif  // UTILITIES_DATA_ENDUSES_HPP
