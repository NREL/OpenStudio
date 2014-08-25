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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Connection;
class HeatingCoilName;
class CoolingCoilName;
class ZoneMixers;

namespace detail {

  /** AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeFourPipeInduction.*/
  class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const IdfObject& idfObject,
                                                              Model_Impl* model,
                                                              bool keepHandle);

    AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                              Model_Impl* model,
                                                              bool keepHandle);

    AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl& other,
                                                              Model_Impl* model,
                                                              bool keepHandle);

    virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl() {}

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

    boost::optional<double> maximumTotalAirFlowRate() const;

    bool isMaximumTotalAirFlowRateAutosized() const;

    double inductionRatio() const;

    bool isInductionRatioDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection supplyAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection inducedAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection airOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: HeatingCoilName.
    HeatingCoilName heatingCoil() const;

    boost::optional<double> maximumHotWaterFlowRate() const;

    bool isMaximumHotWaterFlowRateAutosized() const;

    double minimumHotWaterFlowRate() const;

    bool isMinimumHotWaterFlowRateDefaulted() const;

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: CoolingCoilName.
    boost::optional<CoolingCoilName> coolingCoil() const;

    boost::optional<double> maximumColdWaterFlowRate() const;

    bool isMaximumColdWaterFlowRateAutosized() const;

    double minimumColdWaterFlowRate() const;

    bool isMinimumColdWaterFlowRateDefaulted() const;

    double coolingConvergenceTolerance() const;

    bool isCoolingConvergenceToleranceDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: ZoneMixers.
    boost::optional<ZoneMixers> zoneMixer() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumTotalAirFlowRate(boost::optional<double> maximumTotalAirFlowRate);

    void autosizeMaximumTotalAirFlowRate();

    bool setInductionRatio(double inductionRatio);

    void resetInductionRatio();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setSupplyAirInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setInducedAirInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: HeatingCoilName.
    bool setHeatingCoil(const HeatingCoilName& heatingCoilName);

    void setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate);

    void resetMaximumHotWaterFlowRate();

    void autosizeMaximumHotWaterFlowRate();

    bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

    void resetMinimumHotWaterFlowRate();

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    // TODO: Check argument type. From object lists, some candidates are: CoolingCoilName.
    bool setCoolingCoil(const boost::optional<CoolingCoilName>& coolingCoilName);

    void resetCoolingCoil();

    void setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate);

    void resetMaximumColdWaterFlowRate();

    void autosizeMaximumColdWaterFlowRate();

    bool setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

    void resetMinimumColdWaterFlowRate();

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    void resetCoolingConvergenceTolerance();

    // TODO: Check argument type. From object lists, some candidates are: ZoneMixers.
    bool setZoneMixer(const boost::optional<ZoneMixers>& zoneMixers);

    void resetZoneMixer();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeInduction");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalSupplyAirInletNode() const;
    boost::optional<Connection> optionalInducedAirInletNode() const;
    boost::optional<Connection> optionalAirOutletNode() const;
    boost::optional<HeatingCoilName> optionalHeatingCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_IMPL_HPP

