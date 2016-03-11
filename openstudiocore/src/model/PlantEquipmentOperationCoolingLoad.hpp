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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONCOOLINGLOAD_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONCOOLINGLOAD_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PlantEquipmentOperationCoolingLoad_Impl;

} // detail

/** PlantEquipmentOperationCoolingLoad is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:CoolingLoad'. */
class MODEL_API PlantEquipmentOperationCoolingLoad : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationCoolingLoad(const Model& model);

  virtual ~PlantEquipmentOperationCoolingLoad() {}

  //@}

  static IddObjectType iddObjectType();

 protected:
  /// @cond
  typedef detail::PlantEquipmentOperationCoolingLoad_Impl ImplType;

  explicit PlantEquipmentOperationCoolingLoad(std::shared_ptr<detail::PlantEquipmentOperationCoolingLoad_Impl> impl);

  friend class detail::PlantEquipmentOperationCoolingLoad_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationCoolingLoad");
};

/** \relates PlantEquipmentOperationCoolingLoad*/
typedef boost::optional<PlantEquipmentOperationCoolingLoad> OptionalPlantEquipmentOperationCoolingLoad;

/** \relates PlantEquipmentOperationCoolingLoad*/
typedef std::vector<PlantEquipmentOperationCoolingLoad> PlantEquipmentOperationCoolingLoadVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONCOOLINGLOAD_HPP

