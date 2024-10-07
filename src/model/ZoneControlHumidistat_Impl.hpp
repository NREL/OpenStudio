/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECONTROLHUMIDISTAT_IMPL_HPP
#define MODEL_ZONECONTROLHUMIDISTAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** ZoneControlHumidistat_Impl is a ModelObject_Impl that is the implementation class for ZoneControlHumidistat.*/
    class MODEL_API ZoneControlHumidistat_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneControlHumidistat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneControlHumidistat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneControlHumidistat_Impl(const ZoneControlHumidistat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneControlHumidistat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> humidifyingRelativeHumiditySetpointSchedule() const;

      boost::optional<Schedule> dehumidifyingRelativeHumiditySetpointSchedule() const;

      boost::optional<ThermalZone> controlledZone() const;

      //@}
      /** @name Setters */
      //@{

      bool setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

      bool setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

      void resetHumidifyingRelativeHumiditySetpointSchedule();

      void resetDehumidifyingRelativeHumiditySetpointSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneControlHumidistat");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECONTROLHUMIDISTAT_IMPL_HPP
