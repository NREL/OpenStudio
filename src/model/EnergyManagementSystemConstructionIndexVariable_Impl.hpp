/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Construction;

  namespace detail {

    /** EnergyManagementSystemConstructionIndexVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemConstructionIndexVariable.*/
    class MODEL_API EnergyManagementSystemConstructionIndexVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemConstructionIndexVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemConstructionIndexVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemConstructionIndexVariable_Impl(const EnergyManagementSystemConstructionIndexVariable_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~EnergyManagementSystemConstructionIndexVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      ModelObject constructionObject() const;

      //@}
      /** @name Setters */
      //@{

      bool setConstructionObject(const ModelObject& construction);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemConstructionIndexVariable");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<ModelObject> optionalConstructionObject() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_IMPL_HPP
