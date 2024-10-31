/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLIGHTS_IMPL_HPP
#define MODEL_EXTERIORLIGHTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class ExteriorLightsDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    /** ExteriorLights_Impl is a ExteriorLoadInstance_Impl that is the implementation class for ExteriorLights.*/
    class MODEL_API ExteriorLights_Impl : public ExteriorLoadInstance_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ExteriorLights_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExteriorLights_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExteriorLights_Impl(const ExteriorLights_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorLights_Impl() override = default;

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

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Getters */
      //@{

      ExteriorLightsDefinition exteriorLightsDefinition() const;

      boost::optional<Schedule> schedule() const;

      std::string controlOption() const;

      bool isControlOptionDefaulted() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition);

      bool setSchedule(Schedule& schedule);

      void resetSchedule();
      bool setControlOption(const std::string& controlOption);

      void resetControlOption();

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
      REGISTER_LOGGER("openstudio.model.ExteriorLights");

      std::vector<std::string> controlOptionValues() const;

      boost::optional<ModelObject> exteriorLightsDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;
      boost::optional<ModelObject> facilityAsModelObject() const;

      bool setExteriorLightsDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLIGHTS_IMPL_HPP
