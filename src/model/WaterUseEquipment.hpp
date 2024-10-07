/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERUSEEQUIPMENT_HPP
#define MODEL_WATERUSEEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;
  class WaterUseConnections;
  class WaterUseEquipmentDefinition;

  namespace detail {

    class WaterUseEquipment_Impl;

  }  // namespace detail

  /** WaterUseEquipment is a ModelObject that wraps the OpenStudio IDD object 'OS:WaterUse:Equipment'. */
  class MODEL_API WaterUseEquipment : public SpaceLoadInstance
  {
   public:
    explicit WaterUseEquipment(const WaterUseEquipmentDefinition& waterUseEquipmentDefinition);

    virtual ~WaterUseEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterUseEquipment(const WaterUseEquipment& other) = default;
    WaterUseEquipment(WaterUseEquipment&& other) = default;
    WaterUseEquipment& operator=(const WaterUseEquipment&) = default;
    WaterUseEquipment& operator=(WaterUseEquipment&&) = default;

    static IddObjectType iddObjectType();

    boost::optional<Schedule> flowRateFractionSchedule() const;
    bool setFlowRateFractionSchedule(Schedule& flowRateFractionSchedule);
    void resetFlowRateFractionSchedule();

    WaterUseEquipmentDefinition waterUseEquipmentDefinition() const;
    bool setWaterUseEquipmentDefinition(const WaterUseEquipmentDefinition& definition);

    boost::optional<WaterUseConnections> waterUseConnections() const;

   protected:
    /// @cond

    using ImplType = detail::WaterUseEquipment_Impl;

    explicit WaterUseEquipment(std::shared_ptr<detail::WaterUseEquipment_Impl> impl);

    friend class detail::WaterUseEquipment_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.WaterUseEquipment");
  };

  /** \relates WaterUseEquipment*/
  using OptionalWaterUseEquipment = boost::optional<WaterUseEquipment>;

  /** \relates WaterUseEquipment*/
  using WaterUseEquipmentVector = std::vector<WaterUseEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSEEQUIPMENT_HPP
