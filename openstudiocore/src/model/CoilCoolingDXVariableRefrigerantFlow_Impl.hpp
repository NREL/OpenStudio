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

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;

namespace detail {

  /** CoilCoolingDXVariableRefrigerantFlow_Impl is a HVACComponent_Impl that is the implementation class for CoilCoolingDXVariableRefrigerantFlow.*/
  class MODEL_API CoilCoolingDXVariableRefrigerantFlow_Impl : public HVACComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingDXVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    CoilCoolingDXVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    CoilCoolingDXVariableRefrigerantFlow_Impl(const CoilCoolingDXVariableRefrigerantFlow_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~CoilCoolingDXVariableRefrigerantFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> ratedSensibleHeatRatio() const;

    bool isRatedSensibleHeatRatioAutosized() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    Curve coolingCapacityRatioModifierFunctionofTemperatureCurve() const;

    Curve coolingCapacityModifierCurveFunctionofFlowFraction() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

    void autosizeRatedTotalCoolingCapacity();

    bool setRatedSensibleHeatRatio(boost::optional<double> ratedSensibleHeatRatio);

    void autosizeRatedSensibleHeatRatio();

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool setCoolingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve);

    bool setCoolingCapacityModifierCurveFunctionofFlowFraction(const Curve& curve);

    //@}
    /** @name Other */
    //@{
    
    ModelObject clone(Model model) const;

    std::vector<ModelObject> children() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlow");

    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<Curve> optionalCoolingCapacityRatioModifierFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalCoolingCapacityModifierCurveFunctionofFlowFraction() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP

