/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOVARIABLE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitExportToVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitExport:To:Variable'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitExportToVariable(const Model& model, const std::string& fMUVariableName, double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitExportToVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitExportToVariable(const ExternalInterfaceFunctionalMockupUnitExportToVariable& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToVariable(ExternalInterfaceFunctionalMockupUnitExportToVariable&& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToVariable& operator=(const ExternalInterfaceFunctionalMockupUnitExportToVariable&) = default;
    ExternalInterfaceFunctionalMockupUnitExportToVariable& operator=(ExternalInterfaceFunctionalMockupUnitExportToVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitExportToVariable(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToVariable");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToVariable*/
  using OptionalExternalInterfaceFunctionalMockupUnitExportToVariable = boost::optional<ExternalInterfaceFunctionalMockupUnitExportToVariable>;

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToVariable*/
  using ExternalInterfaceFunctionalMockupUnitExportToVariableVector = std::vector<ExternalInterfaceFunctionalMockupUnitExportToVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOVARIABLE_HPP
