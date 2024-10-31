/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SWIMMINGPOOLINDOOR_HPP
#define MODEL_SWIMMINGPOOLINDOOR_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Surface;
  class Schedule;
  class Node;

  namespace detail {

    class SwimmingPoolIndoor_Impl;

  }  // namespace detail

  /** SwimmingPoolIndoor is a StraightComponent that wraps the OpenStudio IDD object 'OS:SwimmingPool:Indoor'. */
  class MODEL_API SwimmingPoolIndoor : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SwimmingPoolIndoor(const Model& model, const Surface& floorSurface);

    virtual ~SwimmingPoolIndoor() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SwimmingPoolIndoor(const SwimmingPoolIndoor& other) = default;
    SwimmingPoolIndoor(SwimmingPoolIndoor&& other) = default;
    SwimmingPoolIndoor& operator=(const SwimmingPoolIndoor&) = default;
    SwimmingPoolIndoor& operator=(SwimmingPoolIndoor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Surface surface() const;

    double averageDepth() const;

    Schedule activityFactorSchedule() const;

    Schedule makeupWaterSupplySchedule() const;

    Schedule coverSchedule() const;

    double coverEvaporationFactor() const;

    double coverConvectionFactor() const;

    double coverShortWavelengthRadiationFactor() const;

    double coverLongWavelengthRadiationFactor() const;

    double poolHeatingSystemMaximumWaterFlowRate() const;

    double poolMiscellaneousEquipmentPower() const;

    Schedule setpointTemperatureSchedule() const;

    double maximumNumberofPeople() const;

    Schedule peopleSchedule() const;

    Schedule peopleHeatGainSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // Set the Pool's surface object. Surface MUST be of type 'Floor'
    bool setSurface(const Surface& floorSurface);

    bool setAverageDepth(double averageDepth);

    bool setActivityFactorSchedule(Schedule& schedule);

    bool setMakeupWaterSupplySchedule(Schedule& schedule);

    bool setCoverSchedule(Schedule& schedule);

    bool setCoverEvaporationFactor(double coverEvaporationFactor);

    bool setCoverConvectionFactor(double coverConvectionFactor);

    bool setCoverShortWavelengthRadiationFactor(double coverShortWavelengthRadiationFactor);

    bool setCoverLongWavelengthRadiationFactor(double coverLongWavelengthRadiationFactor);

    bool setPoolHeatingSystemMaximumWaterFlowRate(double poolHeatingSystemMaximumWaterFlowRate);

    bool setPoolMiscellaneousEquipmentPower(double poolMiscellaneousEquipmentPower);

    bool setSetpointTemperatureSchedule(Schedule& schedule);

    bool setMaximumNumberofPeople(double maximumNumberofPeople);

    bool setPeopleSchedule(Schedule& schedule);

    bool setPeopleHeatGainSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    // Convenience function to return the inletNode of the Pool object
    boost::optional<Node> poolWaterInletNode() const;

    // Convenience function to return the Outlet Node of the Pool object
    boost::optional<Node> poolWaterOutletNode() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::SwimmingPoolIndoor_Impl;

    explicit SwimmingPoolIndoor(std::shared_ptr<detail::SwimmingPoolIndoor_Impl> impl);

    friend class detail::SwimmingPoolIndoor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SwimmingPoolIndoor");
  };

  /** \relates SwimmingPoolIndoor*/
  using OptionalSwimmingPoolIndoor = boost::optional<SwimmingPoolIndoor>;

  /** \relates SwimmingPoolIndoor*/
  using SwimmingPoolIndoorVector = std::vector<SwimmingPoolIndoor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SWIMMINGPOOLINDOOR_HPP
