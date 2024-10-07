/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PARENTOBJECT_IMPL_HPP
#define MODEL_PARENTOBJECT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API ParentObject_Impl : public ModelObject_Impl
    {
     public:
      // constructor
      ParentObject_Impl(IddObjectType type, Model_Impl* model);

      // constructor
      ParentObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ParentObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ParentObject_Impl(const ParentObject_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ParentObject_Impl() override = default;

      /// return direct child objects in the hierarchy
      virtual std::vector<ModelObject> children() const;

      /// remove the object from the model's workspace, as well as recursive children (unless they are ResourceObjects and are used somewhere else)
      /// return a new workspace containing any removed object(s)
      virtual std::vector<openstudio::IdfObject> remove() override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual ModelObject clone(Model model) const override;

     private:
      REGISTER_LOGGER("openstudio.model.ParentObject");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PARENTOBJECT_IMPL_HPP
