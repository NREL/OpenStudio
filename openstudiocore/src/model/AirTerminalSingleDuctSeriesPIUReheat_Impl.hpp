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

#ifndef MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** AirTerminalSingleDuctSeriesPIUReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctSeriesPIUReheat.*/
  class MODEL_API AirTerminalSingleDuctSeriesPIUReheat_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctSeriesPIUReheat_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    AirTerminalSingleDuctSeriesPIUReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    AirTerminalSingleDuctSeriesPIUReheat_Impl(const AirTerminalSingleDuctSeriesPIUReheat_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~AirTerminalSingleDuctSeriesPIUReheat_Impl() {}

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

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    boost::optional<double> maximumPrimaryAirFlowRate() const;

    bool isMaximumPrimaryAirFlowRateAutosized() const;

    boost::optional<double> minimumPrimaryAirFlowFraction() const;

    bool isMinimumPrimaryAirFlowFractionAutosized() const;

    HVACComponent fan() const;

    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    double convergenceTolerance() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    bool setMaximumPrimaryAirFlowRate(boost::optional<double> maximumPrimaryAirFlowRate);

    void autosizeMaximumPrimaryAirFlowRate();

    bool setMinimumPrimaryAirFlowFraction(boost::optional<double> minimumPrimaryAirFlowFraction);

    void autosizeMinimumPrimaryAirFlowFraction();

    bool setFan(const HVACComponent& fan);

    bool setReheatCoil(const HVACComponent& coil);

    void setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

    void resetMaximumHotWaterorSteamFlowRate();

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    bool setConvergenceTolerance(double convergenceTolerance);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;
    unsigned outletPort() override;
    unsigned secondaryAirInletPort() const;
    boost::optional<Node> secondaryAirInletNode() const;
    bool addToNode(Node & node) override;
    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;
    void setFanAvailabilitySchedule(Schedule & schedule);

    // This is a non publlic method to draw recirculation air from a plenum
    // Use carefully.  Removing (or moving) plenum will leave model in invalid state
    bool setInducedAirPlenumZone(ThermalZone & thermalZone);

    std::vector<IdfObject> remove() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctSeriesPIUReheat");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<HVACComponent> optionalFan() const;
    boost::optional<HVACComponent> optionalReheatCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_IMPL_HPP

