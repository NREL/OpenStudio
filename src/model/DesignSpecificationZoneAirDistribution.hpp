/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP
#define MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class DesignSpecificationZoneAirDistribution_Impl;

  }  // namespace detail

  /** DesignSpecificationZoneAirDistribution is a ModelObject that wraps the OpenStudio IDD object 'OS:DesignSpecification:ZoneAirDistribution'. */
  class MODEL_API DesignSpecificationZoneAirDistribution : public ModelObject
  {
   public:
    explicit DesignSpecificationZoneAirDistribution(const Model& model);

    virtual ~DesignSpecificationZoneAirDistribution() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DesignSpecificationZoneAirDistribution(const DesignSpecificationZoneAirDistribution& other) = default;
    DesignSpecificationZoneAirDistribution(DesignSpecificationZoneAirDistribution&& other) = default;
    DesignSpecificationZoneAirDistribution& operator=(const DesignSpecificationZoneAirDistribution&) = default;
    DesignSpecificationZoneAirDistribution& operator=(DesignSpecificationZoneAirDistribution&&) = default;

    static IddObjectType iddObjectType();

    double zoneAirDistributionEffectivenessinCoolingMode() const;

    bool isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    bool setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode);

    void resetZoneAirDistributionEffectivenessinCoolingMode();

    double zoneAirDistributionEffectivenessinHeatingMode() const;

    bool isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    bool setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode);

    void resetZoneAirDistributionEffectivenessinHeatingMode();

    boost::optional<Schedule> zoneAirDistributionEffectivenessSchedule() const;

    bool setZoneAirDistributionEffectivenessSchedule(Schedule& schedule);

    void resetZoneAirDistributionEffectivenessSchedule();

    double zoneSecondaryRecirculationFraction() const;

    bool isZoneSecondaryRecirculationFractionDefaulted() const;

    bool setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction);

    void resetZoneSecondaryRecirculationFraction();

   protected:
    /// @cond
    using ImplType = detail::DesignSpecificationZoneAirDistribution_Impl;

    explicit DesignSpecificationZoneAirDistribution(std::shared_ptr<detail::DesignSpecificationZoneAirDistribution_Impl> impl);

    friend class detail::DesignSpecificationZoneAirDistribution_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DesignSpecificationZoneAirDistribution");
  };

  /** \relates DesignSpecificationZoneAirDistribution*/
  using OptionalDesignSpecificationZoneAirDistribution = boost::optional<DesignSpecificationZoneAirDistribution>;

  /** \relates DesignSpecificationZoneAirDistribution*/
  using DesignSpecificationZoneAirDistributionVector = std::vector<DesignSpecificationZoneAirDistribution>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP
