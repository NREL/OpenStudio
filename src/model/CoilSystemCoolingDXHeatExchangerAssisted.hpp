/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_HPP
#define MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class AirToAirComponent;

  namespace detail {

    class CoilSystemCoolingDXHeatExchangerAssisted_Impl;

  }  // namespace detail

  /** CoilSystemCoolingDXHeatExchangerAssisted is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoilSystem:Cooling:DX:HeatExchangerAssisted'. */
  class MODEL_API CoilSystemCoolingDXHeatExchangerAssisted : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilSystemCoolingDXHeatExchangerAssisted(const Model& model);

    explicit CoilSystemCoolingDXHeatExchangerAssisted(const Model& model, const AirToAirComponent& heatExchanger);

    virtual ~CoilSystemCoolingDXHeatExchangerAssisted() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilSystemCoolingDXHeatExchangerAssisted(const CoilSystemCoolingDXHeatExchangerAssisted& other) = default;
    CoilSystemCoolingDXHeatExchangerAssisted(CoilSystemCoolingDXHeatExchangerAssisted&& other) = default;
    CoilSystemCoolingDXHeatExchangerAssisted& operator=(const CoilSystemCoolingDXHeatExchangerAssisted&) = default;
    CoilSystemCoolingDXHeatExchangerAssisted& operator=(CoilSystemCoolingDXHeatExchangerAssisted&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    AirToAirComponent heatExchanger() const;

    StraightComponent coolingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatExchanger(const AirToAirComponent& heatExchanger);

    bool setCoolingCoil(const StraightComponent& coolingCoil);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl;

    explicit CoilSystemCoolingDXHeatExchangerAssisted(std::shared_ptr<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl> impl);

    friend class detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilSystemCoolingDXHeatExchangerAssisted");
  };

  /** \relates CoilSystemCoolingDXHeatExchangerAssisted*/
  using OptionalCoilSystemCoolingDXHeatExchangerAssisted = boost::optional<CoilSystemCoolingDXHeatExchangerAssisted>;

  /** \relates CoilSystemCoolingDXHeatExchangerAssisted*/
  using CoilSystemCoolingDXHeatExchangerAssistedVector = std::vector<CoilSystemCoolingDXHeatExchangerAssisted>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_HPP
