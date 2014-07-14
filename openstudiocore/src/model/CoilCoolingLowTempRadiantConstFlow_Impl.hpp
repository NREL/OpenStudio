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

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoilCoolingLowTempRadiantConstFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingLowTempRadiantConstFlow.*/
  class MODEL_API CoilCoolingLowTempRadiantConstFlow_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilCoolingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilCoolingLowTempRadiantConstFlow_Impl(const CoilCoolingLowTempRadiantConstFlow_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~CoilCoolingLowTempRadiantConstFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;
    
    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    virtual bool addToNode(Node & node);


    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> coolingHighWaterTemperatureSchedule() const;

    boost::optional<Schedule> coolingLowWaterTemperatureSchedule() const;

    boost::optional<Schedule> coolingHighControlTemperatureSchedule() const;

    boost::optional<Schedule> coolingLowControlTemperatureSchedule() const;

    std::string condensationControlType() const;

    bool isCondensationControlTypeDefaulted() const;

    double condensationControlDewpointOffset() const;

    bool isCondensationControlDewpointOffsetDefaulted() const;

    //@}
    /** @name Setters */
    //@{


    bool setCoolingHighWaterTemperatureSchedule(Schedule& schedule);

    void resetCoolingHighWaterTemperatureSchedule();

    bool setCoolingLowWaterTemperatureSchedule(Schedule& schedule);

    void resetCoolingLowWaterTemperatureSchedule();

    bool setCoolingHighControlTemperatureSchedule(Schedule& schedule);

    void resetCoolingHighControlTemperatureSchedule();

    bool setCoolingLowControlTemperatureSchedule(Schedule& schedule);

    void resetCoolingLowControlTemperatureSchedule();

    bool setCondensationControlType(std::string condensationControlType);

    void resetCondensationControlType();

    void setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

    void resetCondensationControlDewpointOffset();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantConstFlow");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
 
    std::vector<std::string> condensationControlTypeValues() const;

    boost::optional<ModelObject> coolingHighWaterTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> coolingLowWaterTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> coolingHighControlTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> coolingLowControlTemperatureScheduleAsModelObject() const;

    bool setCoolingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

