/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** ZoneHVACLowTempRadiantConstFlow_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACLowTempRadiantConstFlow.*/
  class MODEL_API ZoneHVACLowTempRadiantConstFlow_Impl : public ZoneHVACComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    ZoneHVACLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    ZoneHVACLowTempRadiantConstFlow_Impl(const ZoneHVACLowTempRadiantConstFlow_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~ZoneHVACLowTempRadiantConstFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{
    virtual ModelObject clone(Model model) const override;

    virtual std::vector<IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;


    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<std::string> radiantSurfaceType() const;

    std::vector<Surface> surfaces() const;

    double hydronicTubingInsideDiameter() const;

    bool isHydronicTubingInsideDiameterDefaulted() const;

    double hydronicTubingLength() const;

    std::string temperatureControlType() const;

    bool isTemperatureControlTypeDefaulted() const;

    HVACComponent heatingCoil() const;

    HVACComponent coolingCoil() const;

    boost::optional<double> ratedFlowRate() const;

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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

    void resetRadiantSurfaceType();

    bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

    void resetHydronicTubingInsideDiameter();

    bool setHydronicTubingLength(boost::optional<double> hydronicTubingLength);

    void resetHydronicTubingLength();

    bool setTemperatureControlType(std::string temperatureControlType);

    void resetTemperatureControlType();

    bool setHeatingCoil(HVACComponent& heatingCoil);

    bool setCoolingCoil(HVACComponent& coolingCoil);

    bool setRatedFlowRate(boost::optional<double> ratedFlowRate);

    void resetRatedFlowRate();

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setRatedPumpHead(double ratedPumpHead);

    void resetRatedPumpHead();

    bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

    void resetRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    void resetFractionofMotorInefficienciestoFluidStream();

    bool setNumberofCircuits(std::string numberofCircuits);

    bool setCircuitLength(double circuitLength);

    boost::optional<ThermalZone> thermalZone() const;

    bool addToThermalZone(ThermalZone & thermalZone) override;

    void removeFromThermalZone() override;

    //@}
    /** @name Other */
    //@{

    virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

    virtual std::vector<std::string> emsInternalVariableNames() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantConstFlow");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;

    std::vector<std::string> radiantSurfaceTypeValues() const;
    std::vector<std::string> temperatureControlTypeValues() const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
