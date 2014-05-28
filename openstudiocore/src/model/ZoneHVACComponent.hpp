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

#ifndef MODEL_ZONEHVACCOMPONENT_HPP
#define MODEL_ZONEHVACCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class ThermalZone;

class Node;

namespace detail {

class ZoneHVACComponent_Impl;

};

/** ZoneHVACComponent is the base class for HVAC related objects that exclusively condition a single zone.
 */
class MODEL_API ZoneHVACComponent : public ParentObject
{
  public:
  virtual ~ZoneHVACComponent() {}

  /** Returns the inlet port.  For a ZoneHVACComponent this port is typically connected
   *  to a zone exhaust node. **/
  unsigned inletPort();

  /** Returns the optional node connected to the inletPort(). **/
  boost::optional<Node> inletNode();

  /** Returns the outlet port. For a ZoneHVACComponent this port is typically connected
   *  to a zone air inlet node **/
  unsigned outletPort();

  /** Returns the optional node connected to the outletPort(). **/
  boost::optional<Node> outletNode();

  /** Returns the optional ThermalZone that this ZoneHVACComponent is attached to
   **/
  boost::optional<ThermalZone> thermalZone();

  /** Adds this ZoneHVACComponent to the thermal zone while managing all
   *  node connections automatically.  Returns true if the operation was
   *  successful.
   **/
  bool addToThermalZone(ThermalZone & thermalZone);

  /** Detaches this ZoneHVACComponent from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
  void removeFromThermalZone();

  protected:

  ZoneHVACComponent(IddObjectType type,const Model& model);

  typedef detail::ZoneHVACComponent_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  explicit ZoneHVACComponent(std::shared_ptr<ImplType> impl);

  virtual std::vector<ModelObject> children() const;

  private:

  REGISTER_LOGGER("openstudio.model.HVACComponent");

};

typedef boost::optional<ZoneHVACComponent> OptionalZoneHVACComponent;

} // model

} // openstudio

#endif // MODEL_ZONEHVACCOMPONENT_HPP

