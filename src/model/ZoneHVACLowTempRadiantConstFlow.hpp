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

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACLowTempRadiantConstFlow_Impl;

  }  // namespace detail

  /** ZoneHVACLowTempRadiantConstFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:ConstantFlow'. */
  class MODEL_API ZoneHVACLowTempRadiantConstFlow : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACLowTempRadiantConstFlow(const Model& model, Schedule& availabilitySchedule, HVACComponent& heatingCoil, HVACComponent& coolingCoil,
                                    double hydronicTubingLength);

    // Ctor with hydronicTubingLength autosized
    ZoneHVACLowTempRadiantConstFlow(const Model& model, Schedule& availabilitySchedule, HVACComponent& heatingCoil, HVACComponent& coolingCoil);

    virtual ~ZoneHVACLowTempRadiantConstFlow() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACLowTempRadiantConstFlow(const ZoneHVACLowTempRadiantConstFlow& other) = default;
    ZoneHVACLowTempRadiantConstFlow(ZoneHVACLowTempRadiantConstFlow&& other) = default;
    ZoneHVACLowTempRadiantConstFlow& operator=(const ZoneHVACLowTempRadiantConstFlow&) = default;
    ZoneHVACLowTempRadiantConstFlow& operator=(ZoneHVACLowTempRadiantConstFlow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> radiantSurfaceTypeValues();

    static std::vector<std::string> temperatureControlTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<std::string> radiantSurfaceType() const;

    std::vector<Surface> surfaces() const;

    std::string fluidtoRadiantSurfaceHeatTransferModel() const;

    bool isFluidtoRadiantSurfaceHeatTransferModelDefaulted() const;

    double hydronicTubingInsideDiameter() const;

    bool isHydronicTubingInsideDiameterDefaulted() const;

    double hydronicTubingOutsideDiameter() const;

    bool isHydronicTubingOutsideDiameterDefaulted() const;

    boost::optional<double> hydronicTubingLength() const;

    bool isHydronicTubingLengthAutosized() const;

    double hydronicTubingConductivity() const;

    bool isHydronicTubingConductivityDefaulted() const;

    std::string temperatureControlType() const;

    bool isTemperatureControlTypeDefaulted() const;

    double runningMeanOutdoorDryBulbTemperatureWeightingFactor() const;

    bool isRunningMeanOutdoorDryBulbTemperatureWeightingFactorDefaulted() const;

    HVACComponent heatingCoil() const;

    HVACComponent coolingCoil() const;

    boost::optional<double> ratedFlowRate() const;

    bool isRatedFlowRateAutosized() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    double ratedPumpHead() const;

    bool isRatedPumpHeadDefaulted() const;

    boost::optional<double> ratedPowerConsumption() const;

    double motorEfficiency() const;

    bool isMotorEfficiencyDefaulted() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

    std::string numberofCircuits() const;

    double circuitLength() const;

    boost::optional<Schedule> changeoverDelayTimePeriodSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRadiantSurfaceType(const std::string& radiantSurfaceType);

    void resetRadiantSurfaceType();

    bool setFluidtoRadiantSurfaceHeatTransferModel(const std::string& fluidtoRadiantSurfaceHeatTransferModel);

    void resetFluidtoRadiantSurfaceHeatTransferModel();

    bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

    void resetHydronicTubingInsideDiameter();

    bool setHydronicTubingOutsideDiameter(double hydronicTubingOutsideDiameter);

    void resetHydronicTubingOutsideDiameter();

    bool setHydronicTubingLength(double hydronicTubingLength);

    // Will forward to autosizeHydronicTubingLength()
    OS_DEPRECATED void resetHydronicTubingLength();  // Shouldn't have existed to begin with.

    void autosizeHydronicTubingLength();

    bool setHydronicTubingConductivity(double hydronicTubingConductivity);

    void resetHydronicTubingConductivity();

    bool setTemperatureControlType(const std::string& temperatureControlType);

    void resetTemperatureControlType();

    bool setRunningMeanOutdoorDryBulbTemperatureWeightingFactor(double runningMeanOutdoorDryBulbTemperatureWeightingFactor);

    void resetRunningMeanOutdoorDryBulbTemperatureWeightingFactor();

    bool setHeatingCoil(HVACComponent& heatingCoil);

    bool setCoolingCoil(HVACComponent& coolingCoil);

    bool setRatedFlowRate(double ratedFlowRate);

    // Will forward to autosizeRatedFlowRate()
    OS_DEPRECATED void resetRatedFlowRate();

    void autosizeRatedFlowRate();

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setRatedPumpHead(double ratedPumpHead);

    void resetRatedPumpHead();

    bool setRatedPowerConsumption(double ratedPowerConsumption);

    void resetRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    void resetFractionofMotorInefficienciestoFluidStream();

    bool setNumberofCircuits(const std::string& numberofCircuits);

    bool setCircuitLength(double circuitLength);

    bool setChangeoverDelayTimePeriodSchedule(Schedule& schedule);

    void resetChangeoverDelayTimePeriodSchedule();

    boost::optional<ThermalZone> thermalZone() const override;

    bool addToThermalZone(ThermalZone& thermalZone);

    void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHydronicTubingLength() const;

    boost::optional<double> autosizedRatedFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACLowTempRadiantConstFlow_Impl;

    explicit ZoneHVACLowTempRadiantConstFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantConstFlow_Impl> impl);

    friend class detail::ZoneHVACLowTempRadiantConstFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantConstFlow");
  };

  /** \relates ZoneHVACLowTempRadiantConstFlow*/
  using OptionalZoneHVACLowTempRadiantConstFlow = boost::optional<ZoneHVACLowTempRadiantConstFlow>;

  /** \relates ZoneHVACLowTempRadiantConstFlow*/
  using ZoneHVACLowTempRadiantConstFlowVector = std::vector<ZoneHVACLowTempRadiantConstFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_HPP
