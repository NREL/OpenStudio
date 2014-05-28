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

#ifndef MODEL_ZONEHVACEQUIPMENTLIST_IMPL_HPP
#define MODEL_ZONEHVACEQUIPMENTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;

namespace detail {

/** ZoneHVACEquipmentList_Impl is a ModelObject_Impl that is the implementation class for ZoneHVACEquipmentList.*/
class MODEL_API ZoneHVACEquipmentList_Impl : public ModelObject_Impl {
 public:

  ZoneHVACEquipmentList_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle);

  ZoneHVACEquipmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

  ZoneHVACEquipmentList_Impl(const ZoneHVACEquipmentList_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

  void addEquipment(const ModelObject & equipment);

  void removeEquipment(const ModelObject & equipment);

  void setCoolingPriority(const ModelObject & equipment, unsigned priority);

  void setHeatingPriority(const ModelObject & euqipment, unsigned priority);

  std::vector<ModelObject> equipment() const;

  std::vector<ModelObject> equipmentInHeatingOrder();

  std::vector<ModelObject> equipmentInCoolingOrder();

  WorkspaceExtensibleGroup getGroupForModelObject(const ModelObject & modelObject);

  virtual ~ZoneHVACEquipmentList_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  ThermalZone thermalZone() const;

  unsigned heatingPriority(const ModelObject & equipment);

  unsigned coolingPriority(const ModelObject & equipment);

  protected:

  private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACEquipmentList");
};

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACEQUIPMENTLIST_IMPL_HPP

