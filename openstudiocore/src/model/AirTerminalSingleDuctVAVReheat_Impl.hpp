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

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API AirTerminalSingleDuctVAVReheat_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> minimumAirFlowFractionSchedule READ minimumAirFlowFractionScheduleAsModelObject WRITE setMinimumAirFlowFractionScheduleAsModelObject RESET resetMinimumAirFlowFractionSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctVAVReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    AirTerminalSingleDuctVAVReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    AirTerminalSingleDuctVAVReheat_Impl(const AirTerminalSingleDuctVAVReheat_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~AirTerminalSingleDuctVAVReheat_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove();

    virtual bool isRemovable() const;

    virtual ModelObject clone(Model model) const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    //@}

    HVACComponent reheatCoil() const;

    bool setReheatCoil(HVACComponent & coil);

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> maximumAirFlowRate() const ;

    void setMaximumAirFlowRate( double value );

    void autosizeMaximumAirFlowRate();

    bool isMaximumAirFlowRateAutosized() const;

    std::string zoneMinimumAirFlowMethod();

    void setZoneMinimumAirFlowMethod( std::string value );

    double constantMinimumAirFlowFraction();

    void setConstantMinimumAirFlowFraction( double value );

    double fixedMinimumAirFlowRate();

    void setFixedMinimumAirFlowRate( double value );

    boost::optional<Schedule> minimumAirFlowFractionSchedule() const;

    bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

    void resetMinimumAirFlowFractionSchedule();

    boost::optional<double> maximumHotWaterOrSteamFlowRate();

    void setMaximumHotWaterOrSteamFlowRate( double value );

    void autosizeMaximumHotWaterOrSteamFlowRate();

    bool isMaximumHotWaterOrSteamFlowRateAutosized() const;

    double minimumHotWaterOrSteamFlowRate();

    void setMinimumHotWaterOrStreamFlowRate( double value );

    double convergenceTolerance();

    void setConvergenceTolerance( double value );

    std::string damperHeatingAction();

    void setDamperHeatingAction( std::string value );

    boost::optional<double> maximumFlowPerZoneFloorAreaDuringReheat();

    void resetMaximumFlowPerZoneFloorAreaDuringReheat();

    void setMaximumFlowPerZoneFloorAreaDuringReheat( double value );

    void autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

    bool isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const;

    boost::optional<double> maximumFlowFractionDuringReheat();

    void setMaximumFlowFractionDuringReheat( double value );

    void autosizeMaximumFlowFractionDuringReheat();

    bool isMaximumFlowFractionDuringReheatAutosized() const;

    void resetMaximumFlowFractionDuringReheat();

    double maximumReheatAirTemperature();

    void setMaximumReheatAirTemperature( double value );

   private:

    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVReheat");

    // optional getters for children so can remove() if constructor fails
    boost::optional<HVACComponent> optionalReheatCoil() const;
    boost::optional<Schedule> optionalAvailabilitySchedule() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> minimumAirFlowFractionScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setMinimumAirFlowFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model

} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_IMPL_HPP
