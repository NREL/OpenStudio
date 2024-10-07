/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGER_IMPL_HPP
#define MODEL_AVAILABILITYMANAGER_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Loop;

  namespace detail {

    class MODEL_API AvailabilityManager_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManager_Impl(IddObjectType type, Model_Impl* model);

      AvailabilityManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManager_Impl(const AvailabilityManager_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~AvailabilityManager_Impl() override = default;

      boost::optional<Loop> loop() const;

     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManager");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGER_IMPL_HPP
