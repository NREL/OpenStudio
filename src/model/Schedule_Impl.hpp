/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULE_IMPL_HPP
#define MODEL_SCHEDULE_IMPL_HPP

#include "ScheduleBase_Impl.hpp"

namespace openstudio {
namespace model {

  class ScheduleTypeLimits;

  namespace detail {

    // derive Schedule_Impl from ResourceObject_Impl to override virtual methods
    class MODEL_API Schedule_Impl : public ScheduleBase_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      Schedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      Schedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      Schedule_Impl(const Schedule_Impl& other, Model_Impl* model, bool keepHandles);

      // virtual destructor
      virtual ~Schedule_Impl() override = default;

      //@}
     protected:
      virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const override;

      virtual bool okToResetScheduleTypeLimits() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Schedule");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULE_IMPL_HPP
