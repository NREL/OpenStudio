/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERUSEEQUIPMENT_IMPL_HPP
#define MODEL_WATERUSEEQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class WaterUseConnections;
  class WaterUseEquipmentDefinition;

  namespace detail {

    class MODEL_API WaterUseEquipment_Impl : public SpaceLoadInstance_Impl
    {

     public:
      WaterUseEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterUseEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterUseEquipment_Impl(const WaterUseEquipment_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterUseEquipment_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;

      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      virtual std::vector<IdfObject> remove() override;

      std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      boost::optional<Schedule> flowRateFractionSchedule() const;
      bool setFlowRateFractionSchedule(Schedule& flowRateFractionSchedule);
      void resetFlowRateFractionSchedule();

      boost::optional<WaterUseConnections> waterUseConnections() const;

      WaterUseEquipmentDefinition waterUseEquipmentDefinition() const;
      bool setWaterUseEquipmentDefinition(const WaterUseEquipmentDefinition& definition);

     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.WaterUseEquipment");

      boost::optional<ModelObject> flowRateFractionScheduleAsModelObject() const;

      bool setFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSEEQUIPMENT_IMPL_HPP
