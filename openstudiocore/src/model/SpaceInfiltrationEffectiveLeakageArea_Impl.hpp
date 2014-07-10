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

#ifndef MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP
#define MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class SpaceInfiltrationEffectiveLeakageArea;

namespace detail {

  /** SpaceInfiltrationEffectiveLeakageArea_Impl is a SpaceLoad_Impl that is the implementation class for SpaceInfiltrationEffectiveLeakageArea.*/
  class MODEL_API SpaceInfiltrationEffectiveLeakageArea_Impl : public SpaceLoad_Impl {
    Q_OBJECT;
    Q_PROPERTY(double effectiveAirLeakageArea READ effectiveAirLeakageArea WRITE setEffectiveAirLeakageArea);
    Q_PROPERTY(openstudio::Quantity effectiveAirLeakageArea_SI READ effectiveAirLeakageArea_SI WRITE setEffectiveAirLeakageArea);
    Q_PROPERTY(openstudio::Quantity effectiveAirLeakageArea_IP READ effectiveAirLeakageArea_IP WRITE setEffectiveAirLeakageArea);

    Q_PROPERTY(double stackCoefficient READ stackCoefficient WRITE setStackCoefficient);
    Q_PROPERTY(openstudio::Quantity stackCoefficient_SI READ stackCoefficient_SI WRITE setStackCoefficient);
    Q_PROPERTY(openstudio::Quantity stackCoefficient_IP READ stackCoefficient_IP WRITE setStackCoefficient);

    Q_PROPERTY(double windCoefficient READ windCoefficient WRITE setWindCoefficient);
    Q_PROPERTY(openstudio::Quantity windCoefficient_SI READ windCoefficient_SI WRITE setWindCoefficient);
    Q_PROPERTY(openstudio::Quantity windCoefficient_IP READ windCoefficient_IP WRITE setWindCoefficient);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject RESET resetSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    SpaceInfiltrationEffectiveLeakageArea_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle);

    SpaceInfiltrationEffectiveLeakageArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    SpaceInfiltrationEffectiveLeakageArea_Impl(const SpaceInfiltrationEffectiveLeakageArea_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    virtual ~SpaceInfiltrationEffectiveLeakageArea_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool hardSize();

    virtual bool hardApplySchedules();

    //@}
    /** @name Getters */
    //@{

    /** Returns the (fractional) infiltration schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    double effectiveAirLeakageArea() const;

    Quantity getEffectiveAirLeakageArea(bool returnIP=false) const;

    double stackCoefficient() const;

    Quantity getStackCoefficient(bool returnIP=false) const;

    double windCoefficient() const;

    Quantity getWindCoefficient(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setEffectiveAirLeakageArea(double effectiveAirLeakageArea);

    bool setEffectiveAirLeakageArea(const Quantity& effectiveAirLeakageArea);

    bool setStackCoefficient(double stackCoefficient);

    bool setStackCoefficient(const Quantity& stackCoefficient);

    bool setWindCoefficient(double windCoefficient);

    bool setWindCoefficient(const Quantity& windCoefficient);

    //@}
   protected:
    
    // index of the space name
    virtual int spaceIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationEffectiveLeakageArea");

    openstudio::Quantity effectiveAirLeakageArea_SI() const;
    openstudio::Quantity effectiveAirLeakageArea_IP() const;
    openstudio::Quantity stackCoefficient_SI() const;
    openstudio::Quantity stackCoefficient_IP() const;
    openstudio::Quantity windCoefficient_SI() const;
    openstudio::Quantity windCoefficient_IP() const;

    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP

