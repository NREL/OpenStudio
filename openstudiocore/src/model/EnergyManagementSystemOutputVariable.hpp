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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class EnergyManagementSystemOutputVariable_Impl;

} // detail

/** EnergyManagementSystemOutputVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:OutputVariable'. */
class MODEL_API EnergyManagementSystemOutputVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemOutputVariable(const Model& model);

  virtual ~EnergyManagementSystemOutputVariable() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> typeofDatainVariableValues();

  static std::vector<std::string> updateFrequencyValues();

  /** @name Getters */
  //@{

  std::string eMSVariableName() const;

  std::string typeofDatainVariable() const;

  std::string updateFrequency() const;

  boost::optional<std::string> eMSProgramorSubroutineName() const;

  boost::optional<std::string> units() const;

  //@}
  /** @name Setters */
  //@{

  bool setEMSVariableName(const std::string& eMSVariableName);

  bool setTypeofDatainVariable(const std::string& typeofDatainVariable);

  bool setUpdateFrequency(const std::string& updateFrequency);

  bool setEMSProgramorSubroutineName(const std::string& eMSProgramorSubroutineName);

  void resetEMSProgramorSubroutineName();

  bool setUnits(const std::string& units);

  void resetUnits();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemOutputVariable_Impl ImplType;

  explicit EnergyManagementSystemOutputVariable(std::shared_ptr<detail::EnergyManagementSystemOutputVariable_Impl> impl);

  friend class detail::EnergyManagementSystemOutputVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemOutputVariable");
};

/** \relates EnergyManagementSystemOutputVariable*/
typedef boost::optional<EnergyManagementSystemOutputVariable> OptionalEnergyManagementSystemOutputVariable;

/** \relates EnergyManagementSystemOutputVariable*/
typedef std::vector<EnergyManagementSystemOutputVariable> EnergyManagementSystemOutputVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_HPP

