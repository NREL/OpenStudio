/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  }  // namespace detail

  /** EnergyManagementSystemTrendVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:TrendVariable'. */
  class MODEL_API EnergyManagementSystemTrendVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    explicit EnergyManagementSystemTrendVariable(const Model& model, const std::string& eMSVariableName);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemActuator& object);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemSensor& object);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemTrendVariable& object);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemInternalVariable& object);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object);

    explicit EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object);

    virtual ~EnergyManagementSystemTrendVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemTrendVariable(const EnergyManagementSystemTrendVariable& other) = default;
    EnergyManagementSystemTrendVariable(EnergyManagementSystemTrendVariable&& other) = default;
    EnergyManagementSystemTrendVariable& operator=(const EnergyManagementSystemTrendVariable&) = default;
    EnergyManagementSystemTrendVariable& operator=(EnergyManagementSystemTrendVariable&&) = default;

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
    using ImplType = detail::EnergyManagementSystemTrendVariable_Impl;

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
  using OptionalEnergyManagementSystemTrendVariable = boost::optional<EnergyManagementSystemTrendVariable>;

  /** \relates EnergyManagementSystemTrendVariable*/
  using EnergyManagementSystemTrendVariableVector = std::vector<EnergyManagementSystemTrendVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP
