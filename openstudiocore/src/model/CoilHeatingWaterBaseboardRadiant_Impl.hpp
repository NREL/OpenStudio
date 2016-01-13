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

#ifndef MODEL_COILHEATINGWATERBASEBOARDRADIANT_IMPL_HPP
#define MODEL_COILHEATINGWATERBASEBOARDRADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CoilHeatingWaterBaseboardRadiant_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingWaterBaseboardRadiant.*/
  class MODEL_API CoilHeatingWaterBaseboardRadiant_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWaterBaseboardRadiant_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingWaterBaseboardRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingWaterBaseboardRadiant_Impl(const CoilHeatingWaterBaseboardRadiant_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilHeatingWaterBaseboardRadiant_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    double ratedAverageWaterTemperature() const;

    double ratedWaterMassFlowRate() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    boost::optional<double> maximumWaterFlowRate() const;

    bool isMaximumWaterFlowRateAutosized() const;

    double convergenceTolerance() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAverageWaterTemperature(double ratedAverageWaterTemperature);

    bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

    bool setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    void setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate);

    void autosizeMaximumWaterFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboardRadiant");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERBASEBOARDRADIANT_IMPL_HPP

