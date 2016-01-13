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

#ifndef MODEL_FLUIDCOOLERSINGLESPEED_IMPL_HPP
#define MODEL_FLUIDCOOLERSINGLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  /** FluidCoolerSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for FluidCoolerSingleSpeed.*/
  class MODEL_API FluidCoolerSingleSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    FluidCoolerSingleSpeed_Impl(const IdfObject& idfObject,
                                Model_Impl* model,
                                bool keepHandle);

    FluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    FluidCoolerSingleSpeed_Impl(const FluidCoolerSingleSpeed_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~FluidCoolerSingleSpeed_Impl() {}

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

    boost::optional<double> designAirFlowRateUfactorTimesAreaValue() const;

    bool isDesignAirFlowRateUfactorTimesAreaValueAutosized() const;

    double nominalCapacity() const;

    double designEnteringWaterTemperature() const;

    double designEnteringAirTemperature() const;

    double designEnteringAirWetbulbTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> designAirFlowRate() const;

    bool isDesignAirFlowRateAutosized() const;

    boost::optional<double> designAirFlowRateFanPower() const;

    bool isDesignAirFlowRateFanPowerAutosized() const;

    boost::optional<Node> outdoorAirInletNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    bool setDesignAirFlowRateUfactorTimesAreaValue(boost::optional<double> designAirFlowRateUfactorTimesAreaValue);

    void autosizeDesignAirFlowRateUfactorTimesAreaValue();

    bool setNominalCapacity(double nominalCapacity);

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void autosizeDesignWaterFlowRate();

    bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

    void autosizeDesignAirFlowRate();

    bool setDesignAirFlowRateFanPower(boost::optional<double> designAirFlowRateFanPower);

    void autosizeDesignAirFlowRateFanPower();

    bool setOutdoorAirInletNode(const boost::optional<Node>& node);

    void resetOutdoorAirInletNode();

    //@}
    /** @name Other */
    //@{

    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.FluidCoolerSingleSpeed");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FLUIDCOOLERSINGLESPEED_IMPL_HPP

