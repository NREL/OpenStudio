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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Curve; //UniVariateCurves;
class ThermalZone;

namespace detail {

  /** ElectricLoadCenterStorageConverter_Impl is a ParentObject_Impl that is the implementation class for ElectricLoadCenterStorageConverter.*/
  class MODEL_API ElectricLoadCenterStorageConverter_Impl : public ParentObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricLoadCenterStorageConverter_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    ElectricLoadCenterStorageConverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    ElectricLoadCenterStorageConverter_Impl(const ElectricLoadCenterStorageConverter_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~ElectricLoadCenterStorageConverter_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    std::string powerConversionEfficiencyMethod() const;

    // Required if powerConversionEfficiencyMethod == "SimpleFixed"
    boost::optional<double> simpleFixedEfficiency() const;

    // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
    boost::optional<double> designMaximumContinuousInputPower() const;

    // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
    // TODO: Later may need to add support for Tables. (UniVariateCurves, UniVariateTables.)
    boost::optional<Curve> efficiencyFunctionofPowerCurve() const;

    // Defaults to 0.0
    double ancillaryPowerConsumedInStandby() const;

    bool isAncillaryPowerConsumedInStandbyDefaulted() const;

    boost::optional<ThermalZone> thermalZone() const;

    // Defualts to 0.0, because E+ doc says that if omitted, all heat gains are conductive
    double radiativeFraction() const;

    bool isRadiativeFractionDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    // No Setter, will be done when calling one of the three following functions
    //bool setPowerConversionEfficiencyMethod(const std::string& powerConversionEfficiencyMethod);

    bool setSimpleFixedEfficiency(double simpleFixedEfficiency);

    void setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower);

    bool setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve);

    void setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby);

    void resetAncillaryPowerConsumedInStandby();

    bool setThermalZone(const ThermalZone& thermalZone);

    void resetThermalZone();

    bool setRadiativeFraction(double radiativeFraction);

    void resetRadiativeFraction();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageConverter");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP

