/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOACTUATOR_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOACTUATOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitImportToActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport:To:Actuator'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitImportToActuator : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitImportToActuator(const ModelObject& modelObject, const std::string& actuatedComponentType,
                                                                   const std::string& actuatedComponentControlType,
                                                                   const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                   const std::string& fMUInstanceName, const std::string& fMUVariableName,
                                                                   double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitImportToActuator() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitImportToActuator(const ExternalInterfaceFunctionalMockupUnitImportToActuator& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportToActuator(ExternalInterfaceFunctionalMockupUnitImportToActuator&& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportToActuator& operator=(const ExternalInterfaceFunctionalMockupUnitImportToActuator&) = default;
    ExternalInterfaceFunctionalMockupUnitImportToActuator& operator=(ExternalInterfaceFunctionalMockupUnitImportToActuator&&) = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitImportToActuator(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportToActuator");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitImportToActuator*/
  using OptionalExternalInterfaceFunctionalMockupUnitImportToActuator = boost::optional<ExternalInterfaceFunctionalMockupUnitImportToActuator>;

  /** \relates ExternalInterfaceFunctionalMockupUnitImportToActuator*/
  using ExternalInterfaceFunctionalMockupUnitImportToActuatorVector = std::vector<ExternalInterfaceFunctionalMockupUnitImportToActuator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOACTUATOR_HPP
