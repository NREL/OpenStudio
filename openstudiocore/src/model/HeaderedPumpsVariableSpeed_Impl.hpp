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

#ifndef MODEL_HEADEREDPUMPSVARIABLESPEED_IMPL_HPP
#define MODEL_HEADEREDPUMPSVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** HeaderedPumpsVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for HeaderedPumpsVariableSpeed.*/
  class MODEL_API HeaderedPumpsVariableSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    HeaderedPumpsVariableSpeed_Impl(const IdfObject& idfObject,
                                    Model_Impl* model,
                                    bool keepHandle);

    HeaderedPumpsVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    HeaderedPumpsVariableSpeed_Impl(const HeaderedPumpsVariableSpeed_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    virtual ~HeaderedPumpsVariableSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> totalRatedFlowRate() const;

    bool isTotalRatedFlowRateAutosized() const;

    int numberofPumpsinBank() const;

    std::string flowSequencingControlScheme() const;

    double ratedPumpHead() const;

    boost::optional<double> ratedPowerConsumption() const;

    bool isRatedPowerConsumptionAutosized() const;

    double motorEfficiency() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    double coefficient1ofthePartLoadPerformanceCurve() const;

    double coefficient2ofthePartLoadPerformanceCurve() const;

    double coefficient3ofthePartLoadPerformanceCurve() const;

    double coefficient4ofthePartLoadPerformanceCurve() const;

    double minimumFlowRateFraction() const;

    std::string pumpControlType() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    boost::optional<ThermalZone> thermalZone() const;

    double skinLossRadiativeFraction() const;

    //@}
    /** @name Setters */
    //@{

    bool setTotalRatedFlowRate(boost::optional<double> totalRatedFlowRate);

    void autosizeTotalRatedFlowRate();

    void setNumberofPumpsinBank(int numberofPumpsinBank);

    bool setFlowSequencingControlScheme(std::string flowSequencingControlScheme);

    void setRatedPumpHead(double ratedPumpHead);

    void setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

    void autosizeRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    void setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

    void setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

    void setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

    void setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

    bool setMinimumFlowRateFraction(double minimumFlowRateFraction);

    bool setPumpControlType(std::string pumpControlType);

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetThermalZone();

    bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;

    unsigned outletPort() override;

    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeaderedPumpsVariableSpeed");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEADEREDPUMPSVARIABLESPEED_IMPL_HPP

