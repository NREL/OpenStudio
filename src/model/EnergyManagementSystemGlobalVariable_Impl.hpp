/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** EnergyManagementSystemGlobalVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemGlobalVariable.*/
    class MODEL_API EnergyManagementSystemGlobalVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemGlobalVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemGlobalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemGlobalVariable_Impl(const EnergyManagementSystemGlobalVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemGlobalVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      bool exportToBCVTB() const;

      bool isExportToBCVTBDefaulted() const;

      bool setExportToBCVTB(bool exportToBCVTB);

      void resetExportToBCVTB();

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
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemGlobalVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_IMPL_HPP
