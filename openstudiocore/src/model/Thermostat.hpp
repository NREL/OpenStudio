/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef MODEL_THERMOSTAT_HPP
#define MODEL_THERMOSTAT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class ThermalZone;

namespace detail {
  class Thermostat_Impl;
};

/** Thermostat is the base class for all types of thermostats.
 */
class MODEL_API Thermostat : public ModelObject
{
  public:

  virtual ~Thermostat() {}

  boost::optional<ThermalZone> thermalZone() const;

  protected:

  Thermostat(IddObjectType type,const Model& model);

  typedef detail::Thermostat_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit Thermostat(std::shared_ptr<ImplType> impl);

  private:

  REGISTER_LOGGER("openstudio.model.Thermostat");

};

typedef boost::optional<Thermostat> OptionalThermostat;

} // model
} // openstudio

#endif // MODEL_THERMOSTAT_HPP

