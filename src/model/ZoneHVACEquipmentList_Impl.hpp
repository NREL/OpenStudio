/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACEQUIPMENTLIST_IMPL_HPP
#define MODEL_ZONEHVACEQUIPMENTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class ModelExtensibleGroup;

  namespace detail {

    /** ZoneHVACEquipmentList_Impl is a ModelObject_Impl that is the implementation class for ZoneHVACEquipmentList.*/
    class MODEL_API ZoneHVACEquipmentList_Impl : public ModelObject_Impl
    {
     public:
      ZoneHVACEquipmentList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACEquipmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACEquipmentList_Impl(const ZoneHVACEquipmentList_Impl& other, Model_Impl* model, bool keepHandle);

      bool addEquipment(const ModelObject& equipment);

      bool removeEquipment(const ModelObject& equipment);

      bool setCoolingPriority(const ModelObject& equipment, unsigned priority);

      bool setHeatingPriority(const ModelObject& equipment, unsigned priority);

      std::vector<ModelObject> equipment() const;

      std::vector<ModelObject> equipmentInHeatingOrder() const;

      std::vector<ModelObject> equipmentInCoolingOrder() const;

      boost::optional<ModelExtensibleGroup> getGroupForModelObject(const ModelObject& modelObject) const;

      virtual ~ZoneHVACEquipmentList_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      std::string loadDistributionScheme() const;

      bool setLoadDistributionScheme(const std::string& scheme);

      ThermalZone thermalZone() const;

      unsigned heatingPriority(const ModelObject& equipment) const;

      unsigned coolingPriority(const ModelObject& equipment) const;

      boost::optional<Schedule> sequentialCoolingFractionSchedule(const ModelObject& equipment) const;
      bool setSequentialCoolingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

      boost::optional<Schedule> sequentialHeatingFractionSchedule(const ModelObject& equipment) const;
      bool setSequentialHeatingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

      // Deprecated
      boost::optional<double> sequentialCoolingFraction(const ModelObject& equipment) const;
      bool setSequentialCoolingFraction(const ModelObject& equipment, double fraction);

      boost::optional<double> sequentialHeatingFraction(const ModelObject& equipment) const;
      bool setSequentialHeatingFraction(const ModelObject& equipment, double fraction);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACEquipmentList");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACEQUIPMENTLIST_IMPL_HPP
