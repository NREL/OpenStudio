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

#ifndef MODEL_AIRFLOWNETWORKREFERENCECRACKCONDITIONS_HPP
#define MODEL_AIRFLOWNETWORKREFERENCECRACKCONDITIONS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkReferenceCrackConditions_Impl;

  }  // namespace detail

  /** AirflowNetworkReferenceCrackConditions is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkReferenceCrackConditions'. */
  class MODEL_API AirflowNetworkReferenceCrackConditions : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a reference crack conditions object with default values. */
    explicit AirflowNetworkReferenceCrackConditions(const Model& model);
    /** Construct a reference crack conditions object with specified temperature, pressure, and barometric pressure. */
    AirflowNetworkReferenceCrackConditions(const Model& model, double temperature, double barometricPressure, double humidityRatio);

    virtual ~AirflowNetworkReferenceCrackConditions() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& other) = default;
    AirflowNetworkReferenceCrackConditions(AirflowNetworkReferenceCrackConditions&& other) = default;
    AirflowNetworkReferenceCrackConditions& operator=(const AirflowNetworkReferenceCrackConditions&) = default;
    AirflowNetworkReferenceCrackConditions& operator=(AirflowNetworkReferenceCrackConditions&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the reference temperature in degrees C. */
    double temperature() const;
    /** Returns true if the reference temperature is defaulted. */
    bool isTemperatureDefaulted() const;
    /** Returns the reference barometric pressure in Pa. */
    double barometricPressure() const;
    /** Returns true if the reference barometric pressure is defaulted. */
    bool isBarometricPressureDefaulted() const;
    /** Returns the reference humidity ratio in kgWater/kgDryAir. */
    double humidityRatio() const;
    /** Returns true if the reference humidity ratio is defaulted. */
    bool isHumidityRatioDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the reference temperature in degrees C. */
    void setTemperature(double referenceTemperature);
    /** Resets the reference temperature to the default. */
    void resetTemperature();
    /** Sets the the reference barometric pressure in Pa. */
    bool setBarometricPressure(double referenceBarometricPressure);
    /** Resets the reference barometric pressure to the default. */
    void resetBarometricPressure();
    /** Sets the reference humidity ratio in kgWater/kgDryAir. */
    void setHumidityRatio(double referenceHumidityRatio);
    /** Resets the reference humidity ratio to the default. */
    void resetHumidityRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkReferenceCrackConditions_Impl;

    explicit AirflowNetworkReferenceCrackConditions(std::shared_ptr<detail::AirflowNetworkReferenceCrackConditions_Impl> impl);

    friend class detail::AirflowNetworkReferenceCrackConditions_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkReferenceCrackConditions");
  };

  /** \relates AirflowNetworkReferenceCrackConditions*/
  using OptionalAirflowNetworkReferenceCrackConditions = boost::optional<AirflowNetworkReferenceCrackConditions>;

  /** \relates AirflowNetworkReferenceCrackConditions*/
  using AirflowNetworkReferenceCrackConditionsVector = std::vector<AirflowNetworkReferenceCrackConditions>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKREFERENCECRACKCONDITIONS_HPP
