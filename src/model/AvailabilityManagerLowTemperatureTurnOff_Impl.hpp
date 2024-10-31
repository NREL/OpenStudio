/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNOFF_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNOFF_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class Schedule;

  namespace detail {

    /** AvailabilityManagerLowTemperatureTurnOff_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerLowTemperatureTurnOff.*/
    class MODEL_API AvailabilityManagerLowTemperatureTurnOff_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerLowTemperatureTurnOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerLowTemperatureTurnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerLowTemperatureTurnOff_Impl(const AvailabilityManagerLowTemperatureTurnOff_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerLowTemperatureTurnOff_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> sensorNode() const;

      double temperature() const;

      Schedule applicabilitySchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSensorNode(const Node& node);

      void resetSensorNode();

      bool setTemperature(double temperature);

      bool setApplicabilitySchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerLowTemperatureTurnOff");

      boost::optional<Schedule> optionalApplicabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNOFF_IMPL_HPP
