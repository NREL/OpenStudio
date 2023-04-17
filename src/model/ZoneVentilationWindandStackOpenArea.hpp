/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~ZoneVentilationWindandStackOpenArea() = default;
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
