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

#ifndef MODEL_COILCOOLINGWATER_IMPL_HPP
#define MODEL_COILCOOLINGWATER_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ControllerWaterCoil;

namespace detail {

  class MODEL_API CoilCoolingWater_Impl : public WaterToAirComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
  public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CoilCoolingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    CoilCoolingWater_Impl(const CoilCoolingWater_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~CoilCoolingWater_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove();

    virtual bool removeFromPlantLoop();

    virtual ModelObject clone(Model model) const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned airInletPort();

    virtual unsigned airOutletPort();

    virtual unsigned waterInletPort();

    virtual unsigned waterOutletPort();

    virtual bool addToNode(Node & node);

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule & schedule );

    boost::optional<double> designWaterFlowRate();

    void setDesignWaterFlowRate( double value );

    bool isDesignWaterFlowRateAutosized();

    void autosizeDesignWaterFlowRate();

    boost::optional<double> designAirFlowRate();

    void setDesignAirFlowRate( double value );

    bool isDesignAirFlowRateAutosized();

    void autosizeDesignAirFlowRate();

    boost::optional<double> designInletWaterTemperature();

    void setDesignInletWaterTemperature( double value );

    bool isDesignInletWaterTemperatureAutosized();

    void autosizeDesignInletWaterTemperature();

    boost::optional<double> designInletAirTemperature();

    void setDesignInletAirTemperature( double value );

    bool isDesignInletAirTemperatureAutosized();

    void autosizeDesignInletAirTemperature();

    boost::optional<double> designOutletAirTemperature();

    void setDesignOutletAirTemperature( double value );

    bool isDesignOutletAirTemperatureAutosized();

    void autosizeDesignOutletAirTemperature();

    boost::optional<double> designInletAirHumidityRatio();

    void setDesignInletAirHumidityRatio( double value );

    bool isDesignInletAirHumidityRatioAutosized();

    void autosizeDesignInletAirHumidityRatio();

    boost::optional<double> designOutletAirHumidityRatio();

    void setDesignOutletAirHumidityRatio( double value );

    bool isDesignOutletAirHumidityRatioAutosized();

    void autosizeDesignOutletAirHumidityRatio();

    std::string typeOfAnalysis();

    void setTypeOfAnalysis( std::string value );

    std::string heatExchangerConfiguration();

    void setHeatExchangerConfiguration( std::string value );

    boost::optional<ControllerWaterCoil> controllerWaterCoil();

  private:    
    REGISTER_LOGGER("openstudio.model.CoilCoolingWater");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

    virtual boost::optional<HVACComponent> containingHVACComponent() const;
  };

} // detail

} // model

} // openstudio

#endif // MODEL_COILCOOLINGWATER_IMPL_HPP
