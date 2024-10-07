/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACELOAD_IMPL_HPP
#define MODEL_SPACELOAD_IMPL_HPP

#include "SpaceItem_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class SpaceType;
  class SpaceLoad;

  namespace detail {

    class MODEL_API SpaceLoad_Impl : public SpaceItem_Impl
    {

     public:
      // Construct completely new object.
      SpaceLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      SpaceLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      SpaceLoad_Impl(const SpaceLoad_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceLoad_Impl() override = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      /// Returns the parent SpaceType.
      boost::optional<SpaceType> spaceType() const;

      /// Sets the parent SpaceType.
      bool setSpaceType(const SpaceType& spaceType);

      void resetSpaceType();

      /// Converts space load levels to absolute values rather than by per area, etc.
      /// Also hard sizes any costs associated with this load.
      /// Only possible if this space load is parented by a space, returns false if
      /// parented by a space type.
      virtual bool hardSize() = 0;

      /// Directly applies default schedules to this space load if found.
      virtual bool hardApplySchedules() = 0;

      virtual bool isAbsolute() const = 0;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceLoad");

      boost::optional<ModelObject> spaceTypeAsModelObject() const;

      bool setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOAD_IMPL_HPP
