/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

