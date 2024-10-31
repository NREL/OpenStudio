/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERUSEEQUIPMENTDEFINITION_IMPL_HPP
#define MODEL_WATERUSEEQUIPMENTDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** WaterUseEquipmentDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for WaterUseEquipmentDefinition.*/
    class MODEL_API WaterUseEquipmentDefinition_Impl : public SpaceLoadDefinition_Impl
    {
     public:
      WaterUseEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterUseEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterUseEquipmentDefinition_Impl(const WaterUseEquipmentDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterUseEquipmentDefinition_Impl() override = default;

      // TODO: remove (unused)
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      std::string endUseSubcategory() const;
      bool isEndUseSubcategoryDefaulted() const;
      bool setEndUseSubcategory(const std::string& endUseSubcategory);
      void resetEndUseSubcategory();

      double peakFlowRate() const;
      bool setPeakFlowRate(double peakFlowRate);
      boost::optional<Schedule> targetTemperatureSchedule() const;
      bool setTargetTemperatureSchedule(const boost::optional<Schedule>& targetTemperatureSchedule);
      void resetTargetTemperatureSchedule();

      boost::optional<Schedule> sensibleFractionSchedule() const;
      bool setSensibleFractionSchedule(const boost::optional<Schedule>& sensibleFractionSchedule);
      void resetSensibleFractionSchedule();

      boost::optional<Schedule> latentFractionSchedule() const;
      bool setLatentFractionSchedule(const boost::optional<Schedule>& latentFractionSchedule);
      void resetLatentFractionSchedule();

     private:
      REGISTER_LOGGER("openstudio.model.WaterUseEquipmentDefinition");

      boost::optional<ModelObject> targetTemperatureScheduleAsModelObject() const;
      bool setTargetTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

      boost::optional<ModelObject> sensibleFractionScheduleAsModelObject() const;
      bool setSensibleFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

      boost::optional<ModelObject> latentFractionScheduleAsModelObject() const;
      bool setLatentFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSEEQUIPMENTDEFINITION_IMPL_HPP
