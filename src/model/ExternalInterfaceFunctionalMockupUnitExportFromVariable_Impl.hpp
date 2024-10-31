/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTFROMVARIABLE_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTFROMVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitExportFromVariable.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(const ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl& other,
                                                                   Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl() override = default;

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

      std::string fMUVariableName() const;

      //@}
      /** @name Setters */
      //@{

      bool setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName);

      bool setOutputVariableName(const std::string& outputVariableName);

      bool setFMUVariableName(const std::string& fMUVariableName);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportFromVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTFROMVARIABLE_IMPL_HPP
