/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP

#include "ModelAPI.hpp"
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

