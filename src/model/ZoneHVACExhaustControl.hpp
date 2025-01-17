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

#ifndef MODEL_ZONEHVACEXHAUSTCONTROL_HPP
#define MODEL_ZONEHVACEXHAUSTCONTROL_HPP

#include <model/ModelAPI.hpp>
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class ZoneHVACExhaustControl_Impl;

  }  // namespace detail

  /** ZoneHVACExhaustControl is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:ExhaustControl'. */
  class MODEL_API ZoneHVACExhaustControl : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACExhaustControl(const Model& model);

    virtual ~ZoneHVACExhaustControl() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACExhaustControl(const ZoneHVACExhaustControl& other) = default;
    ZoneHVACExhaustControl(ZoneHVACExhaustControl&& other) = default;
    ZoneHVACExhaustControl& operator=(const ZoneHVACExhaustControl&) = default;
    ZoneHVACExhaustControl& operator=(ZoneHVACExhaustControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> flowControlTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<ThermalZone> thermalZone() const override;

    boost::optional<double> designExhaustFlowRate() const;

    bool isDesignExhaustFlowRateAutosized() const;

    boost::optional<double> autosizedDesignExhaustFlowRate();

    std::string flowControlType() const;

    boost::optional<Schedule> exhaustFlowFractionSchedule() const;

    boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

    boost::optional<Schedule> minimumExhaustFlowFractionSchedule() const;

    boost::optional<Schedule> balancedExhaustFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool addToThermalZone(ThermalZone& thermalZone);

    void removeFromThermalZone();

    bool setDesignExhaustFlowRate(double designExhaustFlowRate);

    void autosizeDesignExhaustFlowRate();

    bool setFlowControlType(const std::string& flowControlType);

    bool setExhaustFlowFractionSchedule(Schedule& schedule);

    void resetExhaustFlowFractionSchedule();

    bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

    void resetMinimumZoneTemperatureLimitSchedule();

    bool setMinimumExhaustFlowFractionSchedule(Schedule& schedule);

    void resetMinimumExhaustFlowFractionSchedule();

    bool setBalancedExhaustFractionSchedule(Schedule& schedule);

    void resetBalancedExhaustFractionSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACExhaustControl_Impl;

    explicit ZoneHVACExhaustControl(std::shared_ptr<detail::ZoneHVACExhaustControl_Impl> impl);

    friend class detail::ZoneHVACExhaustControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACExhaustControl");
  };

  /** \relates ZoneHVACExhaustControl*/
  using OptionalZoneHVACExhaustControl = boost::optional<ZoneHVACExhaustControl>;

  /** \relates ZoneHVACExhaustControl*/
  using ZoneHVACExhaustControlVector = std::vector<ZoneHVACExhaustControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACEXHAUSTCONTROL_HPP
