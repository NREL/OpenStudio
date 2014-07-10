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

#ifndef UTILITIES_UNITS_QUANTITYFACTORY_HPP
#define UTILITIES_UNITS_QUANTITYFACTORY_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Singleton.hpp"
#include "Quantity.hpp"
#include "UnitFactory.hpp"

#include "../core/Logger.hpp"

#include <set>
#include <map>

namespace openstudio{

class UTILITIES_API QuantityFactorySingleton {

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
  boost::optional<Quantity> createQuantity(const std::string& quantityString,UnitSystem system) const;

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

typedef openstudio::Singleton<QuantityFactorySingleton> QuantityFactory;

/** Creates a Quantity object from quantityString (a string that matches
 *  openstudio::regexQuantity()). */
UTILITIES_API boost::optional<Quantity> createQuantity(const std::string& quantityString);

UTILITIES_API boost::optional<Quantity> createQuantity(const std::string& quantityString, UnitSystem system);

/** Creates a general Quantity object corresponding to value * unitString. */
UTILITIES_API boost::optional<Quantity> createQuantity(double value, const std::string& unitString);

UTILITIES_API boost::optional<Quantity> createQuantity(double value,
                                                       const std::string& unitString,
                                                       UnitSystem system);

} // openstudio

#endif // UTILITIES_UNITS_QUANTITYFACTORY_HPP
