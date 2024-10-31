/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOACTUATOR_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOACTUATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitImportToActuator.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl(const ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl& other,
                                                                 Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      ModelObject actuatedComponentUnique() const;

      std::string actuatedComponentType() const;

      std::string actuatedComponentControlType() const;

      ExternalInterfaceFunctionalMockupUnitImport fMUFile() const;

      std::string fMUInstanceName() const;

      std::string fMUVariableName() const;

      double initialValue() const;

      //@}
      /** @name Setters */
      //@{

      bool setActuatedComponentUnique(const ModelObject& modelObject);

      bool setActuatedComponentType(const std::string& actuatedComponentType);

      bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

      bool setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile);

      bool setFMUInstanceName(const std::string& fMUInstanceName);

      bool setFMUVariableName(const std::string& fMUVariableName);

      bool setInitialValue(double initialValue);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportToActuator");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOACTUATOR_IMPL_HPP
