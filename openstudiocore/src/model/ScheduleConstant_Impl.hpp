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

#ifndef MODEL_SCHEDULECONSTANT_IMPL_HPP
#define MODEL_SCHEDULECONSTANT_IMPL_HPP 

#include "Schedule_Impl.hpp"

namespace openstudio {
namespace model {

class ScheduleConstant;

namespace detail {

  class MODEL_API ScheduleConstant_Impl : public Schedule_Impl {
    Q_OBJECT;
    Q_PROPERTY(double value READ value WRITE setValue);
    // ETH@20120724 Could have value_SI, value_IP properties, but these should be
    // boost::optional<Quantity>, not OSOptionalQuantity. The latter is currently supported
    // by Qt + Attribute, but the former is not.
   public:

    /** @name Constructors and Destructors */
    //@{

    // constructor
    ScheduleConstant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ScheduleConstant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                         Model_Impl* model, 
                         bool keepHandle);

    ScheduleConstant_Impl(const ScheduleConstant_Impl& other, Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~ScheduleConstant_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

    virtual std::vector<double> values() const;

    //@}
    /** @name Getters */
    //@{

    double value() const;

    boost::optional<Quantity> getValue(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    virtual bool resetScheduleTypeLimits();

    void setValue(double value);

    bool setValue(const Quantity& value);

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleConstant");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULECONSTANT_IMPL_HPP
