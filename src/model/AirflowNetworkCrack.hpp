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

#ifndef MODEL_AIRFLOWNETWORKCRACK_HPP
#define MODEL_AIRFLOWNETWORKCRACK_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkReferenceCrackConditions;

  namespace detail {

    class AirflowNetworkCrack_Impl;

  }  // namespace detail

  /** AirflowNetworkCrack is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkCrack'. */
  class MODEL_API AirflowNetworkCrack : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a surface crack object with default values and a specified mass flow coefficient. */
    AirflowNetworkCrack(const Model& model, double massFlowCoefficient);
    /** Construct a surface crack object without a reference conditions object. */
    AirflowNetworkCrack(const Model& model, double massFlowCoefficient, double massFlowExponent);
    /** Construct a surface crack object. */
    AirflowNetworkCrack(const Model& model, double massFlowCoefficient, double massFlowExponent,
                        const AirflowNetworkReferenceCrackConditions& referenceCrackConditions);

    virtual ~AirflowNetworkCrack() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkCrack(const AirflowNetworkCrack& other) = default;
    AirflowNetworkCrack(AirflowNetworkCrack&& other) = default;
    AirflowNetworkCrack& operator=(const AirflowNetworkCrack&) = default;
    AirflowNetworkCrack& operator=(AirflowNetworkCrack&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the mass flow coefficient in kg/s at 1 Pa. */
    double airMassFlowCoefficient() const;
    /** Returns the mass flow exponent. */
    double airMassFlowExponent() const;
    /** Returns true if the mass flow exponent is defaulted. */
    bool isAirMassFlowExponentDefaulted() const;
    /** Returns a reference crack conditions object if available. */
    boost::optional<AirflowNetworkReferenceCrackConditions> referenceCrackConditions() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the mass flow coefficient in kg/s at 1 Pa.  */
    bool setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions);
    /** Sets the mass flow exponent. */
    bool setAirMassFlowExponent(double airMassFlowExponent);
    /** Returns the mass flow exponent to the default value. */
    void resetAirMassFlowExponent();
    /** Sets the the reference crack conditions. */
    bool setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions);
    /** Resets the the reference crack conditions. */
    void resetReferenceCrackConditions();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkCrack_Impl;

    explicit AirflowNetworkCrack(std::shared_ptr<detail::AirflowNetworkCrack_Impl> impl);

    friend class detail::AirflowNetworkCrack_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkCrack");
  };

  /** \relates AirflowNetworkCrack*/
  using OptionalAirflowNetworkCrack = boost::optional<AirflowNetworkCrack>;

  /** \relates AirflowNetworkCrack*/
  using AirflowNetworkCrackVector = std::vector<AirflowNetworkCrack>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCRACK_HPP
