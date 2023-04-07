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

#ifndef MODEL_AIRFLOWNETWORKHORIZONTALOPENING_HPP
#define MODEL_AIRFLOWNETWORKHORIZONTALOPENING_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkHorizontalOpening_Impl;

  }  // namespace detail

  /** AirflowNetworkHorizontalOpening is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkHorizontalOpening'. */
  class MODEL_API AirflowNetworkHorizontalOpening : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a horizontal opening object with default mass flow exponent and sloping plane angle. */
    AirflowNetworkHorizontalOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double dischargeCoefficient);
    /** Construct a horizontal opening object. */
    AirflowNetworkHorizontalOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double massFlowExponentWhenOpeningisClosed,
                                    double slopingPlaneAngle, double dischargeCoefficient);

    virtual ~AirflowNetworkHorizontalOpening() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkHorizontalOpening(const AirflowNetworkHorizontalOpening& other) = default;
    AirflowNetworkHorizontalOpening(AirflowNetworkHorizontalOpening&& other) = default;
    AirflowNetworkHorizontalOpening& operator=(const AirflowNetworkHorizontalOpening&) = default;
    AirflowNetworkHorizontalOpening& operator=(AirflowNetworkHorizontalOpening&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the mass flow coefficient when the opening is closed in kg/s-m at 1 Pa pressure difference. */
    double airMassFlowCoefficientWhenOpeningisClosed() const;
    /** Returns the mass flow exponent when the opening is closed. */
    double airMassFlowExponentWhenOpeningisClosed() const;
    /** Returns true if the  mass flow exponent is defaulted. */
    bool isAirMassFlowExponentWhenOpeningisClosedDefaulted() const;
    /** Returns the angle of the horizontal opening to a sloping plane beneath it (reducing the effective opening area)
      in degrees, 90 degrees corresponds to fully open. */
    double slopingPlaneAngle() const;
    /** Returns true if the sloping plane angle is defaulted. */
    bool isSlopingPlaneAngleDefaulted() const;
    /** Returns the discharge coefficient. */
    double dischargeCoefficient() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the mass flow coefficient when the opening is closed in kg/s-m at 1 Pa pressure difference. */
    bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);
    /** Sets the mass flow exponent when the opening is closed. */
    bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);
    /** Resets the mass flow exponent when the opening is closed to the default value. */
    void resetAirMassFlowExponentWhenOpeningisClosed();
    /** Sets the angle of the horizontal opening to a sloping plane beneath it (reducing the effective opening area)
  in degrees, 90 degrees corresponds to fully open. */
    bool setSlopingPlaneAngle(double slopingPlaneAngle);
    /** Resets the sloping plane to the default value. */
    void resetSlopingPlaneAngle();
    /** Sets the discharge coefficient. */
    bool setDischargeCoefficient(double dischargeCoefficient);

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkHorizontalOpening_Impl;

    explicit AirflowNetworkHorizontalOpening(std::shared_ptr<detail::AirflowNetworkHorizontalOpening_Impl> impl);

    friend class detail::AirflowNetworkHorizontalOpening_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkHorizontalOpening");
  };

  /** \relates AirflowNetworkHorizontalOpening*/
  using OptionalAirflowNetworkHorizontalOpening = boost::optional<AirflowNetworkHorizontalOpening>;

  /** \relates AirflowNetworkHorizontalOpening*/
  using AirflowNetworkHorizontalOpeningVector = std::vector<AirflowNetworkHorizontalOpening>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKHORIZONTALOPENING_HPP
