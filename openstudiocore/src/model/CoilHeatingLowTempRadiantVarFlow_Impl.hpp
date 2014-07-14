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

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

//#include <utilities/units/Quantity.hpp>
//#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoilHeatingLowTempRadiantVarFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingLowTempRadiantVarFlow.*/
  class MODEL_API CoilHeatingLowTempRadiantVarFlow_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingLowTempRadiantVarFlow_Impl(const CoilHeatingLowTempRadiantVarFlow_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilHeatingLowTempRadiantVarFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;
    
    virtual unsigned inletPort();

    virtual unsigned outletPort();    
    
    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;
    
    //virtual ModelObject clone(Model model) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> maximumHotWaterFlow() const;

    // OSOptionalQuantity getMaximumHotWaterFlow(bool returnIP=false) const;

    bool isMaximumHotWaterFlowDefaulted() const;

    bool isMaximumHotWaterFlowAutosized() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //Connection heatingWaterInletNode() const;

    //// TODO: Check return type. From object lists, some candidates are: Connection.
    //Connection heatingWaterOutletNode() const;

    double heatingControlThrottlingRange() const;

    //Quantity getHeatingControlThrottlingRange(bool returnIP=false) const;

    bool isHeatingControlThrottlingRangeDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> heatingControlTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumHotWaterFlow(boost::optional<double> maximumHotWaterFlow);

    //bool setMaximumHotWaterFlow(const OSOptionalQuantity& maximumHotWaterFlow);

    void resetMaximumHotWaterFlow();

    void autosizeMaximumHotWaterFlow();

    bool setHeatingControlThrottlingRange(double heatingControlThrottlingRange);

    //bool setHeatingControlThrottlingRange(const Quantity& heatingControlThrottlingRange);

    void resetHeatingControlThrottlingRange();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setHeatingControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingControlTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    bool addToNode(Node & node);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantVarFlow");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    //boost::optional<Connection> optionalHeatingWaterInletNode() const;
    //boost::optional<Connection> optionalHeatingWaterOutletNode() const;

    //openstudio::OSOptionalQuantity maximumHotWaterFlow_SI() const;
    //openstudio::OSOptionalQuantity maximumHotWaterFlow_IP() const;
    //openstudio::Quantity heatingControlThrottlingRange_SI() const;
    //openstudio::Quantity heatingControlThrottlingRange_IP() const;

    //boost::optional<ModelObject> heatingWaterInletNodeAsModelObject() const;
    //boost::optional<ModelObject> heatingWaterOutletNodeAsModelObject() const;
    
    boost::optional<ModelObject> heatingControlTemperatureScheduleAsModelObject() const;

    //bool setHeatingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    //bool setHeatingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    
    bool setHeatingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

