/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLOADDEFINITION_IMPL_HPP
#define MODEL_EXTERIORLOADDEFINITION_IMPL_HPP

#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class ExteriorLoadInstance;
  class ExteriorLoadDefinition;

  namespace detail {

    class MODEL_API ExteriorLoadDefinition_Impl : public ResourceObject_Impl
    {
     public:
      // Construct completely new object.
      ExteriorLoadDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ExteriorLoadDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ExteriorLoadDefinition_Impl(const ExteriorLoadDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorLoadDefinition_Impl() override = default;

      /// Removes the definition and all instances.
      virtual std::vector<openstudio::IdfObject> remove() override;

      /// Returns all instances of this definition.
      std::vector<ExteriorLoadInstance> instances() const;

      /** Returns the number of instances of this exterior load definition.
    ExteriorLoadInstance multipliers are included in the result **/
      int quantity() const;

     private:
      REGISTER_LOGGER("openstudio.model.ExteriorLoadDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLOADDEFINITION_IMPL_HPP
