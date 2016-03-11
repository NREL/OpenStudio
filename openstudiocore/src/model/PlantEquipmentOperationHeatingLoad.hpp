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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PlantEquipmentOperationHeatingLoad_Impl;

} // detail

/** PlantEquipmentOperationHeatingLoad is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:HeatingLoad'. */
class MODEL_API PlantEquipmentOperationHeatingLoad : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationHeatingLoad(const Model& model);

  virtual ~PlantEquipmentOperationHeatingLoad() {}

  //@}

  static IddObjectType iddObjectType();

 protected:
  /// @cond
  typedef detail::PlantEquipmentOperationHeatingLoad_Impl ImplType;

  explicit PlantEquipmentOperationHeatingLoad(std::shared_ptr<detail::PlantEquipmentOperationHeatingLoad_Impl> impl);

  friend class detail::PlantEquipmentOperationHeatingLoad_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationHeatingLoad");
};

/** \relates PlantEquipmentOperationHeatingLoad*/
typedef boost::optional<PlantEquipmentOperationHeatingLoad> OptionalPlantEquipmentOperationHeatingLoad;

/** \relates PlantEquipmentOperationHeatingLoad*/
typedef std::vector<PlantEquipmentOperationHeatingLoad> PlantEquipmentOperationHeatingLoadVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_HPP

