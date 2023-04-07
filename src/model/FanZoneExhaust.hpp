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

#ifndef MODEL_FANZONEEXHAUST_HPP
#define MODEL_FANZONEEXHAUST_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class AirflowNetworkZoneExhaustFan;
  class AirflowNetworkCrack;

  namespace detail {

    class FanZoneExhaust_Impl;

  }  // namespace detail

  /** FanZoneExhaust is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:Fan:ZoneExhaust'. */
  class MODEL_API FanZoneExhaust : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FanZoneExhaust(const Model& model);

    virtual ~FanZoneExhaust() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanZoneExhaust(const FanZoneExhaust& other) = default;
    FanZoneExhaust(FanZoneExhaust&& other) = default;
    FanZoneExhaust& operator=(const FanZoneExhaust&) = default;
    FanZoneExhaust& operator=(FanZoneExhaust&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> systemAvailabilityManagerCouplingModeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double fanTotalEfficiency() const;

    /** Deprecated, forwards to fanTotalEfficiency */
    double fanEfficiency() const;

    double pressureRise() const;

    boost::optional<double> maximumFlowRate() const;

    std::string endUseSubcategory() const;

    boost::optional<Schedule> flowFractionSchedule() const;

    std::string systemAvailabilityManagerCouplingMode() const;

    boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

    boost::optional<Schedule> balancedExhaustFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setFanTotalEfficiency(double fanTotalEfficiency);

    /** Deprecated, forwards to setFanTotalEfficiency */
    bool setFanEfficiency(double fanTotalEfficiency);

    bool setPressureRise(double pressureRise);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    bool setFlowFractionSchedule(Schedule& schedule);

    void resetFlowFractionSchedule();

    bool setSystemAvailabilityManagerCouplingMode(const std::string& systemAvailabilityManagerCouplingMode);

    bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

    void resetMinimumZoneTemperatureLimitSchedule();

    bool setBalancedExhaustFractionSchedule(Schedule& schedule);

    void resetBalancedExhaustFractionSchedule();

    //@}
    /** @name Other */
    //@{

    AirflowNetworkZoneExhaustFan getAirflowNetworkZoneExhaustFan(const AirflowNetworkCrack& crack);
    boost::optional<AirflowNetworkZoneExhaustFan> airflowNetworkZoneExhaustFan() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FanZoneExhaust_Impl;

    explicit FanZoneExhaust(std::shared_ptr<detail::FanZoneExhaust_Impl> impl);

    friend class detail::FanZoneExhaust_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
  };

  /** \relates FanZoneExhaust*/
  using OptionalFanZoneExhaust = boost::optional<FanZoneExhaust>;

  /** \relates FanZoneExhaust*/
  using FanZoneExhaustVector = std::vector<FanZoneExhaust>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANZONEEXHAUST_HPP
