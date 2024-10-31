/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOVARIABLE_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitExportToVariable.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl(const ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl& other,
                                                                 Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string fMUVariableName() const;

      double initialValue() const;

      //@}
      /** @name Setters */
      //@{

      bool setFMUVariableName(const std::string& fMUVariableName);

      bool setInitialValue(double initialValue);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOVARIABLE_IMPL_HPP
