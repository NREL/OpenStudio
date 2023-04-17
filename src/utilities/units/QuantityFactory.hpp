/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
