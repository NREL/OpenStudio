/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP
#define MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** ZoneHVACHighTemperatureRadiant_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACHighTemperatureRadiant.*/
  class MODEL_API ZoneHVACHighTemperatureRadiant_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACHighTemperatureRadiant_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    ZoneHVACHighTemperatureRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    ZoneHVACHighTemperatureRadiant_Impl(const ZoneHVACHighTemperatureRadiant_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~ZoneHVACHighTemperatureRadiant_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    std::vector<Surface> surfaces() const;

    boost::optional<Schedule> availabilitySchedule() const;

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

  boost::optional<double> autosizedMaximumPowerInput() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumPowerInput(boost::optional<double> maximumPowerInput);

    void autosizeMaximumPowerInput();

    bool setFuelType(std::string fuelType);

    bool setCombustionEfficiency(double combustionEfficiency);

    bool setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy);

    bool setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy);

    bool setFractionofInputthatIsLost(double fractionofInputthatIsLost);

    bool setTemperatureControlType(std::string temperatureControlType);

    bool setHeatingThrottlingRange(double heatingThrottlingRange);

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    boost::optional<ThermalZone> thermalZone() const;

    bool addToThermalZone(ThermalZone & thermalZone) override;

    void removeFromThermalZone() override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;
    //@}

   protected:

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACHighTemperatureRadiant");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP

