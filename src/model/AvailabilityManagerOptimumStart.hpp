/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP
#define MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class AvailabilityManagerOptimumStart_Impl;

  }  // namespace detail

  /** AvailabilityManagerOptimumStart is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:OptimumStart'. */
  class MODEL_API AvailabilityManagerOptimumStart : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerOptimumStart(const Model& model);

    virtual ~AvailabilityManagerOptimumStart() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerOptimumStart(const AvailabilityManagerOptimumStart& other) = default;
    AvailabilityManagerOptimumStart(AvailabilityManagerOptimumStart&& other) = default;
    AvailabilityManagerOptimumStart& operator=(const AvailabilityManagerOptimumStart&) = default;
    AvailabilityManagerOptimumStart& operator=(AvailabilityManagerOptimumStart&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlTypeValues();

    static std::vector<std::string> controlAlgorithmValues();

    /** @name Getters */
    //@{

    Schedule applicabilitySchedule() const;

    std::string controlType() const;

    boost::optional<ThermalZone> controlZone() const;

    double maximumValueforOptimumStartTime() const;

    std::string controlAlgorithm() const;

    double constantTemperatureGradientduringCooling() const;

    double constantTemperatureGradientduringHeating() const;

    double initialTemperatureGradientduringCooling() const;

    double initialTemperatureGradientduringHeating() const;

    double constantStartTime() const;

    int numberofPreviousDays() const;

    //@}
    /** @name Setters */
    //@{

    bool setApplicabilitySchedule(Schedule& schedule);

    bool setControlType(const std::string& controlType);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    bool setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime);

    bool setControlAlgorithm(const std::string& controlAlgorithm);

    bool setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling);

    bool setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating);

    bool setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling);

    bool setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating);

    bool setConstantStartTime(double constantStartTime);

    bool setNumberofPreviousDays(int numberofPreviousDays);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerOptimumStart_Impl;

    explicit AvailabilityManagerOptimumStart(std::shared_ptr<detail::AvailabilityManagerOptimumStart_Impl> impl);

    friend class detail::AvailabilityManagerOptimumStart_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerOptimumStart");
  };

  /** \relates AvailabilityManagerOptimumStart*/
  using OptionalAvailabilityManagerOptimumStart = boost::optional<AvailabilityManagerOptimumStart>;

  /** \relates AvailabilityManagerOptimumStart*/
  using AvailabilityManagerOptimumStartVector = std::vector<AvailabilityManagerOptimumStart>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP
