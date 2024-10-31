/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OTHEREQUIPMENT_IMPL_HPP
#define MODEL_OTHEREQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {

class FuelType;

namespace model {

  class OtherEquipmentDefinition;
  class Schedule;

  namespace detail {

    /** OtherEquipment_Impl is a SpaceLoadInstance_Impl that is the implementation class for OtherEquipment.*/
    class MODEL_API OtherEquipment_Impl : public SpaceLoadInstance_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OtherEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OtherEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OtherEquipment_Impl(const OtherEquipment_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OtherEquipment_Impl() override = default;

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

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      std::string fuelType() const;

      bool isFuelTypeDefaulted() const;

      OtherEquipmentDefinition otherEquipmentDefinition() const;

      /** Returns the (fractional) equipment schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
      boost::optional<Schedule> schedule() const;

      /** Returns true if this object does not specify a schedule directly. */
      bool isScheduleDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      bool setFuelType(const FuelType& fuelType);

      void resetFuelType();

      bool setOtherEquipmentDefinition(const OtherEquipmentDefinition& definition);

      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      /** Sets the (fractional) Schedule. */
      bool setSchedule(Schedule& schedule);

      /** Resets the (fractional) Schedule. */
      void resetSchedule();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      //@}
      /** @name Other */
      //@{

      /** Returns the design level represented by this instance, assuming floorArea (m^2) and
     *  numPeople. */
      double getDesignLevel(double floorArea, double numPeople) const;

      /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and
     *  numPeople. */
      double getPowerPerFloorArea(double floorArea, double numPeople) const;

      /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and
     *  numPeople. */
      double getPowerPerPerson(double floorArea, double numPeople) const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.OtherEquipment");

      boost::optional<ModelObject> otherEquipmentDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setOtherEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OTHEREQUIPMENT_IMPL_HPP
