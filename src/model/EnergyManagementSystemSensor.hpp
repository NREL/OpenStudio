/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

  class OutputVariable;
  class OutputMeter;

  namespace detail {

    class EnergyManagementSystemSensor_Impl;

  }  // namespace detail

  /** EnergyManagementSystemSensor is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Sensor'. */
  class MODEL_API EnergyManagementSystemSensor : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemSensor(const Model& model, const OutputVariable& outvar);

    explicit EnergyManagementSystemSensor(const Model& model, const OutputMeter& outvar);

    explicit EnergyManagementSystemSensor(const Model& model, const std::string& outputVariableOrMeterName);

    virtual ~EnergyManagementSystemSensor() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemSensor(const EnergyManagementSystemSensor& other) = default;
    EnergyManagementSystemSensor(EnergyManagementSystemSensor&& other) = default;
    EnergyManagementSystemSensor& operator=(const EnergyManagementSystemSensor&) = default;
    EnergyManagementSystemSensor& operator=(EnergyManagementSystemSensor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string keyName() const;

    boost::optional<OutputVariable> outputVariable() const;
    boost::optional<OutputMeter> outputMeter() const;
    std::string outputVariableOrMeterName() const;

    //@}
    /** @name Setters */
    //@{

    bool setKeyName(const std::string& keyName);

    bool setOutputVariable(const OutputVariable& outputVariable);
    bool setOutputMeter(const OutputMeter& outputMeter);
    bool setOutputVariableOrMeterName(const std::string& outputVariableOrMeterName);
    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemSensor_Impl;

    explicit EnergyManagementSystemSensor(std::shared_ptr<detail::EnergyManagementSystemSensor_Impl> impl);

    friend class detail::EnergyManagementSystemSensor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemSensor");
    explicit EnergyManagementSystemSensor(const Model& model);
  };

  /** \relates EnergyManagementSystemSensor*/
  using OptionalEnergyManagementSystemSensor = boost::optional<EnergyManagementSystemSensor>;

  /** \relates EnergyManagementSystemSensor*/
  using EnergyManagementSystemSensorVector = std::vector<EnergyManagementSystemSensor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_HPP
