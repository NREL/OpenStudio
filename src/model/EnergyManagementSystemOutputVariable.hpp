/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemSubroutine.hpp"
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

    class EnergyManagementSystemOutputVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemOutputVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:OutputVariable'. */
  class MODEL_API EnergyManagementSystemOutputVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    explicit EnergyManagementSystemOutputVariable(const Model& model, const std::string eMSVariableName);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemActuator& object);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemSensor& object);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemTrendVariable& object);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemInternalVariable& object);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object);

    explicit EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object);

    virtual ~EnergyManagementSystemOutputVariable() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemOutputVariable(const EnergyManagementSystemOutputVariable& other) = default;
    EnergyManagementSystemOutputVariable(EnergyManagementSystemOutputVariable&& other) = default;
    EnergyManagementSystemOutputVariable& operator=(const EnergyManagementSystemOutputVariable&) = default;
    EnergyManagementSystemOutputVariable& operator=(EnergyManagementSystemOutputVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> typeOfDataInVariableValues();

    static std::vector<std::string> updateFrequencyValues();

    /** @name Getters */
    //@{

    std::string emsVariableName() const;

    std::string typeOfDataInVariable() const;

    std::string updateFrequency() const;

    std::string emsProgramOrSubroutineName() const;

    boost::optional<EnergyManagementSystemProgram> emsProgram() const;

    boost::optional<EnergyManagementSystemSubroutine> emsSubroutine() const;

    std::string units() const;

    boost::optional<ModelObject> emsVariableObject() const;

    boost::optional<EnergyManagementSystemActuator> emsActuator() const;
    boost::optional<EnergyManagementSystemSensor> emsSensor() const;
    boost::optional<EnergyManagementSystemGlobalVariable> emsGlobalVariable() const;
    boost::optional<EnergyManagementSystemTrendVariable> emsTrendVariable() const;
    boost::optional<EnergyManagementSystemInternalVariable> emsInternalVariable() const;
    boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> emsCurveOrTableIndexVariable() const;
    boost::optional<EnergyManagementSystemConstructionIndexVariable> emsConstructionIndexVariable() const;

    bool exportToBCVTB() const;

    bool isExportToBCVTBDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setEMSVariableName(const std::string& eMSVariableName);

    bool setTypeOfDataInVariable(const std::string& typeofDatainVariable);

    bool setUpdateFrequency(const std::string& updateFrequency);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

    void resetEMSProgramOrSubroutineName();

    bool setUnits(const std::string& units);

    void resetUnits();

    bool setEMSVariableName(const EnergyManagementSystemActuator& object);
    bool setEMSVariableName(const EnergyManagementSystemSensor& object);
    bool setEMSVariableName(const EnergyManagementSystemGlobalVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemTrendVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemInternalVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object);
    bool setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object);

    bool setExportToBCVTB(bool exportToBCVTB);

    void resetExportToBCVTB();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemOutputVariable_Impl;

    explicit EnergyManagementSystemOutputVariable(std::shared_ptr<detail::EnergyManagementSystemOutputVariable_Impl> impl);

    friend class detail::EnergyManagementSystemOutputVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemOutputVariable");
    explicit EnergyManagementSystemOutputVariable(const Model& model);
  };

  /** \relates EnergyManagementSystemOutputVariable*/
  using OptionalEnergyManagementSystemOutputVariable = boost::optional<EnergyManagementSystemOutputVariable>;

  /** \relates EnergyManagementSystemOutputVariable*/
  using EnergyManagementSystemOutputVariableVector = std::vector<EnergyManagementSystemOutputVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_HPP
