/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INFRAREDTRANSPARENTMATERIAL_IMPL_HPP
#define MODEL_INFRAREDTRANSPARENTMATERIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelPartitionMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** InfraredTransparentMaterial_Impl is a ModelPartitionMaterial_Impl that is the implementation class for InfraredTransparentMaterial.*/
    class MODEL_API InfraredTransparentMaterial_Impl : public ModelPartitionMaterial_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      InfraredTransparentMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InfraredTransparentMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InfraredTransparentMaterial_Impl(const InfraredTransparentMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InfraredTransparentMaterial_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.InfraredTransparentMaterial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INFRAREDTRANSPARENTMATERIAL_IMPL_HPP
