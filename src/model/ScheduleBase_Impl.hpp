/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEBASE_IMPL_HPP
#define MODEL_SCHEDULEBASE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {

namespace model {

  class ScheduleTypeLimits;

  namespace detail {

    /** ScheduleBase_Impl is a ResourceObject_Impl that is the implementation class for ScheduleBase.*/
    class MODEL_API ScheduleBase_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ScheduleBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ScheduleBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ScheduleBase_Impl(const ScheduleBase_Impl& other, Model_Impl* model, bool keepHandles);

      // virtual destructor
      virtual ~ScheduleBase_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const = 0;

      virtual std::vector<double> values() const = 0;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) = 0;

      virtual bool resetScheduleTypeLimits() = 0;

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() = 0;

      //@}
     protected:
      virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const = 0;

      virtual bool okToResetScheduleTypeLimits() const = 0;

      bool valuesAreWithinBounds() const;

     private:
      REGISTER_LOGGER("openstudio.model.ScheduleBase");

      boost::optional<ModelObject> scheduleTypeLimitsAsModelObject() const;

      bool setScheduleTypeLimitsAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEBASE_IMPL_HPP
