/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_IMPL_HPP
#define MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVACOutdoorAirSystem;
  class Schedule;
  class AirLoopHVAC;

  namespace detail {

    /** AirLoopHVACDedicatedOutdoorAirSystem_Impl is a ModelObject_Impl that is the implementation class for AirLoopHVACDedicatedOutdoorAirSystem.*/
    class MODEL_API AirLoopHVACDedicatedOutdoorAirSystem_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirLoopHVACDedicatedOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACDedicatedOutdoorAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACDedicatedOutdoorAirSystem_Impl(const AirLoopHVACDedicatedOutdoorAirSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACDedicatedOutdoorAirSystem_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      AirLoopHVACOutdoorAirSystem airLoopHVACOutdoorAirSystem() const;

      Schedule availabilitySchedule() const;

      double preheatDesignTemperature() const;

      double preheatDesignHumidityRatio() const;

      double precoolDesignTemperature() const;

      double precoolDesignHumidityRatio() const;

      unsigned int numberofAirLoops() const;

      std::vector<AirLoopHVAC> airLoops() const;

      boost::optional<unsigned> airLoopIndex(const AirLoopHVAC& airLoopHVAC) const;

      //@}
      /** @name Setters */
      //@{

      bool setAirLoopHVACOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& airLoopHVACOutdoorAirSystem);

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setPreheatDesignTemperature(double preheatDesignTemperature);

      bool setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio);

      bool setPrecoolDesignTemperature(double precoolDesignTemperature);

      bool setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio);

      bool addAirLoop(const AirLoopHVAC& airLoopHVAC);

      bool removeAirLoop(const AirLoopHVAC& airLoopHVAC);

      bool removeAirLoop(unsigned groupIndex);

      void removeAllAirLoops();

      virtual std::vector<openstudio::IdfObject> remove() override;

      bool addAirLoops(const std::vector<AirLoopHVAC>& airLoopHVACs);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACDedicatedOutdoorAirSystem");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_IMPL_HPP
