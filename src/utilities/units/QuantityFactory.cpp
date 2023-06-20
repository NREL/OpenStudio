/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "QuantityFactory.hpp"
#include "QuantityRegex.hpp"

#include "../core/Exception.hpp"
#include "../core/Containers.hpp"

#include <boost/pointer_cast.hpp>
#include <boost/lexical_cast.hpp>

using std::shared_ptr;
using std::dynamic_pointer_cast;

namespace openstudio {

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(const std::string& quantityString) const {
  OptionalQuantity result;
  StringPair strings = decomposeQuantityString(quantityString);
  try {
    auto value = boost::lexical_cast<double>(strings.first);
    result = createQuantity(value, strings.second);
  } catch (...) {
    LOG(Error, "Unable to create quantity from string '" << quantityString << "'. Returning 0.0.");
  }
  return result;
}

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(const std::string& quantityString, UnitSystem system) const {
  OptionalQuantity result;
  StringPair strings = decomposeQuantityString(quantityString);
  try {
    auto value = boost::lexical_cast<double>(strings.first);
    result = createQuantity(value, strings.second, system);
  } catch (...) {
    LOG(Error, "Unable to create quantity from string '" << quantityString << "'. Returning 0.0.");
  }
  return result;
}

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(double value, const std::string& unitString) const {
  // create return variable, and try to create unit
  OptionalQuantity result;
  UnitSystem system = getSystem(unitString);
  OptionalUnit unit = UnitFactory::instance().createUnit(unitString, system);
  if (unit) {
    result = Quantity(value, *unit);
  }
  return result;
}

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(double value, const std::string& unitString, UnitSystem system) const {
  // create return variable, and try to create unit
  OptionalQuantity result;
  OptionalUnit unit = UnitFactory::instance().createUnit(unitString, system);
  if (unit) {
    result = Quantity(value, *unit);
  }
  return result;
}

boost::optional<Quantity> createQuantity(const std::string& quantityString) {
  return QuantityFactory::instance().createQuantity(quantityString);
}

boost::optional<Quantity> createQuantity(const std::string& quantityString, UnitSystem system) {
  return QuantityFactory::instance().createQuantity(quantityString, system);
}

boost::optional<Quantity> createQuantity(double value, const std::string& unitString) {
  return QuantityFactory::instance().createQuantity(value, unitString);
}

boost::optional<Quantity> createQuantity(double value, const std::string& unitString, UnitSystem system) {
  return QuantityFactory::instance().createQuantity(value, unitString, system);
}

}  // namespace openstudio
