/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~EnergyManagementSystemOutputVariable() override = default;
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
