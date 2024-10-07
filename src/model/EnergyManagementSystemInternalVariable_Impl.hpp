/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** EnergyManagementSystemInternalVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemInternalVariable.*/
    class MODEL_API EnergyManagementSystemInternalVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemInternalVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemInternalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemInternalVariable_Impl(const EnergyManagementSystemInternalVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemInternalVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string internalDataIndexKeyName() const;

      std::string internalDataType() const;

      //@}
      /** @name Setters */
      //@{

      bool setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName);

      void resetInternalDataIndexKeyName();

      bool setInternalDataType(const std::string& internalDataType);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemInternalVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_IMPL_HPP
