/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_HPP
#define MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class AirToAirComponent;
  class WaterToAirComponent;

  namespace detail {

    class CoilSystemCoolingWaterHeatExchangerAssisted_Impl;

  }  // namespace detail

  /** CoilSystemCoolingWaterHeatExchangerAssisted is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoilSystem:Cooling:Water:HeatExchangerAssisted'. */
  class MODEL_API CoilSystemCoolingWaterHeatExchangerAssisted : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // This constructor will also create the underlying objects CoilCoolingWater and HeatExchangerAirToAirSensibleAndLatent
    explicit CoilSystemCoolingWaterHeatExchangerAssisted(const Model& model);

    explicit CoilSystemCoolingWaterHeatExchangerAssisted(const Model& model, const AirToAirComponent& heatExchanger);

    virtual ~CoilSystemCoolingWaterHeatExchangerAssisted() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilSystemCoolingWaterHeatExchangerAssisted(const CoilSystemCoolingWaterHeatExchangerAssisted& other) = default;
    CoilSystemCoolingWaterHeatExchangerAssisted(CoilSystemCoolingWaterHeatExchangerAssisted&& other) = default;
    CoilSystemCoolingWaterHeatExchangerAssisted& operator=(const CoilSystemCoolingWaterHeatExchangerAssisted&) = default;
    CoilSystemCoolingWaterHeatExchangerAssisted& operator=(CoilSystemCoolingWaterHeatExchangerAssisted&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    AirToAirComponent heatExchanger() const;

    WaterToAirComponent coolingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatExchanger(const AirToAirComponent& heatExchanger);

    bool setCoolingCoil(const WaterToAirComponent& coolingCoil);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl;

    explicit CoilSystemCoolingWaterHeatExchangerAssisted(std::shared_ptr<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl> impl);

    friend class detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilSystemCoolingWaterHeatExchangerAssisted");
  };

  /** \relates CoilSystemCoolingWaterHeatExchangerAssisted*/
  using OptionalCoilSystemCoolingWaterHeatExchangerAssisted = boost::optional<CoilSystemCoolingWaterHeatExchangerAssisted>;

  /** \relates CoilSystemCoolingWaterHeatExchangerAssisted*/
  using CoilSystemCoolingWaterHeatExchangerAssistedVector = std::vector<CoilSystemCoolingWaterHeatExchangerAssisted>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_HPP
