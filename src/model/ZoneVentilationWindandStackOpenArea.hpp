/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEVENTILATIONWINDANDSTACKOPENAREA_HPP
#define MODEL_ZONEVENTILATIONWINDANDSTACKOPENAREA_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class ZoneVentilationWindandStackOpenArea_Impl;

  }  // namespace detail

  /** ZoneVentilationWindandStackOpenArea is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneVentilation:WindandStackOpenArea'. */
  class MODEL_API ZoneVentilationWindandStackOpenArea : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneVentilationWindandStackOpenArea(const Model& model);

    virtual ~ZoneVentilationWindandStackOpenArea() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneVentilationWindandStackOpenArea(const ZoneVentilationWindandStackOpenArea& other) = default;
    ZoneVentilationWindandStackOpenArea(ZoneVentilationWindandStackOpenArea&& other) = default;
    ZoneVentilationWindandStackOpenArea& operator=(const ZoneVentilationWindandStackOpenArea&) = default;
    ZoneVentilationWindandStackOpenArea& operator=(ZoneVentilationWindandStackOpenArea&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double openingArea() const;

    Schedule openingAreaFractionSchedule() const;

    boost::optional<double> openingEffectiveness() const;
    bool isOpeningEffectivenessAutocalculated() const;

    double effectiveAngle() const;

    double heightDifference() const;

    boost::optional<double> dischargeCoefficientforOpening() const;
    bool isDischargeCoefficientforOpeningAutocalculated() const;

    double minimumIndoorTemperature() const;

    boost::optional<Schedule> minimumIndoorTemperatureSchedule() const;

    double maximumIndoorTemperature() const;

    boost::optional<Schedule> maximumIndoorTemperatureSchedule() const;

    double deltaTemperature() const;

    boost::optional<Schedule> deltaTemperatureSchedule() const;

    double minimumOutdoorTemperature() const;

    boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

    double maximumOutdoorTemperature() const;

    boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

    double maximumWindSpeed() const;

    //@}
    /** @name Setters */
    //@{

    bool setOpeningArea(double openingArea);

    bool setOpeningAreaFractionSchedule(Schedule& schedule);

    bool setOpeningEffectiveness(double openingEffectiveness);
    void autocalculateOpeningEffectiveness();

    bool setEffectiveAngle(double effectiveAngle);

    bool setHeightDifference(double heightDifference);

    bool setDischargeCoefficientforOpening(double dischargeCoefficientforOpening);
    void autocalculateDischargeCoefficientforOpening();

    bool setMinimumIndoorTemperature(double minimumIndoorTemperature);

    bool setMinimumIndoorTemperatureSchedule(Schedule& schedule);
    void resetMinimumIndoorTemperatureSchedule();

    bool setMaximumIndoorTemperature(double maximumIndoorTemperature);

    bool setMaximumIndoorTemperatureSchedule(Schedule& schedule);
    void resetMaximumIndoorTemperatureSchedule();

    bool setDeltaTemperature(double deltaTemperature);

    bool setDeltaTemperatureSchedule(Schedule& schedule);
    void resetDeltaTemperatureSchedule();

    bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

    bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);
    void resetMinimumOutdoorTemperatureSchedule();

    bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

    bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);
    void resetMaximumOutdoorTemperatureSchedule();

    bool setMaximumWindSpeed(double maximumWindSpeed);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneVentilationWindandStackOpenArea_Impl;

    explicit ZoneVentilationWindandStackOpenArea(std::shared_ptr<detail::ZoneVentilationWindandStackOpenArea_Impl> impl);

    friend class detail::ZoneVentilationWindandStackOpenArea_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneVentilationWindandStackOpenArea");
  };

  /** \relates ZoneVentilationWindandStackOpenArea*/
  using OptionalZoneVentilationWindandStackOpenArea = boost::optional<ZoneVentilationWindandStackOpenArea>;

  /** \relates ZoneVentilationWindandStackOpenArea*/
  using ZoneVentilationWindandStackOpenAreaVector = std::vector<ZoneVentilationWindandStackOpenArea>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEVENTILATIONWINDANDSTACKOPENAREA_HPP
