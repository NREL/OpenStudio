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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_IMPL_HPP

//#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

class Schedule; 
class StraightComponent;

namespace detail {

  /** AirTerminalSingleDuctConstantVolumeCooledBeam_Impl is a ZoneHAVC_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeCooledBeam.*/
  class MODEL_API AirTerminalSingleDuctConstantVolumeCooledBeam_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const IdfObject& idfObject,
                                                       Model_Impl* model,
                                                       bool keepHandle);

    AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle);

    AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const AirTerminalSingleDuctConstantVolumeCooledBeam_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle);

    virtual ~AirTerminalSingleDuctConstantVolumeCooledBeam_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();
    
    virtual bool isRemovable() const;
    
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const; 

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;
    
    virtual std::vector<ModelObject> children() const;
    
    virtual bool addToNode(Node & node); 
    
    virtual unsigned inletPort();
				
    virtual unsigned outletPort();
    
    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const; 
    
    HVACComponent coilCoolingCooledBeam() const; 

    std::string cooledBeamType() const; 

    boost::optional<double> supplyAirVolumetricFlowRate() const; 

    bool isSupplyAirVolumetricFlowRateDefaulted() const; 

    bool isSupplyAirVolumetricFlowRateAutosized() const; 

    boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate() const; 

    bool isMaximumTotalChilledWaterVolumetricFlowRateDefaulted() const; 

    bool isMaximumTotalChilledWaterVolumetricFlowRateAutosized() const; 

    boost::optional<int> numberofBeams() const; 

    bool isNumberofBeamsDefaulted() const; 

    bool isNumberofBeamsAutosized() const; 

    boost::optional<double> beamLength() const; 

    bool isBeamLengthDefaulted() const; 

    bool isBeamLengthAutosized() const; 

    double designInletWaterTemperature() const; 

    bool isDesignInletWaterTemperatureDefaulted() const; 

    double designOutletWaterTemperature() const; 

    bool isDesignOutletWaterTemperatureDefaulted() const; 

    boost::optional<double> coefficientofInductionKin() const; 

    bool isCoefficientofInductionKinDefaulted() const; 

    bool isCoefficientofInductionKinAutocalculated() const; 

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule); 
    
    bool setCoolingCoil(HVACComponent& coilCoolingCooledBeam); 

    bool setCooledBeamType(std::string cooledBeamType); 

    bool setSupplyAirVolumetricFlowRate(boost::optional<double> supplyAirVolumetricFlowRate); 

    void resetSupplyAirVolumetricFlowRate(); 

    void autosizeSupplyAirVolumetricFlowRate(); 

    bool setMaximumTotalChilledWaterVolumetricFlowRate(boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate); 

    void resetMaximumTotalChilledWaterVolumetricFlowRate(); 

    void autosizeMaximumTotalChilledWaterVolumetricFlowRate(); 

    bool setNumberofBeams(boost::optional<int> numberofBeams); 

    void resetNumberofBeams(); 

    void autosizeNumberofBeams(); 

    bool setBeamLength(boost::optional<double> beamLength); 

    void resetBeamLength(); 

    void autosizeBeamLength(); 

    bool setDesignInletWaterTemperature(double designInletWaterTemperature); 

    void resetDesignInletWaterTemperature(); 

    bool setDesignOutletWaterTemperature(double designOutletWaterTemperature); 

    void resetDesignOutletWaterTemperature(); 

    bool setCoefficientofInductionKin(boost::optional<double> coefficientofInductionKin); 

    void resetCoefficientofInductionKin(); 

    void autocalculateCoefficientofInductionKin(); 
    
    // boost::optional<ThermalZone> thermalZone();

    // bool addToThermalZone(ThermalZone & thermalZone);

    // void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeCooledBeam");
    
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;
    
    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> coolingCoilAsModelObject() const;
    
    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_IMPL_HPP

