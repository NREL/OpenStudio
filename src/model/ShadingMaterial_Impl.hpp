/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADINGMATERIAL_IMPL_HPP
#define MODEL_SHADINGMATERIAL_IMPL_HPP

#include "FenestrationMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class ShadingMaterial;

  namespace detail {

    class MODEL_API ShadingMaterial_Impl : public FenestrationMaterial_Impl
    {
     public:
      // Construct completely new object.
      ShadingMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ShadingMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ShadingMaterial_Impl(const ShadingMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ShadingMaterial_Impl() override = default;

     private:
      REGISTER_LOGGER("openstudio.model.ShadingMaterial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGMATERIAL_IMPL_HPP
