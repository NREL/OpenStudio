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

#ifndef UTILITIES_DATA_ENDUSES_HPP
#define UTILITIES_DATA_ENDUSES_HPP

#include "../UtilitiesAPI.hpp"
#include "Attribute.hpp"
#include "DataEnums.hpp"

namespace openstudio {

/** EndUses is wrapper around Attribute for storing information about end uses.
*/
class UTILITIES_API EndUses {
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

} // openstudio

#endif // UTILITIES_DATA_ENDUSES_HPP
