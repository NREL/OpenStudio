/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORWATEREQUIPMENT_IMPL_HPP
#define MODEL_EXTERIORWATEREQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class ExteriorWaterEquipmentDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    /** ExteriorWaterEquipment_Impl is a ExteriorLoadInstance_Impl that is the implementation class for ExteriorWaterEquipment.*/
    class MODEL_API ExteriorWaterEquipment_Impl : public ExteriorLoadInstance_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ExteriorWaterEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExteriorWaterEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExteriorWaterEquipment_Impl(const ExteriorWaterEquipment_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorWaterEquipment_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool setDefinition(const ExteriorLoadDefinition& definition) override;

      //@}
      /** @name Getters */
      //@{

      ExteriorWaterEquipmentDefinition exteriorWaterEquipmentDefinition() const;

      Schedule schedule() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setExteriorWaterEquipmentDefinition(const ExteriorWaterEquipmentDefinition& exteriorWaterEquipmentDefinition);

      bool setSchedule(Schedule& schedule);

      void resetSchedule();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      Facility facility() const;

      //@}
     protected:
      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.ExteriorWaterEquipment");

      boost::optional<ModelObject> exteriorWaterEquipmentDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;
      boost::optional<ModelObject> facilityAsModelObject() const;

      bool setExteriorWaterEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORWATEREQUIPMENT_IMPL_HPP
