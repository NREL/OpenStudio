/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERICMODELOBJECT_IMPL_HPP
#define MODEL_GENERICMODELOBJECT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    // derive GenericModelObject_Impl from ModelObject_Impl to override virtual methods
    class MODEL_API GenericModelObject_Impl : public ModelObject_Impl
    {
     public:
      // constructor
      GenericModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      GenericModelObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      GenericModelObject_Impl(const GenericModelObject_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GenericModelObject_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

     private:
      REGISTER_LOGGER("openstudio.model.GenericModelObject");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERICMODELOBJECT_IMPL_HPP
