/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitImportFromVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport:From:Variable'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitImportFromVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitImportFromVariable(const Model& model, const std::string& outputVariableIndexKeyName,
                                                                     const std::string& outputVariableName,
                                                                     const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                     const std::string& fMUInstanceName, const std::string& fMUVariableName);

    virtual ~ExternalInterfaceFunctionalMockupUnitImportFromVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitImportFromVariable(const ExternalInterfaceFunctionalMockupUnitImportFromVariable& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportFromVariable(ExternalInterfaceFunctionalMockupUnitImportFromVariable&& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportFromVariable& operator=(const ExternalInterfaceFunctionalMockupUnitImportFromVariable&) = default;
    ExternalInterfaceFunctionalMockupUnitImportFromVariable& operator=(ExternalInterfaceFunctionalMockupUnitImportFromVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitImportFromVariable(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportFromVariable");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitImportFromVariable*/
  using OptionalExternalInterfaceFunctionalMockupUnitImportFromVariable = boost::optional<ExternalInterfaceFunctionalMockupUnitImportFromVariable>;

  /** \relates ExternalInterfaceFunctionalMockupUnitImportFromVariable*/
  using ExternalInterfaceFunctionalMockupUnitImportFromVariableVector = std::vector<ExternalInterfaceFunctionalMockupUnitImportFromVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_HPP
