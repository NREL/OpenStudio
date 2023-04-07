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

#ifndef MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_HPP
#define MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  class StraightComponent;

  // TODO: Check the following class names against object getters and setters.
  // AFNTerminalUnit;
  // AFNCoilNames
  // AFNHeatExchangerNames
  //   HeatExchanger:AirToAir:FlatPlate

  namespace detail {

    class HeatExchangerAirToAirSensibleAndLatent_Impl;
    class HeatExchangerDesiccantBalancedFlow_Impl;
    class CoilHeatingDXMultiSpeed_Impl;
    class CoilHeatingWater_Impl;
    class CoilHeatingDesuperheater_Impl;
    class CoilHeatingGas_Impl;
    class CoilHeatingElectric_Impl;
    class CoilHeatingDXSingleSpeed_Impl;
    class CoilCoolingWater_Impl;
    class CoilCoolingDXMultiSpeed_Impl;
    class CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
    class CoilHeatingDXSingleSpeed_Impl;
    class CoilCoolingDXSingleSpeed_Impl;
    class AirTerminalSingleDuctConstantVolumeReheat_Impl;
    class AirTerminalSingleDuctVAVReheat_Impl;
    class AirflowNetworkEquivalentDuct_Impl;

  }  // namespace detail

  /** AirflowNetworkEquivalentDuct is a AirflowNetworkComponent that wraps the OpenStudio IDD object 'OS:AirflowNetwork:EquivalentDuct'. */
  class MODEL_API AirflowNetworkEquivalentDuct : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AirflowNetworkEquivalentDuct() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkEquivalentDuct(const AirflowNetworkEquivalentDuct& other) = default;
    AirflowNetworkEquivalentDuct(AirflowNetworkEquivalentDuct&& other) = default;
    AirflowNetworkEquivalentDuct& operator=(const AirflowNetworkEquivalentDuct&) = default;
    AirflowNetworkEquivalentDuct& operator=(AirflowNetworkEquivalentDuct&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<StraightComponent> straightComponent() const;
    boost::optional<std::string> coilObjectType() const;
    boost::optional<std::string> heatExchangerObjectType() const;
    boost::optional<std::string> terminalUnitObjectType() const;

    double airPathLength() const;

    double airPathHydraulicDiameter() const;

    //@}
    /** @name Setters */
    //@{

    bool setAirPathLength(double airPathLength);

    bool setAirPathHydraulicDiameter(double airPathHydraulicDiameter);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkEquivalentDuct_Impl;

    explicit AirflowNetworkEquivalentDuct(std::shared_ptr<detail::AirflowNetworkEquivalentDuct_Impl> impl);

    AirflowNetworkEquivalentDuct(const Model& model, double length, double diameter, const Handle& handle);

    void resetComponent();

    friend class detail::AirTerminalSingleDuctConstantVolumeReheat_Impl;
    friend class detail::AirTerminalSingleDuctVAVReheat_Impl;
    friend class detail::CoilCoolingDXSingleSpeed_Impl;
    friend class detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
    friend class detail::CoilCoolingDXMultiSpeed_Impl;
    friend class detail::CoilCoolingWater_Impl;
    friend class detail::CoilHeatingDXSingleSpeed_Impl;
    friend class detail::CoilHeatingElectric_Impl;
    friend class detail::CoilHeatingGas_Impl;
    friend class detail::CoilHeatingDesuperheater_Impl;
    friend class detail::CoilHeatingWater_Impl;
    friend class detail::CoilHeatingDXMultiSpeed_Impl;
    friend class detail::HeatExchangerAirToAirSensibleAndLatent_Impl;
    friend class detail::HeatExchangerDesiccantBalancedFlow_Impl;

    friend class detail::AirflowNetworkEquivalentDuct_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkEquivalentDuct");
  };

  /** \relates AirflowNetworkEquivalentDuct*/
  using OptionalAirflowNetworkEquivalentDuct = boost::optional<AirflowNetworkEquivalentDuct>;

  /** \relates AirflowNetworkEquivalentDuct*/
  using AirflowNetworkEquivalentDuctVector = std::vector<AirflowNetworkEquivalentDuct>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_HPP
