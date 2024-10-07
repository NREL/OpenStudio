/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STEAMEQUIPMENT_IMPL_HPP
#define MODEL_STEAMEQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class SteamEquipmentDefinition;
  class Schedule;

  namespace detail {

    /** SteamEquipment_Impl is a SpaceLoadInstance_Impl that is the implementation class for SteamEquipment.*/
    class MODEL_API SteamEquipment_Impl : public SpaceLoadInstance_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SteamEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SteamEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SteamEquipment_Impl(const SteamEquipment_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SteamEquipment_Impl() override = default;

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

      SteamEquipmentDefinition steamEquipmentDefinition() const;

      /** Returns the (fractional) equipment schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
      boost::optional<Schedule> schedule() const;

      /** Returns true if this object does not specify a schedule directly. */
      bool isScheduleDefaulted() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setSteamEquipmentDefinition(const SteamEquipmentDefinition& definition);

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      /** Sets the (fractional) Schedule. */
      bool setSchedule(Schedule& schedule);

      /** Resets the (fractional) Schedule. */
      void resetSchedule();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

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
      REGISTER_LOGGER("openstudio.model.SteamEquipment");

      boost::optional<ModelObject> steamEquipmentDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setSteamEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STEAMEQUIPMENT_IMPL_HPP
