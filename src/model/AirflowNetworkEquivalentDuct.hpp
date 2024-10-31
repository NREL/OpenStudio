/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~AirflowNetworkEquivalentDuct() override = default;
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
