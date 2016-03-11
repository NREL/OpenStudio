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

    void setRatedMaximumContinuousOutputPower(boost::optional<double> ratedMaximumContinuousOutputPower);

    void resetRatedMaximumContinuousOutputPower();

    void setNightTareLossPower(boost::optional<double> nightTareLossPower);

    void resetNightTareLossPower();

    void setNominalVoltageInput(boost::optional<double> nominalVoltageInput);

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

