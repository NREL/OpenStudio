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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class EnergyManagementSystemGlobalVariable_Impl;

} // detail

/** EnergyManagementSystemGlobalVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:GlobalVariable'. */
class MODEL_API EnergyManagementSystemGlobalVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemGlobalVariable(const Model& model, const std::string& variableName);

  virtual ~EnergyManagementSystemGlobalVariable() {}

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
  typedef detail::EnergyManagementSystemGlobalVariable_Impl ImplType;

  explicit EnergyManagementSystemGlobalVariable(std::shared_ptr<detail::EnergyManagementSystemGlobalVariable_Impl> impl);

  friend class detail::EnergyManagementSystemGlobalVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemGlobalVariable");
};

/** \relates EnergyManagementSystemGlobalVariable*/
typedef boost::optional<EnergyManagementSystemGlobalVariable> OptionalEnergyManagementSystemGlobalVariable;

/** \relates EnergyManagementSystemGlobalVariable*/
typedef std::vector<EnergyManagementSystemGlobalVariable> EnergyManagementSystemGlobalVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_HPP

