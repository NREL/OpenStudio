/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GASLAYER_IMPL_HPP
#define MODEL_GASLAYER_IMPL_HPP

#include "FenestrationMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class GasLayer;

  namespace detail {

    class MODEL_API GasLayer_Impl : public FenestrationMaterial_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      GasLayer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      GasLayer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      GasLayer_Impl(const GasLayer_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GasLayer_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      virtual boost::optional<double> getVisibleTransmittance() const override;

      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.GasLayer");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GASLAYER_IMPL_HPP
