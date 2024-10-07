/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERUSEEQUIPMENTDEFINITION_HPP
#define MODEL_WATERUSEEQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class WaterUseEquipmentDefinition_Impl;

  }  // namespace detail

  /** WaterUseEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 'OS:WaterUse:Equipment:Definition'. */
  class MODEL_API WaterUseEquipmentDefinition : public SpaceLoadDefinition
  {
   public:
    explicit WaterUseEquipmentDefinition(const Model& model);

    virtual ~WaterUseEquipmentDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterUseEquipmentDefinition(const WaterUseEquipmentDefinition& other) = default;
    WaterUseEquipmentDefinition(WaterUseEquipmentDefinition&& other) = default;
    WaterUseEquipmentDefinition& operator=(const WaterUseEquipmentDefinition&) = default;
    WaterUseEquipmentDefinition& operator=(WaterUseEquipmentDefinition&&) = default;

    static IddObjectType iddObjectType();

    std::string endUseSubcategory() const;
    bool isEndUseSubcategoryDefaulted() const;
    bool setEndUseSubcategory(const std::string& endUseSubcategory);
    void resetEndUseSubcategory();

    double peakFlowRate() const;
    bool setPeakFlowRate(double peakFlowRate);
    boost::optional<Schedule> targetTemperatureSchedule() const;
    bool setTargetTemperatureSchedule(const Schedule& targetTemperatureSchedule);
    void resetTargetTemperatureSchedule();

    boost::optional<Schedule> sensibleFractionSchedule() const;
    bool setSensibleFractionSchedule(const Schedule& sensibleFractionSchedule);
    void resetSensibleFractionSchedule();

    boost::optional<Schedule> latentFractionSchedule() const;
    bool setLatentFractionSchedule(const Schedule& latentFractionSchedule);
    void resetLatentFractionSchedule();

   protected:
    using ImplType = detail::WaterUseEquipmentDefinition_Impl;

    explicit WaterUseEquipmentDefinition(std::shared_ptr<detail::WaterUseEquipmentDefinition_Impl> impl);

    friend class detail::WaterUseEquipmentDefinition_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

   private:
    REGISTER_LOGGER("openstudio.model.WaterUseEquipmentDefinition");
  };

  using OptionalWaterUseEquipmentDefinition = boost::optional<WaterUseEquipmentDefinition>;

  using WaterUseEquipmentDefinitionVector = std::vector<WaterUseEquipmentDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSEEQUIPMENTDEFINITION_HPP
