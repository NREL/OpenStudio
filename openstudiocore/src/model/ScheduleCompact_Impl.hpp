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

#ifndef MODEL_SCHEDULECOMPACT_IMPL_HPP
#define MODEL_SCHEDULECOMPACT_IMPL_HPP 

#include "Schedule_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API ScheduleCompact_Impl : public Schedule_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    // constructor
    ScheduleCompact_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ScheduleCompact_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                         Model_Impl* model, 
                         bool keepHandle);

    ScheduleCompact_Impl(const ScheduleCompact_Impl& other, Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~ScheduleCompact_Impl(){}

    //@}
    /** @name Getters */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<double> values() const;

    virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;


    //@}
    /** @name Setters */
    //@{

    virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    virtual bool resetScheduleTypeLimits();

    void setToConstantValue(double value);

    bool setToConstantValue(const Quantity& value);

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

    //@}
    /** @name Queries */
    //@{

    bool isConstantValue() const;

    boost::optional<double> constantValue() const;

    boost::optional<Quantity> getConstantValue(bool returnIP=false) const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleCompact");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULECOMPACT_IMPL_HPP
