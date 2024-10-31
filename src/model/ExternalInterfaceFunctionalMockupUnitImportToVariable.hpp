/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOVARIABLE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitImportToVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport:To:Variable'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitImportToVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitImportToVariable(const Model& model, const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                   const std::string& fMUInstanceName, const std::string& fMUVariableName,
                                                                   double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitImportToVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitImportToVariable(const ExternalInterfaceFunctionalMockupUnitImportToVariable& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportToVariable(ExternalInterfaceFunctionalMockupUnitImportToVariable&& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportToVariable& operator=(const ExternalInterfaceFunctionalMockupUnitImportToVariable&) = default;
    ExternalInterfaceFunctionalMockupUnitImportToVariable& operator=(ExternalInterfaceFunctionalMockupUnitImportToVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ExternalInterfaceFunctionalMockupUnitImport fMUFile() const;

    std::string fMUInstanceName() const;

    std::string fMUVariableName() const;

    double initialValue() const;

    //@}
    /** @name Setters */
    //@{

    bool setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile);

    bool setFMUInstanceName(const std::string& fMUInstanceName);

    bool setFMUVariableName(const std::string& fMUVariableName);

    bool setInitialValue(double initialValue);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitImportToVariable(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportToVariable");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitImportToVariable*/
  using OptionalExternalInterfaceFunctionalMockupUnitImportToVariable = boost::optional<ExternalInterfaceFunctionalMockupUnitImportToVariable>;

  /** \relates ExternalInterfaceFunctionalMockupUnitImportToVariable*/
  using ExternalInterfaceFunctionalMockupUnitImportToVariableVector = std::vector<ExternalInterfaceFunctionalMockupUnitImportToVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOVARIABLE_HPP
