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

#ifndef MODEL_FLUIDCOOLERTWOSPEED_IMPL_HPP
#define MODEL_FLUIDCOOLERTWOSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  /** FluidCoolerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for FluidCoolerTwoSpeed.*/
  class MODEL_API FluidCoolerTwoSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    FluidCoolerTwoSpeed_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle);

    FluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    FluidCoolerTwoSpeed_Impl(const FluidCoolerTwoSpeed_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~FluidCoolerTwoSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    std::string performanceInputMethod() const;

    boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

    bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

    bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    double highSpeedNominalCapacity() const;

    boost::optional<double> lowSpeedNominalCapacity() const;

    bool isLowSpeedNominalCapacityAutosized() const;

    double lowSpeedNominalCapacitySizingFactor() const;

    double designEnteringWaterTemperature() const;

    double designEnteringAirTemperature() const;

    double designEnteringAirWetbulbTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highFanSpeedAirFlowRate() const;

    bool isHighFanSpeedAirFlowRateAutosized() const;

    boost::optional<double> highFanSpeedFanPower() const;

    bool isHighFanSpeedFanPowerAutosized() const;

    boost::optional<double> lowFanSpeedAirFlowRate() const;

    bool isLowFanSpeedAirFlowRateAutosized() const;

    double lowFanSpeedAirFlowRateSizingFactor() const;

    boost::optional<double> lowFanSpeedFanPower() const;

    bool isLowFanSpeedFanPowerAutosized() const;

    double lowFanSpeedFanPowerSizingFactor() const;

    boost::optional<Node> outdoorAirInletNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    bool setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue);

    void autosizeHighFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue);

    void autosizeLowFanSpeedUfactorTimesAreaValue();

    void setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    bool setHighSpeedNominalCapacity(double highSpeedNominalCapacity);

    bool setLowSpeedNominalCapacity(boost::optional<double> lowSpeedNominalCapacity);

    void autosizeLowSpeedNominalCapacity();

    void setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor);

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void autosizeDesignWaterFlowRate();

    bool setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate);

    void autosizeHighFanSpeedAirFlowRate();

    bool setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower);

    void autosizeHighFanSpeedFanPower();

    bool setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate);

    void autosizeLowFanSpeedAirFlowRate();

    void setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

    bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

    void autosizeLowFanSpeedFanPower();

    void setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

    bool setOutdoorAirInletNode(const boost::optional<Node>& node);

    void resetOutdoorAirInletNode();

    //@}
    /** @name Other */
    //@{

    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.FluidCoolerTwoSpeed");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FLUIDCOOLERTWOSPEED_IMPL_HPP

