/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GLAZING_IMPL_HPP
#define MODEL_GLAZING_IMPL_HPP

#include "FenestrationMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class Glazing;

  namespace detail {

    class MODEL_API Glazing_Impl : public FenestrationMaterial_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      Glazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      Glazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      Glazing_Impl(const Glazing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Glazing_Impl() override = default;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.Glazing");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GLAZING_IMPL_HPP
