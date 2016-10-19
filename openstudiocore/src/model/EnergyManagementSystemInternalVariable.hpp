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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

namespace detail {

  class EnergyManagementSystemInternalVariable_Impl;

} // detail

/** EnergyManagementSystemInternalVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:InternalVariable'. */
class MODEL_API EnergyManagementSystemInternalVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{
  explicit EnergyManagementSystemInternalVariable(const Model& model, const std::string internalDataType);

  virtual ~EnergyManagementSystemInternalVariable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string internalDataIndexKeyName() const;

  std::string internalDataType() const;

  //@}
  /** @name Setters */
  //@{

  bool setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName);

  void resetInternalDataIndexKeyName();

  bool setInternalDataType(const std::string& internalDataType);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemInternalVariable_Impl ImplType;

  explicit EnergyManagementSystemInternalVariable(std::shared_ptr<detail::EnergyManagementSystemInternalVariable_Impl> impl);

  friend class detail::EnergyManagementSystemInternalVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class energyplus::ReverseTranslator;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemInternalVariable");
  explicit EnergyManagementSystemInternalVariable(const Model& model);
};

/** \relates EnergyManagementSystemInternalVariable*/
typedef boost::optional<EnergyManagementSystemInternalVariable> OptionalEnergyManagementSystemInternalVariable;

/** \relates EnergyManagementSystemInternalVariable*/
typedef std::vector<EnergyManagementSystemInternalVariable> EnergyManagementSystemInternalVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP

