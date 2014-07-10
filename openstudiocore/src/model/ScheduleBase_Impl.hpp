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

#ifndef MODEL_SCHEDULEBASE_IMPL_HPP
#define MODEL_SCHEDULEBASE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {

class OSQuantityVector;

namespace model {

class ScheduleTypeLimits;

namespace detail {

  /** ScheduleBase_Impl is a ResourceObject_Impl that is the implementation class for ScheduleBase.*/
  class MODEL_API ScheduleBase_Impl : public ResourceObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> scheduleTypeLimits READ scheduleTypeLimitsAsModelObject WRITE setScheduleTypeLimitsAsModelObject RESET resetScheduleTypeLimits);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    ScheduleBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ScheduleBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    // clone copy constructor
    ScheduleBase_Impl(const ScheduleBase_Impl& other, Model_Impl* model,bool keepHandles);

    // virtual destructor
    virtual ~ScheduleBase_Impl(){}

    //@}
    /** @name Getters */
    //@{

    virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const = 0;

    virtual std::vector<double> values() const = 0;

    OSQuantityVector getValues(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) = 0;

    virtual bool resetScheduleTypeLimits() = 0;

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays() = 0;

    //@}
   protected:
    boost::optional<Quantity> toQuantity(double value, bool returnIP=false) const;

    boost::optional<double> toDouble(const Quantity& quantity) const;

    virtual void populateValidityReport(ValidityReport& report,bool checkNames) const;

    virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const = 0;

    virtual bool okToResetScheduleTypeLimits() const = 0;

    bool valuesAreWithinBounds() const;

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleBase");

    boost::optional<ModelObject> scheduleTypeLimitsAsModelObject() const;

    bool setScheduleTypeLimitsAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULEBASE_IMPL_HPP

