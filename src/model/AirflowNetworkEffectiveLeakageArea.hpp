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

#ifndef MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_HPP
#define MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkEffectiveLeakageArea_Impl;

  }  // namespace detail

  /** AirflowNetworkEffectiveLeakageArea is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkEffectiveLeakageArea'. */
  class MODEL_API AirflowNetworkEffectiveLeakageArea : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct an effective leakage area object with default values and a specified effective leakage area. */
    AirflowNetworkEffectiveLeakageArea(const Model& model, double effectiveLeakageArea);
    /** Construct an effective leakage area object. */
    AirflowNetworkEffectiveLeakageArea(const Model& model, double effectiveLeakageArea, double dischargeCoefficient,
                                       double referencePressureDifference, double massFlowExponent);

    virtual ~AirflowNetworkEffectiveLeakageArea() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkEffectiveLeakageArea(const AirflowNetworkEffectiveLeakageArea& other) = default;
    AirflowNetworkEffectiveLeakageArea(AirflowNetworkEffectiveLeakageArea&& other) = default;
    AirflowNetworkEffectiveLeakageArea& operator=(const AirflowNetworkEffectiveLeakageArea&) = default;
    AirflowNetworkEffectiveLeakageArea& operator=(AirflowNetworkEffectiveLeakageArea&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the effective leakage area in square meters. */
    double effectiveLeakageArea() const;
    /** Returns the discharge coefficient. */
    double dischargeCoefficient() const;
    /** Returns true if the discharge coefficient is defaulted. */
    bool isDischargeCoefficientDefaulted() const;
    /** Returns the reference pressure difference in Pa. */
    double referencePressureDifference() const;
    /** Returns true if the reference pressure difference is defaulted. */
    bool isReferencePressureDifferenceDefaulted() const;
    /** Returns the mass flow exponent. */
    double airMassFlowExponent() const;
    /** Returns true if the mass flow exponent. */
    bool isAirMassFlowExponentDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the effective leakage area in square meters. */
    bool setEffectiveLeakageArea(double effectiveLeakageArea);
    /** Sets the discharge coefficient. */
    bool setDischargeCoefficient(double dischargeCoefficient);
    /** Returns the discharge coefficient to the default value. */
    void resetDischargeCoefficient();
    /** Sets the reference pressure difference in Pa. */
    bool setReferencePressureDifference(double referencePressureDifference);
    /** Returns the reference pressure difference to the default value. */
    void resetReferencePressureDifference();
    /** Sets the mass flow exponent. */
    bool setAirMassFlowExponent(double airMassFlowExponent);
    /** Returns the mass flow exponent to the default value. */
    void resetAirMassFlowExponent();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkEffectiveLeakageArea_Impl;

    explicit AirflowNetworkEffectiveLeakageArea(std::shared_ptr<detail::AirflowNetworkEffectiveLeakageArea_Impl> impl);

    friend class detail::AirflowNetworkEffectiveLeakageArea_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkEffectiveLeakageArea");
  };

  /** \relates AirflowNetworkEffectiveLeakageArea*/
  using OptionalAirflowNetworkEffectiveLeakageArea = boost::optional<AirflowNetworkEffectiveLeakageArea>;

  /** \relates AirflowNetworkEffectiveLeakageArea*/
  using AirflowNetworkEffectiveLeakageAreaVector = std::vector<AirflowNetworkEffectiveLeakageArea>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_HPP
