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

#ifndef MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP
#define MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP

#include "Thermostat.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ThermostatSetpointDualSetpoint_Impl;
  }

  class Schedule;

  /** ThermostatSetpointDualSetpoint is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:ThermostatSetpoint:DualSetpoint'. */
  class MODEL_API ThermostatSetpointDualSetpoint : public Thermostat
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ThermostatSetpointDualSetpoint(const Model& model);

    virtual ~ThermostatSetpointDualSetpoint() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint& other) = default;
    ThermostatSetpointDualSetpoint(ThermostatSetpointDualSetpoint&& other) = default;
    ThermostatSetpointDualSetpoint& operator=(const ThermostatSetpointDualSetpoint&) = default;
    ThermostatSetpointDualSetpoint& operator=(ThermostatSetpointDualSetpoint&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

    boost::optional<Schedule> coolingSetpointTemperatureSchedule() const;

    /** This ends up in the ZoneControl:Thermostat object during ForwardTranslation */
    double temperatureDifferenceBetweenCutoutAndSetpoint() const;
    bool isTemperatureDifferenceBetweenCutoutAndSetpointDefaulted() const;

    /** \deprecated */
    boost::optional<Schedule> getHeatingSchedule() const;

    /** \deprecated */
    boost::optional<Schedule> getCoolingSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setCoolingSetpointTemperatureSchedule(Schedule& schedule);

    void resetCoolingSetpointTemperatureSchedule();

    bool setTemperatureDifferenceBetweenCutoutAndSetpoint(double deltaT);

    /** \deprecated */
    bool setHeatingSchedule(Schedule& s);

    /** \deprecated */
    void resetHeatingSchedule();

    /** \deprecated */
    bool setCoolingSchedule(Schedule& s);

    /** \deprecated */
    void resetCoolingSchedule();

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    using ImplType = detail::ThermostatSetpointDualSetpoint_Impl;

    explicit ThermostatSetpointDualSetpoint(std::shared_ptr<detail::ThermostatSetpointDualSetpoint_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ThermostatSetpointDualSetpoint");
  };

  using OptionalThermostatSetpointDualSetpoint = boost::optional<ThermostatSetpointDualSetpoint>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP
