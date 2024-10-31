/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEITEM_IMPL_HPP
#define MODEL_SPACEITEM_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class Space;
  class SpaceItem;

  namespace detail {

    class MODEL_API SpaceItem_Impl : public ModelObject_Impl
    {

     public:
      // Construct completely new object.
      SpaceItem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      SpaceItem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      SpaceItem_Impl(const SpaceItem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceItem_Impl() override = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      /// Returns the parent Space.
      boost::optional<Space> space() const;

      /// Sets the parent Space.
      bool setSpace(const Space& space);

      void resetSpace();

     protected:
      // index of the space name
      virtual int spaceIndex() const = 0;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceItem");

      boost::optional<ModelObject> spaceAsModelObject() const;

      bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEITEM_IMPL_HPP
