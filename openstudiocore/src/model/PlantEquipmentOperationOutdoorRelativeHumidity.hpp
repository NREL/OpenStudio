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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PlantEquipmentOperationOutdoorRelativeHumidity_Impl;

} // detail

/** PlantEquipmentOperationOutdoorRelativeHumidity is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorRelativeHumidity'. */
class MODEL_API PlantEquipmentOperationOutdoorRelativeHumidity : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationOutdoorRelativeHumidity(const Model& model);

  virtual ~PlantEquipmentOperationOutdoorRelativeHumidity() {}

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
  typedef detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl ImplType;

  explicit PlantEquipmentOperationOutdoorRelativeHumidity(std::shared_ptr<detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl> impl);

  friend class detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorRelativeHumidity");
};

/** \relates PlantEquipmentOperationOutdoorRelativeHumidity*/
typedef boost::optional<PlantEquipmentOperationOutdoorRelativeHumidity> OptionalPlantEquipmentOperationOutdoorRelativeHumidity;

/** \relates PlantEquipmentOperationOutdoorRelativeHumidity*/
typedef std::vector<PlantEquipmentOperationOutdoorRelativeHumidity> PlantEquipmentOperationOutdoorRelativeHumidityVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP

