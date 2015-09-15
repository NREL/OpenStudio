/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include "Inverter_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<ThermalZone> thermalZone() const;

    boost::optional<double> radiativeFraction() const;

    boost::optional<double> ratedMaximumContinuousOutputPower() const;

    boost::optional<double> nightTareLossPower() const;

    boost::optional<double> nominalVoltageInput() const;

    boost::optional<double> efficiencyat10PowerandNominalVoltage() const;

    boost::optional<double> efficiencyat20PowerandNominalVoltage() const;

    boost::optional<double> efficiencyat30PowerandNominalVoltage() const;

    boost::optional<double> efficiencyat50PowerandNominalVoltage() const;

    boost::optional<double> efficiencyat75PowerandNominalVoltage() const;

    boost::optional<double> efficiencyat100PowerandNominalVoltage() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetThermalZone();

    bool setRadiativeFraction(boost::optional<double> radiativeFraction);

    void resetRadiativeFraction();

    void setRatedMaximumContinuousOutputPower(boost::optional<double> ratedMaximumContinuousOutputPower);

    void resetRatedMaximumContinuousOutputPower();

    void setNightTareLossPower(boost::optional<double> nightTareLossPower);

    void resetNightTareLossPower();

    void setNominalVoltageInput(boost::optional<double> nominalVoltageInput);

    void resetNominalVoltageInput();

    bool setEfficiencyat10PowerandNominalVoltage(boost::optional<double> efficiencyat10PowerandNominalVoltage);

    void resetEfficiencyat10PowerandNominalVoltage();

    bool setEfficiencyat20PowerandNominalVoltage(boost::optional<double> efficiencyat20PowerandNominalVoltage);

    void resetEfficiencyat20PowerandNominalVoltage();

    bool setEfficiencyat30PowerandNominalVoltage(boost::optional<double> efficiencyat30PowerandNominalVoltage);

    void resetEfficiencyat30PowerandNominalVoltage();

    bool setEfficiencyat50PowerandNominalVoltage(boost::optional<double> efficiencyat50PowerandNominalVoltage);

    void resetEfficiencyat50PowerandNominalVoltage();

    bool setEfficiencyat75PowerandNominalVoltage(boost::optional<double> efficiencyat75PowerandNominalVoltage);

    void resetEfficiencyat75PowerandNominalVoltage();

    bool setEfficiencyat100PowerandNominalVoltage(boost::optional<double> efficiencyat100PowerandNominalVoltage);

    void resetEfficiencyat100PowerandNominalVoltage();

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

