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

#ifndef MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Inverter_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** ElectricLoadCenterInverterLookUpTable_Impl is a Inverter_Impl that is the implementation class for ElectricLoadCenterInverterLookUpTable.*/
  class MODEL_API ElectricLoadCenterInverterLookUpTable_Impl : public Inverter_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricLoadCenterInverterLookUpTable_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle);

    ElectricLoadCenterInverterLookUpTable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    ElectricLoadCenterInverterLookUpTable_Impl(const ElectricLoadCenterInverterLookUpTable_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    virtual ~ElectricLoadCenterInverterLookUpTable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    virtual boost::optional<ThermalZone> thermalZone() const override;

    boost::optional<double> radiativeFraction() const;

    boost::optional<double> ratedMaximumContinuousOutputPower() const;

    boost::optional<double> nightTareLossPower() const;

    boost::optional<double> nominalVoltageInput() const;

    boost::optional<double> efficiencyAt10PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt20PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt30PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt50PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt75PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt100PowerAndNominalVoltage() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    virtual bool setThermalZone(const ThermalZone& thermalZone) override;

    virtual void resetThermalZone() override;

    bool setRadiativeFraction(boost::optional<double> radiativeFraction);

    void resetRadiativeFraction();

    bool setRatedMaximumContinuousOutputPower(boost::optional<double> ratedMaximumContinuousOutputPower);

    void resetRatedMaximumContinuousOutputPower();

    bool setNightTareLossPower(boost::optional<double> nightTareLossPower);

    void resetNightTareLossPower();

    bool setNominalVoltageInput(boost::optional<double> nominalVoltageInput);

    void resetNominalVoltageInput();

    bool setEfficiencyAt10PowerAndNominalVoltage(boost::optional<double> efficiencyAt10PowerAndNominalVoltage);

    void resetEfficiencyAt10PowerAndNominalVoltage();

    bool setEfficiencyAt20PowerAndNominalVoltage(boost::optional<double> efficiencyAt20PowerAndNominalVoltage);

    void resetEfficiencyAt20PowerAndNominalVoltage();

    bool setEfficiencyAt30PowerAndNominalVoltage(boost::optional<double> efficiencyAt30PowerAndNominalVoltage);

    void resetEfficiencyAt30PowerAndNominalVoltage();

    bool setEfficiencyAt50PowerAndNominalVoltage(boost::optional<double> efficiencyAt50PowerAndNominalVoltage);

    void resetEfficiencyAt50PowerAndNominalVoltage();

    bool setEfficiencyAt75PowerAndNominalVoltage(boost::optional<double> efficiencyAt75PowerAndNominalVoltage);

    void resetEfficiencyAt75PowerAndNominalVoltage();

    bool setEfficiencyAt100PowerAndNominalVoltage(boost::optional<double> efficiencyAt100PowerAndNominalVoltage);

    void resetEfficiencyAt100PowerAndNominalVoltage();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterLookUpTable");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_IMPL_HPP
