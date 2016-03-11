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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  class PlantEquipmentOperationOutdoorDewpointDifference_Impl;

} // detail

/** PlantEquipmentOperationOutdoorDewpointDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDewpointDifference'. */
class MODEL_API PlantEquipmentOperationOutdoorDewpointDifference : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationOutdoorDewpointDifference(const Model& model);

  virtual ~PlantEquipmentOperationOutdoorDewpointDifference() {}

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
  typedef detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl ImplType;

  explicit PlantEquipmentOperationOutdoorDewpointDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl> impl);

  friend class detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDewpointDifference");
};

/** \relates PlantEquipmentOperationOutdoorDewpointDifference*/
typedef boost::optional<PlantEquipmentOperationOutdoorDewpointDifference> OptionalPlantEquipmentOperationOutdoorDewpointDifference;

/** \relates PlantEquipmentOperationOutdoorDewpointDifference*/
typedef std::vector<PlantEquipmentOperationOutdoorDewpointDifference> PlantEquipmentOperationOutdoorDewpointDifferenceVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_HPP

