/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

