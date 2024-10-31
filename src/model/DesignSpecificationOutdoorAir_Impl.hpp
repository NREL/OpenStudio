/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DESIGNSPECIFICATIONOUTDOORAIR_IMPL_HPP
#define MODEL_DESIGNSPECIFICATIONOUTDOORAIR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** DesignSpecificationOutdoorAir_Impl is a ModelObject_Impl that is the
   *  implementation class for DesignSpecificationOutdoorAir.*/
    class MODEL_API DesignSpecificationOutdoorAir_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DesignSpecificationOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DesignSpecificationOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DesignSpecificationOutdoorAir_Impl(const DesignSpecificationOutdoorAir_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DesignSpecificationOutdoorAir_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      std::string outdoorAirMethod() const;

      bool isOutdoorAirMethodDefaulted() const;

      double outdoorAirFlowperPerson() const;

      bool isOutdoorAirFlowperPersonDefaulted() const;

      double outdoorAirFlowperFloorArea() const;

      bool isOutdoorAirFlowperFloorAreaDefaulted() const;

      double outdoorAirFlowRate() const;

      bool isOutdoorAirFlowRateDefaulted() const;

      double outdoorAirFlowAirChangesperHour() const;

      bool isOutdoorAirFlowAirChangesperHourDefaulted() const;

      boost::optional<Schedule> outdoorAirFlowRateFractionSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setOutdoorAirMethod(const std::string& outdoorAirMethod);

      void resetOutdoorAirMethod();

      bool setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson);

      void resetOutdoorAirFlowperPerson();

      bool setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea);

      void resetOutdoorAirFlowperFloorArea();

      bool setOutdoorAirFlowRate(double outdoorAirFlowRate);

      void resetOutdoorAirFlowRate();

      bool setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour);

      void resetOutdoorAirFlowAirChangesperHour();

      bool setOutdoorAirFlowRateFractionSchedule(Schedule& schedule);

      void resetOutdoorAirFlowRateFractionSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DesignSpecificationOutdoorAir");

      std::vector<std::string> outdoorAirMethodValues() const;
      boost::optional<ModelObject> outdoorAirFlowRateFractionScheduleAsModelObject() const;

      bool setOutdoorAirFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNSPECIFICATIONOUTDOORAIR_IMPL_HPP
