/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERUSECONNECTIONS_HPP
#define MODEL_WATERUSECONNECTIONS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class WaterUseEquipment;

  namespace detail {

    class WaterUseConnections_Impl;

  }  // namespace detail

  /** WaterUseConnections is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:WaterUse:Connections'. */
  class MODEL_API WaterUseConnections : public StraightComponent
  {

   public:
    explicit WaterUseConnections(const Model& model);

    virtual ~WaterUseConnections() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterUseConnections(const WaterUseConnections& other) = default;
    WaterUseConnections(WaterUseConnections&& other) = default;
    WaterUseConnections& operator=(const WaterUseConnections&) = default;
    WaterUseConnections& operator=(WaterUseConnections&&) = default;

    static IddObjectType iddObjectType();

    boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

    bool setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule);

    void resetHotWaterSupplyTemperatureSchedule();

    boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

    bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

    void resetColdWaterSupplyTemperatureSchedule();

    std::vector<WaterUseEquipment> waterUseEquipment() const;

    bool addWaterUseEquipment(const WaterUseEquipment& waterUseEquipment);

    // Detaches waterUseEquipment from this object but does not remove it from the model
    bool removeWaterUseEquipment(WaterUseEquipment& waterUseEquipment);

    unsigned inletPort() const;

    unsigned outletPort() const;

    std::string drainWaterHeatExchangerType() const;

    bool setDrainWaterHeatExchangerType(const std::string& drainWaterHeatExchangerType);

    std::string drainWaterHeatExchangerDestination() const;

    bool setDrainWaterHeatExchangerDestination(const std::string& drainWaterHeatExchangerDestination);

    boost::optional<double> drainWaterHeatExchangerUFactorTimesArea() const;

    bool setDrainWaterHeatExchangerUFactorTimesArea(double drainWaterHeatExchangerUFactorTimesArea);

    void resetDrainWaterHeatExchangerUFactorTimesArea();

   protected:
    /// @cond
    using ImplType = detail::WaterUseConnections_Impl;

    explicit WaterUseConnections(std::shared_ptr<detail::WaterUseConnections_Impl> impl);

    friend class detail::WaterUseConnections_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.WaterUseConnections");
  };

  /** \relates WaterUseConnections*/
  using OptionalWaterUseConnections = boost::optional<WaterUseConnections>;

  /** \relates WaterUseConnections*/
  using WaterUseConnectionsVector = std::vector<WaterUseConnections>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSECONNECTIONS_HPP
