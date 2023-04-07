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

    virtual ~EnergyManagementSystemSensor() = default;
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
