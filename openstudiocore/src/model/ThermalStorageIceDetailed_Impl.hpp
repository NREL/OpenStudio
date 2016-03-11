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

#ifndef MODEL_THERMALSTORAGEICEDETAILED_IMPL_HPP
#define MODEL_THERMALSTORAGEICEDETAILED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {

  /** ThermalStorageIceDetailed_Impl is a StraightComponent_Impl that is the implementation class for ThermalStorageIceDetailed.*/
  class MODEL_API ThermalStorageIceDetailed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ThermalStorageIceDetailed_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle);

    ThermalStorageIceDetailed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    ThermalStorageIceDetailed_Impl(const ThermalStorageIceDetailed_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~ThermalStorageIceDetailed_Impl() {}

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

    double capacity() const;

    Curve dischargingCurve() const;

    Curve chargingCurve() const;

    double timestepoftheCurveData() const;

    double parasiticElectricLoadDuringDischarging() const;

    double parasiticElectricLoadDuringCharging() const;

    double tankLossCoefficient() const;

    double freezingTemperatureofStorageMedium() const;

    std::string thawProcessIndicator() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    void setCapacity(double capacity);

    bool setDischargingCurve(const Curve& dischargingCurve);

    bool setChargingCurve(const Curve& chargingCurve);

    void setTimestepoftheCurveData(double timestepoftheCurveData);

    void setParasiticElectricLoadDuringDischarging(double parasiticElectricLoadDuringDischarging);

    void setParasiticElectricLoadDuringCharging(double parasiticElectricLoadDuringCharging);

    void setTankLossCoefficient(double tankLossCoefficient);

    void setFreezingTemperatureofStorageMedium(double freezingTemperatureofStorageMedium);

    bool setThawProcessIndicator(std::string thawProcessIndicator);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;
    unsigned outletPort() override;

    bool addToNode(Node & node) override;
    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ThermalStorageIceDetailed");

    boost::optional<Curve> optionalDischargingCurve() const;
    boost::optional<Curve> optionalChargingCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_THERMALSTORAGEICEDETAILED_IMPL_HPP

