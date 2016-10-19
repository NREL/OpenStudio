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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace energyplus { 
  class ReverseTranslator;
}

namespace model {

namespace detail {

  class EnergyManagementSystemActuator_Impl;

} // detail

/** EnergyManagementSystemActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Actuator'. */
class MODEL_API EnergyManagementSystemActuator : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemActuator(const ModelObject& modelObject, const std::string actuatedComponentType, const std::string actuatedComponentControlType);

  virtual ~EnergyManagementSystemActuator() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  ModelObject actuatedComponent() const;

  std::string actuatedComponentControlType() const;

  std::string actuatedComponentType() const;

  //@}
  /** @name Setters */
  //@{

  bool setActuatedComponent(const ModelObject& modelObject);

  bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

  bool setActuatedComponentType(const std::string& actuatedComponentType);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemActuator_Impl ImplType;

  explicit EnergyManagementSystemActuator(std::shared_ptr<detail::EnergyManagementSystemActuator_Impl> impl);

  friend class detail::EnergyManagementSystemActuator_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class energyplus::ReverseTranslator;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemActuator");
  //These are for the ReveseTranslator
  explicit EnergyManagementSystemActuator(const ModelObject& modelObject);

  explicit EnergyManagementSystemActuator(const Model& model);

};

/** \relates EnergyManagementSystemActuator*/
typedef boost::optional<EnergyManagementSystemActuator> OptionalEnergyManagementSystemActuator;

/** \relates EnergyManagementSystemActuator*/
typedef std::vector<EnergyManagementSystemActuator> EnergyManagementSystemActuatorVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP

