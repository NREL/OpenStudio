/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RESOURCEOBJECT_IMPL_HPP
#define MODEL_RESOURCEOBJECT_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class ResourceObject;

  namespace detail {

    class MODEL_API ResourceObject_Impl : public ParentObject_Impl
    {
     public:
      // Construct completely new object.
      ResourceObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ResourceObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ResourceObject_Impl(const ResourceObject_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ResourceObject_Impl() override = default;

      unsigned directUseCount(bool excludeChildren) const;

      unsigned nonResourceObjectUseCount(bool excludeChildren) const;

     private:
      REGISTER_LOGGER("openstudio.model.ResourceObject");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RESOURCEOBJECT_IMPL_HPP
