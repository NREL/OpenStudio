/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGWATER_IMPL_HPP
#define MODEL_COILHEATINGWATER_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ControllerWaterCoil;

namespace detail {

  class MODEL_API CoilHeatingWater_Impl : public WaterToAirComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
  public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CoilHeatingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    CoilHeatingWater_Impl(const CoilHeatingWater_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~CoilHeatingWater_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove();

    virtual bool removeFromPlantLoop();

    virtual ModelObject clone(Model model) const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool addToNode(Node & node);

    virtual unsigned airInletPort();

    virtual unsigned airOutletPort();

    virtual unsigned waterInletPort();

    virtual unsigned waterOutletPort();

    virtual boost::optional<HVACComponent> containingHVACComponent() const;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Getters and Setters */
    //@{

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> uFactorTimesAreaValue();

    void setUFactorTimesAreaValue( double value );

    bool isUFactorTimesAreaValueAutosized();

    void autosizeUFactorTimesAreaValue();

    boost::optional<double> maximumWaterFlowRate();

    void setMaximumWaterFlowRate( double value );

    bool isMaximumWaterFlowRateAutosized();

    void autosizeMaximumWaterFlowRate();

    std::string performanceInputMethod();

    void setPerformanceInputMethod( std::string value );

    boost::optional<double> ratedCapacity();

    void setRatedCapacity( double value );

    bool isRatedCapacityAutosized();

    void autosizeRatedCapacity();

    double ratedInletWaterTemperature();

    void setRatedInletWaterTemperature( double value );

    double ratedInletAirTemperature();

    void setRatedInletAirTemperature( double value );

    double ratedOutletWaterTemperature();

    void setRatedOutletWaterTemperature( double value );

    double ratedOutletAirTemperature();

    void setRatedOutletAirTemperature( double value );

    double ratedRatioForAirAndWaterConvection();

    void setRatedRatioForAirAndWaterConvection( double value );

    boost::optional<ControllerWaterCoil> controllerWaterCoil();

    //@}
  private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWater");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATER_IMPL_HPP
