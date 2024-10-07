/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTFROMVARIABLE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTFROMVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitExportFromVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitExport:From:Variable'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitExportFromVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitExportFromVariable(const Model& model, const std::string& outputVariableIndexKeyName,
                                                                     const std::string& outputVariableName, const std::string& fMUVariableName);

    virtual ~ExternalInterfaceFunctionalMockupUnitExportFromVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitExportFromVariable(const ExternalInterfaceFunctionalMockupUnitExportFromVariable& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportFromVariable(ExternalInterfaceFunctionalMockupUnitExportFromVariable&& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportFromVariable& operator=(const ExternalInterfaceFunctionalMockupUnitExportFromVariable&) = default;
    ExternalInterfaceFunctionalMockupUnitExportFromVariable& operator=(ExternalInterfaceFunctionalMockupUnitExportFromVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitExportFromVariable(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportFromVariable");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitExportFromVariable*/
  using OptionalExternalInterfaceFunctionalMockupUnitExportFromVariable = boost::optional<ExternalInterfaceFunctionalMockupUnitExportFromVariable>;

  /** \relates ExternalInterfaceFunctionalMockupUnitExportFromVariable*/
  using ExternalInterfaceFunctionalMockupUnitExportFromVariableVector = std::vector<ExternalInterfaceFunctionalMockupUnitExportFromVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTFROMVARIABLE_HPP
