/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOACTUATOR_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOACTUATOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitExportToActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitExport:To:Actuator'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToActuator : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitExportToActuator(const ModelObject& modelObject, const std::string& actuatedComponentType,
                                                                   const std::string& actuatedComponentControlType,
                                                                   const std::string& fMUVariableName, double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitExportToActuator() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitExportToActuator(const ExternalInterfaceFunctionalMockupUnitExportToActuator& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToActuator(ExternalInterfaceFunctionalMockupUnitExportToActuator&& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToActuator& operator=(const ExternalInterfaceFunctionalMockupUnitExportToActuator&) = default;
    ExternalInterfaceFunctionalMockupUnitExportToActuator& operator=(ExternalInterfaceFunctionalMockupUnitExportToActuator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject actuatedComponentUnique() const;

    std::string actuatedComponentType() const;

    std::string actuatedComponentControlType() const;

    std::string fMUVariableName() const;

    double initialValue() const;

    //@}
    /** @name Setters */
    //@{

    bool setActuatedComponentUnique(const ModelObject& modelObject);

    bool setActuatedComponentType(const std::string& actuatedComponentType);

    bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

    bool setFMUVariableName(const std::string& fMUVariableName);

    bool setInitialValue(double initialValue);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitExportToActuator(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToActuator");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToActuator*/
  using OptionalExternalInterfaceFunctionalMockupUnitExportToActuator = boost::optional<ExternalInterfaceFunctionalMockupUnitExportToActuator>;

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToActuator*/
  using ExternalInterfaceFunctionalMockupUnitExportToActuatorVector = std::vector<ExternalInterfaceFunctionalMockupUnitExportToActuator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOACTUATOR_HPP
