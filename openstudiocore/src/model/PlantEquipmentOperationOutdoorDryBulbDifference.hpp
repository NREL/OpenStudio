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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  class PlantEquipmentOperationOutdoorDryBulbDifference_Impl;

} // detail

/** PlantEquipmentOperationOutdoorDryBulbDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDryBulbDifference'. */
class MODEL_API PlantEquipmentOperationOutdoorDryBulbDifference : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationOutdoorDryBulbDifference(const Model& model);

  virtual ~PlantEquipmentOperationOutdoorDryBulbDifference() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Node> referenceTemperatureNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceTemperatureNode(const Node& node);

  void resetReferenceTemperatureNode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl ImplType;

  explicit PlantEquipmentOperationOutdoorDryBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl> impl);

  friend class detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDryBulbDifference");
};

/** \relates PlantEquipmentOperationOutdoorDryBulbDifference*/
typedef boost::optional<PlantEquipmentOperationOutdoorDryBulbDifference> OptionalPlantEquipmentOperationOutdoorDryBulbDifference;

/** \relates PlantEquipmentOperationOutdoorDryBulbDifference*/
typedef std::vector<PlantEquipmentOperationOutdoorDryBulbDifference> PlantEquipmentOperationOutdoorDryBulbDifferenceVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP

