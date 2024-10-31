/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FENESTRATIONMATERIAL_IMPL_HPP
#define MODEL_FENESTRATIONMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class FenestrationMaterial;

  namespace detail {

    class MODEL_API FenestrationMaterial_Impl : public Material_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      FenestrationMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      FenestrationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      FenestrationMaterial_Impl(const FenestrationMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FenestrationMaterial_Impl() override = default;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.FenestrationMaterial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FENESTRATIONMATERIAL_IMPL_HPP
