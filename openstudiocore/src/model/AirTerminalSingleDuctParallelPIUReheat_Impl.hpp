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

#ifndef MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  /** AirTerminalSingleDuctParallelPIUReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctParallelPIUReheat.*/
  class MODEL_API AirTerminalSingleDuctParallelPIUReheat_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> maximumPrimaryAirFlowRate READ maximumPrimaryAirFlowRate WRITE setMaximumPrimaryAirFlowRate);
    Q_PROPERTY(bool isMaximumPrimaryAirFlowRateAutosized READ isMaximumPrimaryAirFlowRateAutosized);
    Q_PROPERTY(boost::optional<double> maximumSecondaryAirFlowRate READ maximumSecondaryAirFlowRate WRITE setMaximumSecondaryAirFlowRate);
    Q_PROPERTY(bool isMaximumSecondaryAirFlowRateAutosized READ isMaximumSecondaryAirFlowRateAutosized);
    Q_PROPERTY(boost::optional<double> minimumPrimaryAirFlowFraction READ minimumPrimaryAirFlowFraction WRITE setMinimumPrimaryAirFlowFraction);
    Q_PROPERTY(bool isMinimumPrimaryAirFlowFractionAutosized READ isMinimumPrimaryAirFlowFractionAutosized);
    Q_PROPERTY(boost::optional<double> fanOnFlowFraction READ fanOnFlowFraction WRITE setFanOnFlowFraction);
    Q_PROPERTY(bool isFanOnFlowFractionAutosized READ isFanOnFlowFractionAutosized);
    Q_PROPERTY(boost::optional<double> maximumHotWaterorSteamFlowRate READ maximumHotWaterorSteamFlowRate WRITE setMaximumHotWaterorSteamFlowRate RESET resetMaximumHotWaterorSteamFlowRate);
    Q_PROPERTY(bool isMaximumHotWaterorSteamFlowRateAutosized READ isMaximumHotWaterorSteamFlowRateAutosized);
    Q_PROPERTY(double minimumHotWaterorSteamFlowRate READ minimumHotWaterorSteamFlowRate WRITE setMinimumHotWaterorSteamFlowRate RESET resetMinimumHotWaterorSteamFlowRate);
    Q_PROPERTY(bool isMinimumHotWaterorSteamFlowRateDefaulted READ isMinimumHotWaterorSteamFlowRateDefaulted);
    Q_PROPERTY(double convergenceTolerance READ convergenceTolerance WRITE setConvergenceTolerance RESET resetConvergenceTolerance);
    Q_PROPERTY(bool isConvergenceToleranceDefaulted READ isConvergenceToleranceDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctParallelPIUReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    AirTerminalSingleDuctParallelPIUReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    AirTerminalSingleDuctParallelPIUReheat_Impl(const AirTerminalSingleDuctParallelPIUReheat_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~AirTerminalSingleDuctParallelPIUReheat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<openstudio::IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumPrimaryAirFlowRate() const;

    bool isMaximumPrimaryAirFlowRateAutosized() const;

    boost::optional<double> maximumSecondaryAirFlowRate() const;

    bool isMaximumSecondaryAirFlowRateAutosized() const;

    boost::optional<double> minimumPrimaryAirFlowFraction() const;

    bool isMinimumPrimaryAirFlowFractionAutosized() const;

    boost::optional<double> fanOnFlowFraction() const;

    bool isFanOnFlowFractionAutosized() const;

    boost::optional<Node> secondaryAirInletNode() const;

    unsigned secondaryAirInletPort() const;

    HVACComponent fan() const;

    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

    double convergenceTolerance() const;

    bool isConvergenceToleranceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void setMaximumPrimaryAirFlowRate(boost::optional<double> maximumPrimaryAirFlowRate);

    void autosizeMaximumPrimaryAirFlowRate();

    void setMaximumSecondaryAirFlowRate(boost::optional<double> maximumSecondaryAirFlowRate);

    void autosizeMaximumSecondaryAirFlowRate();

    void setMinimumPrimaryAirFlowFraction(boost::optional<double> minimumPrimaryAirFlowFraction);

    void autosizeMinimumPrimaryAirFlowFraction();

    void setFanOnFlowFraction(boost::optional<double> fanOnFlowFraction);

    void autosizeFanOnFlowFraction();

    void setFan( HVACComponent & hvacComponent );

    void setReheatCoil( HVACComponent & hvacComponent );

    void setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

    void resetMaximumHotWaterorSteamFlowRate();

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    void resetMinimumHotWaterorSteamFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    void resetConvergenceTolerance();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctParallelPIUReheat");

    // optional getters for children() (in case want to remove() before fully constructed).
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalFan() const;
    boost::optional<HVACComponent> optionalReheatCoil() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_IMPL_HPP

