/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"

namespace openstudio {
namespace model {

  class OutputVariable;
  class OutputMeter;

  namespace detail {

    /** EnergyManagementSystemSensor_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemSensor.*/
    class MODEL_API EnergyManagementSystemSensor_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemSensor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemSensor_Impl(const EnergyManagementSystemSensor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemSensor_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemSensor");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<OutputVariable> optionalOutputVariable() const;
      boost::optional<OutputMeter> optionalOutputMeter() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMSENSOR_IMPL_HPP
