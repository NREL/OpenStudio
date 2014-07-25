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

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

/** CoilCoolingLowTempRadiantVarFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingLowTempRadiantVarFlow.*/
class MODEL_API CoilCoolingLowTempRadiantVarFlow_Impl : public StraightComponent_Impl {
   
    Q_OBJECT

   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilCoolingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilCoolingLowTempRadiantVarFlow_Impl(const CoilCoolingLowTempRadiantVarFlow_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilCoolingLowTempRadiantVarFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;
    
    virtual unsigned inletPort();

    virtual unsigned outletPort();    
    
    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> maximumColdWaterFlow() const;

    bool isMaximumColdWaterFlowDefaulted() const;

    bool isMaximumColdWaterFlowAutosized() const;

    double coolingControlThrottlingRange() const;
    
    bool isCoolingControlThrottlingRangeDefaulted() const;

    boost::optional<Schedule> coolingControlTemperatureSchedule() const;

    std::string condensationControlType() const;

    bool isCondensationControlTypeDefaulted() const;

    double condensationControlDewpointOffset() const;

    bool isCondensationControlDewpointOffsetDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumColdWaterFlow(boost::optional<double> maximumColdWaterFlow);

    void resetMaximumColdWaterFlow();

    void autosizeMaximumColdWaterFlow();

    bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

    void resetCoolingControlThrottlingRange();

    bool setCoolingControlTemperatureSchedule(Schedule& schedule);

    void resetCoolingControlTemperatureSchedule();

    bool setCondensationControlType(std::string condensationControlType);

    void resetCondensationControlType();

    void setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

    void resetCondensationControlDewpointOffset();

    //@}
    /** @name Other */
    //@{
    
    bool addToNode(Node & node);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantVarFlow");

    std::vector<std::string> condensationControlTypeValues() const;

    //boost::optional<ModelObject> coolingWaterInletNodeAsModelObject() const;
    //boost::optional<ModelObject> coolingWaterOutletNodeAsModelObject() const;
    boost::optional<ModelObject> coolingControlTemperatureScheduleAsModelObject() const;

    //bool setCoolingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    //bool setCoolingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

