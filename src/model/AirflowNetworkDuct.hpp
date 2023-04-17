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

#ifndef MODEL_AIRFLOWNETWORKDUCT_HPP
#define MODEL_AIRFLOWNETWORKDUCT_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkDuct_Impl;

  }  // namespace detail

  /** AirflowNetworkDuct is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:Duct'. */
  class MODEL_API AirflowNetworkDuct : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirflowNetworkDuct(const Model& model);

    virtual ~AirflowNetworkDuct() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDuct(const AirflowNetworkDuct& other) = default;
    AirflowNetworkDuct(AirflowNetworkDuct&& other) = default;
    AirflowNetworkDuct& operator=(const AirflowNetworkDuct&) = default;
    AirflowNetworkDuct& operator=(AirflowNetworkDuct&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double ductLength() const;

    double hydraulicDiameter() const;

    double crossSectionArea() const;

    double surfaceRoughness() const;

    bool isSurfaceRoughnessDefaulted() const;

    double coefficientforLocalDynamicLossDuetoFitting() const;

    bool isCoefficientforLocalDynamicLossDuetoFittingDefaulted() const;

    double ductWallHeatTransmittanceCoefficient() const;

    bool isDuctWallHeatTransmittanceCoefficientDefaulted() const;

    double insideConvectionCoefficient() const;

    bool isInsideConvectionCoefficientDefaulted() const;

    double outsideConvectionCoefficient() const;

    bool isOutsideConvectionCoefficientDefaulted() const;

    double overallMoistureTransmittanceCoefficientfromAirtoAir() const;

    bool isOverallMoistureTransmittanceCoefficientfromAirtoAirDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDuctLength(double ductLength);

    bool setHydraulicDiameter(double hydraulicDiameter);

    bool setCrossSectionArea(double crossSectionArea);

    bool setSurfaceRoughness(double surfaceRoughness);

    void resetSurfaceRoughness();

    bool setCoefficientforLocalDynamicLossDuetoFitting(double coefficientforLocalDynamicLossDuetoFitting);

    void resetCoefficientforLocalDynamicLossDuetoFitting();

    bool setDuctWallHeatTransmittanceCoefficient(double coefficient);

    void resetDuctWallHeatTransmittanceCoefficient();

    bool setInsideConvectionCoefficient(double coefficient);

    void resetInsideConvectionCoefficient();

    bool setOutsideConvectionCoefficient(double coefficient);

    void resetOutsideConvectionCoefficient();

    bool setOverallMoistureTransmittanceCoefficientfromAirtoAir(double overallMoistureTransmittanceCoefficientfromAirtoAir);

    void resetOverallMoistureTransmittanceCoefficientfromAirtoAir();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDuct_Impl;

    explicit AirflowNetworkDuct(std::shared_ptr<detail::AirflowNetworkDuct_Impl> impl);

    friend class detail::AirflowNetworkDuct_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDuct");
  };

  /** \relates AirflowNetworkDuct*/
  using OptionalAirflowNetworkDuct = boost::optional<AirflowNetworkDuct>;

  /** \relates AirflowNetworkDuct*/
  using AirflowNetworkDuctVector = std::vector<AirflowNetworkDuct>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDUCT_HPP
