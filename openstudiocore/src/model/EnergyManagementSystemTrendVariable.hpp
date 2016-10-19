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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemGlobalVariable.hpp"
#include "EnergyManagementSystemTrendVariable.hpp"
#include "EnergyManagementSystemInternalVariable.hpp"
#include "EnergyManagementSystemCurveOrTableIndexVariable.hpp"
#include "EnergyManagementSystemConstructionIndexVariable.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

namespace detail {

  class EnergyManagementSystemTrendVariable_Impl;

} // detail

/** EnergyManagementSystemTrendVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:TrendVariable'. */
class MODEL_API EnergyManagementSystemTrendVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{
  explicit EnergyManagementSystemTrendVariable(const Model& model, const std::string eMSVariableName);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemActuator& object);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemSensor& object);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemTrendVariable& object);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemInternalVariable& object);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object);

  explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object);

  virtual ~EnergyManagementSystemTrendVariable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string emsVariableName() const;

  int numberOfTimestepsToBeLogged() const;

  boost::optional<ModelObject> emsVariableObject() const;

  boost::optional<EnergyManagementSystemActuator> emsActuator() const;
  boost::optional<EnergyManagementSystemSensor> emsSensor() const;
  boost::optional<EnergyManagementSystemGlobalVariable> emsGlobalVariable() const;
  boost::optional<EnergyManagementSystemTrendVariable> emsTrendVariable() const;
  boost::optional<EnergyManagementSystemInternalVariable> emsInternalVariable() const;
  boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> emsCurveOrTableIndexVariable() const;
  boost::optional<EnergyManagementSystemConstructionIndexVariable> emsConstructionIndexVariable() const;

  //@}
  /** @name Setters */
  //@{

  bool setEMSVariableName(const std::string& eMSVariableName);

  bool setNumberOfTimestepsToBeLogged(int numberofTimestepstobeLogged);

  bool setEMSVariableName(const EnergyManagementSystemActuator& object);
  bool setEMSVariableName(const EnergyManagementSystemSensor& object);
  bool setEMSVariableName(const EnergyManagementSystemGlobalVariable& object);
  bool setEMSVariableName(const EnergyManagementSystemTrendVariable& object);
  bool setEMSVariableName(const EnergyManagementSystemInternalVariable& object);
  bool setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object);
  bool setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemTrendVariable_Impl ImplType;

  explicit EnergyManagementSystemTrendVariable(std::shared_ptr<detail::EnergyManagementSystemTrendVariable_Impl> impl);

  friend class detail::EnergyManagementSystemTrendVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class energyplus::ReverseTranslator;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemTrendVariable");
  explicit EnergyManagementSystemTrendVariable(const Model& model);
};

/** \relates EnergyManagementSystemTrendVariable*/
typedef boost::optional<EnergyManagementSystemTrendVariable> OptionalEnergyManagementSystemTrendVariable;

/** \relates EnergyManagementSystemTrendVariable*/
typedef std::vector<EnergyManagementSystemTrendVariable> EnergyManagementSystemTrendVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP

