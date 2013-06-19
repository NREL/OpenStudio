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

#include <utilities/units/QuantityFactory.hpp>
#include <utilities/units/QuantityRegex.hpp>

#include <utilities/core/Exception.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/lexical_cast.hpp>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace openstudio{

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(const std::string& quantityString) const {
  OptionalQuantity result;
  StringPair strings = decomposeQuantityString(quantityString);
  try {
    double value = boost::lexical_cast<double>(strings.first);
    result = createQuantity(value,strings.second);
  }
  catch (...) {
    LOG(Error,"Unable to create quantity from string '" << quantityString << "'. Returning 0.0.");
  }
  return result;
}

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(const std::string& quantityString,
                                                  UnitSystem system) const
{
  OptionalQuantity result;
  StringPair strings = decomposeQuantityString(quantityString);
  try {
    double value = boost::lexical_cast<double>(strings.first);
    result = createQuantity(value,strings.second,system);
  }
  catch (...) {
    LOG(Error,"Unable to create quantity from string '" << quantityString << "'. Returning 0.0.");
  }
  return result;
}

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(
    double value,
    const std::string& unitString) const
{
  // create return variable, and try to create unit
  OptionalQuantity result;
  UnitSystem system = getSystem(unitString);
  OptionalUnit unit = UnitFactory::instance().createUnit(unitString,system);
  if (unit) {
    result = Quantity(value,*unit);
  }
  return result;
}

boost::optional<Quantity> QuantityFactorySingleton::createQuantity(
    double value,
    const std::string& unitString,
    UnitSystem system) const
{
  // create return variable, and try to create unit
  OptionalQuantity result;
  OptionalUnit unit = UnitFactory::instance().createUnit(unitString,system);
  if (unit) {
    result = Quantity(value,*unit);
  }
  return result;
}

boost::optional<Quantity> createQuantity(const std::string& quantityString) {
  return QuantityFactory::instance().createQuantity(quantityString);
}

boost::optional<Quantity> createQuantity(const std::string& quantityString,
                                         UnitSystem system)
{
  return QuantityFactory::instance().createQuantity(quantityString,system);
}

boost::optional<Quantity> createQuantity(double value,const std::string& unitString) {
  return QuantityFactory::instance().createQuantity(value,unitString);
}

boost::optional<Quantity> createQuantity(double value,
                                         const std::string& unitString,
                                         UnitSystem system)
{
  return QuantityFactory::instance().createQuantity(value,unitString,system);
}

} // openstudio
