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

#ifndef MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP
#define MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Surface;

  namespace detail {

    class ZoneHVACHighTemperatureRadiant_Impl;

  }  // namespace detail

  /** ZoneHVACHighTemperatureRadiant is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:HighTemperatureRadiant'. */
  class MODEL_API ZoneHVACHighTemperatureRadiant : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACHighTemperatureRadiant(const Model& model);

    virtual ~ZoneHVACHighTemperatureRadiant() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACHighTemperatureRadiant(const ZoneHVACHighTemperatureRadiant& other) = default;
    ZoneHVACHighTemperatureRadiant(ZoneHVACHighTemperatureRadiant&& other) = default;
    ZoneHVACHighTemperatureRadiant& operator=(const ZoneHVACHighTemperatureRadiant&) = default;
    ZoneHVACHighTemperatureRadiant& operator=(ZoneHVACHighTemperatureRadiant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    static std::vector<std::string> temperatureControlTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    /** In EnergyPlus 8.2.0 and above this property maps to the EnergyPlus field "Heating Design Capacity" **/
    boost::optional<double> maximumPowerInput() const;

    bool isMaximumPowerInputAutosized() const;

    std::string fuelType() const;

    double combustionEfficiency() const;

    double fractionofInputConvertedtoRadiantEnergy() const;

    double fractionofInputConvertedtoLatentEnergy() const;

    double fractionofInputthatIsLost() const;

    std::string temperatureControlType() const;

    double heatingThrottlingRange() const;

    boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumPowerInput(double maximumPowerInput);

    void autosizeMaximumPowerInput();

    bool setFuelType(const std::string& fuelType);

    bool setCombustionEfficiency(double combustionEfficiency);

    bool setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy);

    bool setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy);

    bool setFractionofInputthatIsLost(double fractionofInputthatIsLost);

    bool setTemperatureControlType(const std::string& temperatureControlType);

    bool setHeatingThrottlingRange(double heatingThrottlingRange);

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    boost::optional<ThermalZone> thermalZone() const override;

    bool addToThermalZone(ThermalZone& thermalZone);

    void removeFromThermalZone();
    //@}

    boost::optional<double> autosizedMaximumPowerInput() const;

   protected:
    /// @cond
    using ImplType = detail::ZoneHVACHighTemperatureRadiant_Impl;

    explicit ZoneHVACHighTemperatureRadiant(std::shared_ptr<detail::ZoneHVACHighTemperatureRadiant_Impl> impl);

    friend class detail::ZoneHVACHighTemperatureRadiant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACHighTemperatureRadiant");
  };

  /** \relates ZoneHVACHighTemperatureRadiant*/
  using OptionalZoneHVACHighTemperatureRadiant = boost::optional<ZoneHVACHighTemperatureRadiant>;

  /** \relates ZoneHVACHighTemperatureRadiant*/
  using ZoneHVACHighTemperatureRadiantVector = std::vector<ZoneHVACHighTemperatureRadiant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP
