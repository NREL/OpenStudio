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

#ifndef MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP
#define MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Node;

namespace detail {

  /** HeatExchangerFluidToFluid_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatExchangerFluidToFluid.*/
  class MODEL_API HeatExchangerFluidToFluid_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    HeatExchangerFluidToFluid_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle);

    HeatExchangerFluidToFluid_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    HeatExchangerFluidToFluid_Impl(const HeatExchangerFluidToFluid_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~HeatExchangerFluidToFluid_Impl() {}

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

    boost::optional<double> loopDemandSideDesignFlowRate() const;

    bool isLoopDemandSideDesignFlowRateAutosized() const;

    boost::optional<double> loopSupplySideDesignFlowRate() const;

    bool isLoopSupplySideDesignFlowRateAutosized() const;

    std::string heatExchangeModelType() const;

    bool isHeatExchangeModelTypeDefaulted() const;

    boost::optional<double> heatExchangerUFactorTimesAreaValue() const;

    bool isHeatExchangerUFactorTimesAreaValueAutosized() const;

    std::string controlType() const;

    bool isControlTypeDefaulted() const;

    double minimumTemperatureDifferencetoActivateHeatExchanger() const;

    bool isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted() const;

    std::string heatTransferMeteringEndUseType() const;

    bool isHeatTransferMeteringEndUseTypeDefaulted() const;

    boost::optional<Node> componentOverrideLoopSupplySideInletNode() const;

    boost::optional<Node> componentOverrideLoopDemandSideInletNode() const;

    std::string componentOverrideCoolingControlTemperatureMode() const;

    bool isComponentOverrideCoolingControlTemperatureModeDefaulted() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    boost::optional<double> operationMinimumTemperatureLimit() const;

    boost::optional<double> operationMaximumTemperatureLimit() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setLoopDemandSideDesignFlowRate(boost::optional<double> loopDemandSideDesignFlowRate);

    void autosizeLoopDemandSideDesignFlowRate();

    bool setLoopSupplySideDesignFlowRate(boost::optional<double> loopSupplySideDesignFlowRate);

    void autosizeLoopSupplySideDesignFlowRate();

    bool setHeatExchangeModelType(std::string heatExchangeModelType);

    void resetHeatExchangeModelType();

    bool setHeatExchangerUFactorTimesAreaValue(boost::optional<double> heatExchangerUFactorTimesAreaValue);

    void autosizeHeatExchangerUFactorTimesAreaValue();

    bool setControlType(std::string controlType);

    void resetControlType();

    bool setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger);

    void resetMinimumTemperatureDifferencetoActivateHeatExchanger();

    bool setHeatTransferMeteringEndUseType(std::string heatTransferMeteringEndUseType);

    void resetHeatTransferMeteringEndUseType();

    bool setComponentOverrideLoopSupplySideInletNode(const boost::optional<Node>& node);

    void resetComponentOverrideLoopSupplySideInletNode();

    bool setComponentOverrideLoopDemandSideInletNode(const boost::optional<Node>& node);

    void resetComponentOverrideLoopDemandSideInletNode();

    bool setComponentOverrideCoolingControlTemperatureMode(std::string componentOverrideCoolingControlTemperatureMode);

    void resetComponentOverrideCoolingControlTemperatureMode();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    void setOperationMinimumTemperatureLimit(boost::optional<double> operationMinimumTemperatureLimit);

    void resetOperationMinimumTemperatureLimit();

    void setOperationMaximumTemperatureLimit(boost::optional<double> operationMaximumTemperatureLimit);

    void resetOperationMaximumTemperatureLimit();

    //@}
    /** @name Other */
    //@{

    unsigned supplyInletPort();

    unsigned supplyOutletPort();

    unsigned demandInletPort();

    unsigned demandOutletPort();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeatExchangerFluidToFluid");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP

