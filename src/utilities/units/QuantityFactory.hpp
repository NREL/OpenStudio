/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_QUANTITYFACTORY_HPP
#define UTILITIES_UNITS_QUANTITYFACTORY_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Singleton.hpp"
#include "Quantity.hpp"
#include "UnitFactory.hpp"

#include "../core/Logger.hpp"

#include <set>
#include <map>

namespace openstudio {

class UTILITIES_API QuantityFactorySingleton
{

  friend class Singleton<QuantityFactorySingleton>;

 public:
  // FACTORY USE

  /** Parses quantity string to separate the value from the unit. Then creates the quantity
   *  with the help of UnitFactory. Uses openstudio::getSystem to intuit the appropriate system,
   *  if possible. */
  boost::optional<Quantity> createQuantity(const std::string& quantityString) const;

  /** Parses quantity string to separate the value from the unit. Then creates the quantity
   *  with the help of UnitFactory. system specifies the preferred unit system--it does not
   *  guarantee that the returned result will be in that system. */
  boost::optional<Quantity> createQuantity(const std::string& quantityString, UnitSystem system) const;

  /** Creates a quantity from value and unitString, which is passed to UnitFactory. Uses
   *  openstudio::getSystem to intuit the appropriate system, if possible. */
  boost::optional<Quantity> createQuantity(double value, const std::string& unitString) const;

  /** Creates a quantity from value and unitString, which is passed to UnitFactory. system
   *  specifies the preferred unit system--it does not guarantee that the returned result will
   *  be in that system. */
  boost::optional<Quantity> createQuantity(double value, const std::string& unitString, UnitSystem system) const;

 private:
  REGISTER_LOGGER("openstudio.units.QuantityFactory");
};

using QuantityFactory = openstudio::Singleton<QuantityFactorySingleton>;

/** Creates a Quantity object from quantityString (a string that matches
 *  openstudio::regexQuantity()). */
UTILITIES_API boost::optional<Quantity> createQuantity(const std::string& quantityString);

UTILITIES_API boost::optional<Quantity> createQuantity(const std::string& quantityString, UnitSystem system);

/** Creates a general Quantity object corresponding to value * unitString. */
UTILITIES_API boost::optional<Quantity> createQuantity(double value, const std::string& unitString);

UTILITIES_API boost::optional<Quantity> createQuantity(double value, const std::string& unitString, UnitSystem system);

}  // namespace openstudio

#endif  // UTILITIES_UNITS_QUANTITYFACTORY_HPP
