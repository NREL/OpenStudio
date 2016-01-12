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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PlantEquipmentOperationOutdoorDryBulb_Impl;

} // detail

/** PlantEquipmentOperationOutdoorDryBulb is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDryBulb'. */
class MODEL_API PlantEquipmentOperationOutdoorDryBulb : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationOutdoorDryBulb(const Model& model);

  virtual ~PlantEquipmentOperationOutdoorDryBulb() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{


  //@}
  /** @name Setters */
  //@{


  //@}
  /** @name Other */
  //@{


  //@}
 protected:
  /// @cond
  typedef detail::PlantEquipmentOperationOutdoorDryBulb_Impl ImplType;

  explicit PlantEquipmentOperationOutdoorDryBulb(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulb_Impl> impl);

  friend class detail::PlantEquipmentOperationOutdoorDryBulb_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDryBulb");
};

/** \relates PlantEquipmentOperationOutdoorDryBulb*/
typedef boost::optional<PlantEquipmentOperationOutdoorDryBulb> OptionalPlantEquipmentOperationOutdoorDryBulb;

/** \relates PlantEquipmentOperationOutdoorDryBulb*/
typedef std::vector<PlantEquipmentOperationOutdoorDryBulb> PlantEquipmentOperationOutdoorDryBulbVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_HPP

