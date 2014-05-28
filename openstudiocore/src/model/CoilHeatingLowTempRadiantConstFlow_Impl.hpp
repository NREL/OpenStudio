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

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoilHeatingLowTempRadiantConstFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingLowTempRadiantConstFlow.*/
  class MODEL_API CoilHeatingLowTempRadiantConstFlow_Impl : public StraightComponent_Impl {

    public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilHeatingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilHeatingLowTempRadiantConstFlow_Impl(const CoilHeatingLowTempRadiantConstFlow_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~CoilHeatingLowTempRadiantConstFlow_Impl() {}

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

    boost::optional<Schedule> heatingHighWaterTemperatureSchedule() const;

    boost::optional<Schedule> heatingLowWaterTemperatureSchedule() const;

    boost::optional<Schedule> heatingHighControlTemperatureSchedule() const;

    boost::optional<Schedule> heatingLowControlTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingHighWaterTemperatureSchedule(Schedule& schedule);

    void resetHeatingHighWaterTemperatureSchedule();

    bool setHeatingLowWaterTemperatureSchedule(Schedule& schedule);

    void resetHeatingLowWaterTemperatureSchedule();

    bool setHeatingHighControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingHighControlTemperatureSchedule();

    bool setHeatingLowControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingLowControlTemperatureSchedule();

    //@}
    /** @name Other */
    //@{
    
    bool addToNode(Node & node);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantConstFlow");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals

    boost::optional<ModelObject> heatingWaterInletNodeAsModelObject() const;
    boost::optional<ModelObject> heatingWaterOutletNodeAsModelObject() const;
    boost::optional<ModelObject> heatingHighWaterTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingLowWaterTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingHighControlTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingLowControlTemperatureScheduleAsModelObject() const;

    bool setHeatingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

