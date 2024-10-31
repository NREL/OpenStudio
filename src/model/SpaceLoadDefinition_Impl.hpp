/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACELOADDEFINITION_IMPL_HPP
#define MODEL_SPACELOADDEFINITION_IMPL_HPP

#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class SpaceLoadInstance;
  class SpaceLoadDefinition;

  namespace detail {

    class MODEL_API SpaceLoadDefinition_Impl : public ResourceObject_Impl
    {
     public:
      // Construct completely new object.
      SpaceLoadDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      SpaceLoadDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      SpaceLoadDefinition_Impl(const SpaceLoadDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceLoadDefinition_Impl() override = default;

      /// Removes the definition and all instances.
      virtual std::vector<openstudio::IdfObject> remove() override;

      /// Returns all instances of this definition.
      std::vector<SpaceLoadInstance> instances() const;

      /** Returns the floor area associated with instances of this space load definition.
    If two instances are in the same space then that floor area will be double counted.
    If two instances used in the same space type then that floor area will be double counted.
    Space and SpaceLoadInstance multipliers are included in the result. **/
      double floorArea() const;

      /** Returns the number of instances of this space load.
    Space and SpaceLoadInstance multipliers are included in the result **/
      int quantity() const;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceLoadDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOADDEFINITION_IMPL_HPP
