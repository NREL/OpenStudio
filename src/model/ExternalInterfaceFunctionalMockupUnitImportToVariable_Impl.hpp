/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOVARIABLE_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitImportToVariable.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl(const ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl& other,
                                                                 Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      ExternalInterfaceFunctionalMockupUnitImport fMUFile() const;

      std::string fMUInstanceName() const;

      std::string fMUVariableName() const;

      double initialValue() const;

      //@}
      /** @name Setters */
      //@{

      bool setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFileName);

      bool setFMUInstanceName(const std::string& fMUInstanceName);

      bool setFMUVariableName(const std::string& fMUVariableName);

      bool setInitialValue(double initialValue);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportToVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOVARIABLE_IMPL_HPP
