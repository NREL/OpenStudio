/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONTROLLERMECHANICALVENTILATION_IMPL_HPP
#define MODEL_CONTROLLERMECHANICALVENTILATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ControllerOutdoorAir;

  namespace detail {

    /** ControllerMechanicalVentilation_Impl is a ModelObject_Impl that is the implementation class for ControllerMechanicalVentilation.*/
    class MODEL_API ControllerMechanicalVentilation_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ControllerMechanicalVentilation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ControllerMechanicalVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ControllerMechanicalVentilation_Impl(const ControllerMechanicalVentilation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ControllerMechanicalVentilation_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      bool demandControlledVentilation() const;

      bool isDemandControlledVentilationDefaulted() const;

      std::string systemOutdoorAirMethod() const;

      bool isSystemOutdoorAirMethodDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setDemandControlledVentilation(bool demandControlledVentilation);

      void resetDemandControlledVentilation();

      bool setSystemOutdoorAirMethod(const std::string& systemOutdoorAirMethod);

      void resetSystemOutdoorAirMethod();

      //@}
      /** @name Other */
      //@{

      ControllerOutdoorAir controllerOutdoorAir() const;
      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ControllerMechanicalVentilation");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      std::vector<std::string> systemOutdoorAirMethodValues() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONTROLLERMECHANICALVENTILATION_IMPL_HPP
