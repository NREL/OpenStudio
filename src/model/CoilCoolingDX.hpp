/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDX_HPP
#define MODEL_COILCOOLINGDX_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class CoilCoolingDXCurveFitPerformance;

  namespace detail {

    class CoilCoolingDX_Impl;

  }  // namespace detail

  /** CoilCoolingDX is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX'. */
  class MODEL_API CoilCoolingDX : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDX(const Model& model, const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance);

    virtual ~CoilCoolingDX() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDX(const CoilCoolingDX& other) = default;
    CoilCoolingDX(CoilCoolingDX&& other) = default;
    CoilCoolingDX& operator=(const CoilCoolingDX&) = default;
    CoilCoolingDX& operator=(CoilCoolingDX&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<ThermalZone> condenserZone() const;

    CoilCoolingDXCurveFitPerformance performanceObject() const;

    // boost::optional<HVACComponent> condensateCollectionWaterStorageTank() const;
    // boost::optional<HVACComponent> evaporativeCondenserSupplyWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    // TODO: As of E+ 9.3.0, this field appears to have no effect.
    bool setCondenserZone(const ThermalZone& thermalZone);
    void resetCondenserZone();

    // TODO: Eventually provide an API to set these Nodes once E+ is done implementing support for it (as of 9.3.0 it's not the case)
    // boost::optional<std::string> condenserInletNodeName() const;
    // bool setCondenserInletNodeName(const std::string& condenserInletNodeName);
    // void resetCondenserInletNodeName();

    // boost::optional<std::string> condenserOutletNodeName() const;
    // bool setCondenserOutletNodeName(const std::string& condenserOutletNodeName);
    // void resetCondenserOutletNodeName();

    bool setPerformanceObject(const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance);

    // bool setCondensateCollectionWaterStorageTank(const HVACComponent& condensateCollectionWaterStorageTank);
    // void resetCondensateCollectionWaterStorageTank();

    // bool setEvaporativeCondenserSupplyWaterStorageTank(const HVACComponent& evaporativeCondenserSupplyWaterStorageTank);
    // void resetEvaporativeCondenserSupplyWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDX_Impl;

    explicit CoilCoolingDX(std::shared_ptr<detail::CoilCoolingDX_Impl> impl);

    friend class detail::CoilCoolingDX_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDX");
  };

  /** \relates CoilCoolingDX*/
  using OptionalCoilCoolingDX = boost::optional<CoilCoolingDX>;

  /** \relates CoilCoolingDX*/
  using CoilCoolingDXVector = std::vector<CoilCoolingDX>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDX_HPP
