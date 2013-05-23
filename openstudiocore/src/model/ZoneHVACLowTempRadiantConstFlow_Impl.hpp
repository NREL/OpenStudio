/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>

//#include <utilities/units/Quantity.hpp>
//#include <utilities/units/OSOptionalQuantity.hpp>

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
    virtual ModelObject clone(Model model) const;
   
    virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();


    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<std::string> radiantSurfaceGroupName() const;

    double hydronicTubingInsideDiameter() const;

    bool isHydronicTubingInsideDiameterDefaulted() const;

    boost::optional<double> hydronicTubingLength() const;

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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRadiantSurfaceGroupName(boost::optional<std::string> radiantSurfaceGroupName);

    void resetRadiantSurfaceGroupName();

    bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

    void resetHydronicTubingInsideDiameter();

    bool setHydronicTubingLength(boost::optional<double> hydronicTubingLength);

    void resetHydronicTubingLength();

    bool setTemperatureControlType(std::string temperatureControlType);

    void resetTemperatureControlType();

    bool setHeatingCoil(HVACComponent& heatingCoil);

    bool setCoolingCoil(HVACComponent& coolingCoil);

    void setRatedFlowRate(boost::optional<double> ratedFlowRate);

    void resetRatedFlowRate();

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    void setRatedPumpHead(double ratedPumpHead);

    void resetRatedPumpHead();

    void setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

    void resetRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    void resetFractionofMotorInefficienciestoFluidStream();

    boost::optional<ThermalZone> thermalZone();

    bool addToThermalZone(ThermalZone & thermalZone);

    void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

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

    std::vector<std::string> radiantSurfaceGroupNameValues() const;
    //openstudio::Quantity hydronicTubingInsideDiameter_SI() const;
    //openstudio::Quantity hydronicTubingInsideDiameter_IP() const;
    //openstudio::OSOptionalQuantity hydronicTubingLength_SI() const;
    //openstudio::OSOptionalQuantity hydronicTubingLength_IP() const;
    std::vector<std::string> temperatureControlTypeValues() const;
    //openstudio::OSOptionalQuantity ratedFlowRate_SI() const;
    //openstudio::OSOptionalQuantity ratedFlowRate_IP() const;
    //openstudio::Quantity ratedPumpHead_SI() const;
    //openstudio::Quantity ratedPumpHead_IP() const;
    //openstudio::OSOptionalQuantity ratedPowerConsumption_SI() const;
    //openstudio::OSOptionalQuantity ratedPowerConsumption_IP() const;
    //openstudio::Quantity motorEfficiency_SI() const;
    //openstudio::Quantity motorEfficiency_IP() const;
    //openstudio::Quantity fractionofMotorInefficienciestoFluidStream_SI() const;
    //openstudio::Quantity fractionofMotorInefficienciestoFluidStream_IP() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;
    boost::optional<ModelObject> coolingCoilAsModelObject() const;
    boost::optional<ModelObject> pumpFlowRateScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

