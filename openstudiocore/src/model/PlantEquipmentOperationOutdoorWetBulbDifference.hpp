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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULBDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULBDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  class PlantEquipmentOperationOutdoorWetBulbDifference_Impl;

} // detail

/** PlantEquipmentOperationOutdoorWetBulbDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorWetBulbDifference'. */
class MODEL_API PlantEquipmentOperationOutdoorWetBulbDifference : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationOutdoorWetBulbDifference(const Model& model);

  virtual ~PlantEquipmentOperationOutdoorWetBulbDifference() {}

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
  typedef detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl ImplType;

  explicit PlantEquipmentOperationOutdoorWetBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl> impl);

  friend class detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorWetBulbDifference");
};

/** \relates PlantEquipmentOperationOutdoorWetBulbDifference*/
typedef boost::optional<PlantEquipmentOperationOutdoorWetBulbDifference> OptionalPlantEquipmentOperationOutdoorWetBulbDifference;

/** \relates PlantEquipmentOperationOutdoorWetBulbDifference*/
typedef std::vector<PlantEquipmentOperationOutdoorWetBulbDifference> PlantEquipmentOperationOutdoorWetBulbDifferenceVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULBDIFFERENCE_HPP

