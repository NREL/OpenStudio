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

#ifndef MODEL_ZONEHVACEQUIPMENTLIST_HPP
#define MODEL_ZONEHVACEQUIPMENTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ZoneHVACEquipmentList_Impl;

} // detail

class ThermalZone;

/** ZoneHVACEquipmentList is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EquipmentList'. */
class MODEL_API ZoneHVACEquipmentList : public ModelObject {
  public:

  explicit ZoneHVACEquipmentList(const ThermalZone & thermalZone);

  virtual ~ZoneHVACEquipmentList() {}

  static IddObjectType iddObjectType();

  /** Add new equipment setting the heating and cooling priorities 
   *  to the next available priority level.
   *  Air terminals associated with AirLoopHVAC will be moved to first priority.
   *  This method is relatively dumb.  It will add any model object to the list
   *  even if it is not hvac equipment.  That might change in the future.
   */
  void addEquipment(const ModelObject & equipment);

  /** Remove equipment from the EquipmentList.
    * This will not remove the equipment from the model or 
    * disconnect any node connections.  Use only if you know what you are doing.
    */
  void removeEquipment(const ModelObject & equipment);

  /** Set cooling priority of equipment.
   *  Asserts when equipment is not in the ZoneHVACEquipmentList
   */
  void setCoolingPriority(const ModelObject & equipment, unsigned priority);

  /** Set heating priority of equipment.
   *  Asserts when equipment is not in the ZoneHVACEquipmentList
   */
  void setHeatingPriority(const ModelObject & euqipment, unsigned priority);

  /** Return all equipment.  Order is undetermined. */
  std::vector<ModelObject> equipment() const;

  /** Return all equipment.  Order is determined by heating priority */
  std::vector<ModelObject> equipmentInHeatingOrder();

  /** Return all equipment.  Order is determined by coooling priority */
  std::vector<ModelObject> equipmentInCoolingOrder();

  ThermalZone thermalZone() const;

  /** Return the heating priority for equipment */
  unsigned heatingPriority(const ModelObject & equipment);

  /** Return the cooling priority for equipment */
  unsigned coolingPriority(const ModelObject & equipment);

  protected:

  /// @cond

  typedef detail::ZoneHVACEquipmentList_Impl ImplType;

  explicit ZoneHVACEquipmentList(std::shared_ptr<detail::ZoneHVACEquipmentList_Impl> impl);

  friend class detail::ZoneHVACEquipmentList_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACEquipmentList");
};

/** \relates ZoneHVACEquipmentList*/
typedef boost::optional<ZoneHVACEquipmentList> OptionalZoneHVACEquipmentList;

/** \relates ZoneHVACEquipmentList*/
typedef std::vector<ZoneHVACEquipmentList> ZoneHVACEquipmentListVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACEQUIPMENTLIST_HPP

