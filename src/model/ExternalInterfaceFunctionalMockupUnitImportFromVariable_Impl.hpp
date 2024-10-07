/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitImportFromVariable.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(const ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl& other,
                                                                   Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string outputVariableIndexKeyName() const;

      std::string outputVariableName() const;

      ExternalInterfaceFunctionalMockupUnitImport fMUFile() const;

      std::string fMUInstanceName() const;

      std::string fMUVariableName() const;

      //@}
      /** @name Setters */
      //@{

      bool setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName);

      bool setOutputVariableName(const std::string& outputVariableName);

      bool setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile);

      bool setFMUInstanceName(const std::string& fMUInstanceName);

      bool setFMUVariableName(const std::string& fMUVariableName);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportFromVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_IMPL_HPP
