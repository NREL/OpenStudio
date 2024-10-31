/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORFUELEQUIPMENT_IMPL_HPP
#define MODEL_EXTERIORFUELEQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance_Impl.hpp"

namespace openstudio {

class FuelType;

namespace model {

  class ExteriorFuelEquipmentDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    /** ExteriorFuelEquipment_Impl is a ExteriorLoadInstance_Impl that is the implementation class for ExteriorFuelEquipment.*/
    class MODEL_API ExteriorFuelEquipment_Impl : public ExteriorLoadInstance_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ExteriorFuelEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExteriorFuelEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExteriorFuelEquipment_Impl(const ExteriorFuelEquipment_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorFuelEquipment_Impl() override = default;

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

      ExteriorFuelEquipmentDefinition exteriorFuelEquipmentDefinition() const;

      Schedule schedule() const;

      std::string fuelType() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setExteriorFuelEquipmentDefinition(const ExteriorFuelEquipmentDefinition& exteriorFuelEquipmentDefinition);

      bool setSchedule(Schedule& schedule);

      void resetSchedule();

      bool setFuelType(const FuelType& fuelType);

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
      REGISTER_LOGGER("openstudio.model.ExteriorFuelEquipment");

      std::vector<std::string> fuelTypeValues() const;

      boost::optional<ModelObject> exteriorFuelEquipmentDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;
      boost::optional<ModelObject> facilityAsModelObject() const;

      bool setExteriorFuelEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORFUELEQUIPMENT_IMPL_HPP
