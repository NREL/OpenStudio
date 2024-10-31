/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_IMPL_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class ElectricEquipmentITEAirCooledDefinition;
  class ElectricEquipmentITEAirCooled;
  class Curve;
  class Schedule;

  namespace detail {

    /** ElectricEquipmentITEAirCooled_Impl is a SpaceLoadInstance_Impl that is the implementation class for ElectricEquipmentITEAirCooled.*/
    class MODEL_API ElectricEquipmentITEAirCooled_Impl : public SpaceLoadInstance_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricEquipmentITEAirCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricEquipmentITEAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricEquipmentITEAirCooled_Impl(const ElectricEquipmentITEAirCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricEquipmentITEAirCooled_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;
      //@}
      /** @name Getters */
      //@{

      ElectricEquipmentITEAirCooledDefinition electricEquipmentITEAirCooledDefinition() const;

      // Return the Design Power Input Schedule. If not specified, default to always 1.
      boost::optional<Schedule> designPowerInputSchedule() const;

      bool isDesignPowerInputScheduleDefaulted() const;

      // Return the CPU Loading Schedule. If not specified, default to always 1.
      boost::optional<Schedule> cPULoadingSchedule() const;

      bool isCPULoadingScheduleDefaulted() const;

      std::string cPUEndUseSubcategory() const;

      bool isCPUEndUseSubcategoryDefaulted() const;

      std::string fanEndUseSubcategory() const;

      bool isFanEndUseSubcategoryDefaulted() const;

      std::string electricPowerSupplyEndUseSubcategory() const;

      bool isElectricPowerSupplyEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setElectricEquipmentITEAirCooledDefinition(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition);

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      // Note Schedules are passed by reference, not const reference.
      bool setDesignPowerInputSchedule(Schedule& schedule);

      void resetDesignPowerInputSchedule();

      // Note Schedules are passed by reference, not const reference.
      bool setCPULoadingSchedule(Schedule& schedule);

      void resetCPULoadingSchedule();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      bool setCPUEndUseSubcategory(const std::string& cPUEndUseSubcategory);

      void resetCPUEndUseSubcategory();

      bool setFanEndUseSubcategory(const std::string& fanEndUseSubcategory);

      void resetFanEndUseSubcategory();

      bool setElectricPowerSupplyEndUseSubcategory(const std::string& electricPowerSupplyEndUseSubcategory);

      void resetElectricPowerSupplyEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> designLevel() const;

      boost::optional<double> powerPerFloorArea() const;

      double getDesignLevel(double floorArea) const;

      double getPowerPerFloorArea(double floorArea) const;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooled");

      boost::optional<ModelObject> scheduleAsModelObject() const;
      boost::optional<ModelObject> electricEquipmentITEAirCooledDefinitionAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setElectricEquipmentITEAirCooledDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_IMPL_HPP
