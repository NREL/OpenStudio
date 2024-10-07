/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_HPP
#define MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVACOutdoorAirSystem;
  class Schedule;
  class AirLoopHVAC;

  namespace detail {

    class AirLoopHVACDedicatedOutdoorAirSystem_Impl;

  }  // namespace detail

  /** AirLoopHVACDedicatedOutdoorAirSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:DedicatedOutdoorAirSystem:MovableInsulation'. */
  class MODEL_API AirLoopHVACDedicatedOutdoorAirSystem : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    explicit AirLoopHVACDedicatedOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& oaSystem);

    virtual ~AirLoopHVACDedicatedOutdoorAirSystem() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACDedicatedOutdoorAirSystem(const AirLoopHVACDedicatedOutdoorAirSystem& other) = default;
    AirLoopHVACDedicatedOutdoorAirSystem(AirLoopHVACDedicatedOutdoorAirSystem&& other) = default;
    AirLoopHVACDedicatedOutdoorAirSystem& operator=(const AirLoopHVACDedicatedOutdoorAirSystem&) = default;
    AirLoopHVACDedicatedOutdoorAirSystem& operator=(AirLoopHVACDedicatedOutdoorAirSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

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

    bool addAirLoops(const std::vector<AirLoopHVAC>& airLoopHVACs);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl;

    explicit AirLoopHVACDedicatedOutdoorAirSystem(std::shared_ptr<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl> impl);

    friend class detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACDedicatedOutdoorAirSystem");
  };

  /** \relates AirLoopHVACDedicatedOutdoorAirSystem */
  using OptionalAirLoopHVACDedicatedOutdoorAirSystem = boost::optional<AirLoopHVACDedicatedOutdoorAirSystem>;

  /** \relates AirLoopHVACDedicatedOutdoorAirSystem */
  using AirLoopHVACDedicatedOutdoorAirSystemVector = std::vector<AirLoopHVACDedicatedOutdoorAirSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACDEDICATEDOUTDOORAIRSYSTEM_HPP
