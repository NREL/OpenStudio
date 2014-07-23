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

#ifndef MODEL_COILHEATINGWATERBASEBOARD_IMPL_HPP
#define MODEL_COILHEATINGWATERBASEBOARD_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CoilHeatingWaterBaseboard_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingWaterBaseboard.*/
  class MODEL_API CoilHeatingWaterBaseboard_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWaterBaseboard_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle);

    CoilHeatingWaterBaseboard_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    CoilHeatingWaterBaseboard_Impl(const CoilHeatingWaterBaseboard_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~CoilHeatingWaterBaseboard_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    unsigned inletPort();

    unsigned outletPort();

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> uFactorTimesAreaValue() const;

    bool isUFactorTimesAreaValueDefaulted() const;

    bool isUFactorTimesAreaValueAutosized() const;

    boost::optional<double> maximumWaterFlowRate() const;

    bool isMaximumWaterFlowRateDefaulted() const;

    bool isMaximumWaterFlowRateAutosized() const;

    double convergenceTolerance() const;

    bool isConvergenceToleranceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setUFactorTimesAreaValue(boost::optional<double> uFactorTimesAreaValue);

    void resetUFactorTimesAreaValue();

    void autosizeUFactorTimesAreaValue();

    void setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate);

    void resetMaximumWaterFlowRate();

    void autosizeMaximumWaterFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    void resetConvergenceTolerance();

    //@}
    /** @name Other */
    //@{

    bool addToNode(Node & node);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboard");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERBASEBOARD_IMPL_HPP

