/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_FANVARIABLEVOLUME_IMPL_HPP
#define MODEL_FANVARIABLEVOLUME_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  /** FanVariableVolume_Impl is a StraightComponent_Impl that is the implementation
 *  class for FanVariableVolume.*/
  class MODEL_API FanVariableVolume_Impl : public StraightComponent_Impl {
    
    
    
    
    

    
    
    

    
    
    
    

    
    
    

    
    
    
    

    
    
    

    
    
    
    

    
    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    

    
  public:
    /** @name Constructors and Destructors */
    //@{

    FanVariableVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    FanVariableVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    FanVariableVolume_Impl(const FanVariableVolume_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~FanVariableVolume_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fanEfficiency() const;

    Quantity getFanEfficiency(bool returnIP=false) const;

    bool isFanEfficiencyDefaulted() const;

    double pressureRise() const;

    Quantity getPressureRise(bool returnIP=false) const;

    boost::optional<double> maximumFlowRate() const;

    OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

    bool isMaximumFlowRateAutosized() const;

    std::string fanPowerMinimumFlowRateInputMethod() const;

    bool isFanPowerMinimumFlowRateInputMethodDefaulted() const;

    double fanPowerMinimumFlowFraction() const;

    Quantity getFanPowerMinimumFlowFraction(bool returnIP=false) const;

    bool isFanPowerMinimumFlowFractionDefaulted() const;

    boost::optional<double> fanPowerMinimumAirFlowRate() const;

    OSOptionalQuantity getFanPowerMinimumAirFlowRate(bool returnIP=false) const;

    double motorEfficiency() const;

    Quantity getMotorEfficiency(bool returnIP=false) const;

    bool isMotorEfficiencyDefaulted() const;

    double motorInAirstreamFraction() const;

    Quantity getMotorInAirstreamFraction(bool returnIP=false) const;

    bool isMotorInAirstreamFractionDefaulted() const;

    boost::optional<double> fanPowerCoefficient1() const;

    OSOptionalQuantity getFanPowerCoefficient1(bool returnIP=false) const;

    boost::optional<double> fanPowerCoefficient2() const;

    OSOptionalQuantity getFanPowerCoefficient2(bool returnIP=false) const;

    boost::optional<double> fanPowerCoefficient3() const;

    OSOptionalQuantity getFanPowerCoefficient3(bool returnIP=false) const;

    boost::optional<double> fanPowerCoefficient4() const;

    OSOptionalQuantity getFanPowerCoefficient4(bool returnIP=false) const;

    boost::optional<double> fanPowerCoefficient5() const;

    OSOptionalQuantity getFanPowerCoefficient5(bool returnIP=false) const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFanEfficiency(double fanEfficiency);

    bool setFanEfficiency(const Quantity& fanEfficiency);

    void resetFanEfficiency();

    void setPressureRise(double pressureRise);

    bool setPressureRise(const Quantity& pressureRise);

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

    bool setMaximumFlowRate(const OSOptionalQuantity& maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    bool setFanPowerMinimumFlowRateInputMethod(std::string fanPowerMinimumFlowRateInputMethod);

    void resetFanPowerMinimumFlowRateInputMethod();

    bool setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction);

    bool setFanPowerMinimumFlowFraction(const Quantity& fanPowerMinimumFlowFraction);

    void resetFanPowerMinimumFlowFraction();

    bool setFanPowerMinimumAirFlowRate(boost::optional<double> fanPowerMinimumAirFlowRate);

    bool setFanPowerMinimumAirFlowRate(const OSOptionalQuantity& fanPowerMinimumAirFlowRate);

    void resetFanPowerMinimumAirFlowRate();

    bool setMotorEfficiency(double motorEfficiency);

    bool setMotorEfficiency(const Quantity& motorEfficiency);

    void resetMotorEfficiency();

    bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

    bool setMotorInAirstreamFraction(const Quantity& motorInAirstreamFraction);

    void resetMotorInAirstreamFraction();

    void setFanPowerCoefficient1(boost::optional<double> fanPowerCoefficient1);

    bool setFanPowerCoefficient1(const OSOptionalQuantity& fanPowerCoefficient1);

    void resetFanPowerCoefficient1();

    void setFanPowerCoefficient2(boost::optional<double> fanPowerCoefficient2);

    bool setFanPowerCoefficient2(const OSOptionalQuantity& fanPowerCoefficient2);

    void resetFanPowerCoefficient2();

    void setFanPowerCoefficient3(boost::optional<double> fanPowerCoefficient3);

    bool setFanPowerCoefficient3(const OSOptionalQuantity& fanPowerCoefficient3);

    void resetFanPowerCoefficient3();

    void setFanPowerCoefficient4(boost::optional<double> fanPowerCoefficient4);

    bool setFanPowerCoefficient4(const OSOptionalQuantity& fanPowerCoefficient4);

    void resetFanPowerCoefficient4();

    void setFanPowerCoefficient5(boost::optional<double> fanPowerCoefficient5);

    bool setFanPowerCoefficient5(const OSOptionalQuantity& fanPowerCoefficient5);

    void resetFanPowerCoefficient5();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
  protected:
  private:
    REGISTER_LOGGER("openstudio.model.FanVariableVolume");

    openstudio::Quantity fanEfficiency_SI() const;
    openstudio::Quantity fanEfficiency_IP() const;
    openstudio::Quantity pressureRise_SI() const;
    openstudio::Quantity pressureRise_IP() const;
    openstudio::OSOptionalQuantity maximumFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumFlowRate_IP() const;
    std::vector<std::string> fanPowerMinimumFlowRateInputMethodValues() const;
    openstudio::Quantity fanPowerMinimumFlowFraction_SI() const;
    openstudio::Quantity fanPowerMinimumFlowFraction_IP() const;
    openstudio::OSOptionalQuantity fanPowerMinimumAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity fanPowerMinimumAirFlowRate_IP() const;
    openstudio::Quantity motorEfficiency_SI() const;
    openstudio::Quantity motorEfficiency_IP() const;
    openstudio::Quantity motorInAirstreamFraction_SI() const;
    openstudio::Quantity motorInAirstreamFraction_IP() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient1_SI() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient1_IP() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient2_SI() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient2_IP() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient3_SI() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient3_IP() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient4_SI() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient4_IP() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient5_SI() const;
    openstudio::OSOptionalQuantity fanPowerCoefficient5_IP() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FANVARIABLEVOLUME_IMPL_HPP

