/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_HPP

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

    class EnergyManagementSystemMeteredOutputVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemMeteredOutputVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:MeteredOutputVariable'. */
  class MODEL_API EnergyManagementSystemMeteredOutputVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const std::string eMSVariableName);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemActuator& object);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemSensor& object);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemTrendVariable& object);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemInternalVariable& object);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object);

    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object);

    virtual ~EnergyManagementSystemMeteredOutputVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemMeteredOutputVariable(const EnergyManagementSystemMeteredOutputVariable& other) = default;
    EnergyManagementSystemMeteredOutputVariable(EnergyManagementSystemMeteredOutputVariable&& other) = default;
    EnergyManagementSystemMeteredOutputVariable& operator=(const EnergyManagementSystemMeteredOutputVariable&) = default;
    EnergyManagementSystemMeteredOutputVariable& operator=(EnergyManagementSystemMeteredOutputVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> updateFrequencyValues();

    static std::vector<std::string> resourceTypeValues();

    static std::vector<std::string> groupTypeValues();

    static std::vector<std::string> endUseCategoryValues();

    /** @name Getters */
    //@{

    std::string emsVariableName() const;

    std::string updateFrequency() const;

    std::string emsProgramOrSubroutineName() const;

    boost::optional<EnergyManagementSystemProgram> emsProgram() const;

    boost::optional<EnergyManagementSystemSubroutine> emsSubroutine() const;

    std::string resourceType() const;

    std::string groupType() const;

    std::string endUseCategory() const;

    std::string endUseSubcategory() const;

    std::string units() const;

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

    bool setUpdateFrequency(const std::string& updateFrequency);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

    void resetEMSProgramOrSubroutineName();

    bool setResourceType(const std::string& resourceType);

    bool setGroupType(const std::string& groupType);

    bool setEndUseCategory(const std::string& endUseCategory);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setUnits(const std::string& units);

    void resetUnits();

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
    using ImplType = detail::EnergyManagementSystemMeteredOutputVariable_Impl;

    explicit EnergyManagementSystemMeteredOutputVariable(std::shared_ptr<detail::EnergyManagementSystemMeteredOutputVariable_Impl> impl);

    friend class detail::EnergyManagementSystemMeteredOutputVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemMeteredOutputVariable");
    explicit EnergyManagementSystemMeteredOutputVariable(const Model& model);
  };

  /** \relates EnergyManagementSystemMeteredOutputVariable*/
  using OptionalEnergyManagementSystemMeteredOutputVariable = boost::optional<EnergyManagementSystemMeteredOutputVariable>;

  /** \relates EnergyManagementSystemMeteredOutputVariable*/
  using EnergyManagementSystemMeteredOutputVariableVector = std::vector<EnergyManagementSystemMeteredOutputVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_HPP
