/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEACTUATOR_HPP
#define MODEL_EXTERNALINTERFACEACTUATOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceActuator_Impl;

  }  // namespace detail

  /** ExternalInterfaceActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:Actuator'. */
  class MODEL_API ExternalInterfaceActuator : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceActuator(const ModelObject& modelObject);

    explicit ExternalInterfaceActuator(const ModelObject& modelObject, const std::string actuatedComponentType,
                                       const std::string actuatedComponentControlType);

    virtual ~ExternalInterfaceActuator() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceActuator(const ExternalInterfaceActuator& other) = default;
    ExternalInterfaceActuator(ExternalInterfaceActuator&& other) = default;
    ExternalInterfaceActuator& operator=(const ExternalInterfaceActuator&) = default;
    ExternalInterfaceActuator& operator=(ExternalInterfaceActuator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject actuatedComponentUnique() const;

    std::string actuatedComponentType() const;

    std::string actuatedComponentControlType() const;

    boost::optional<double> optionalInitialValue() const;

    bool exportToBCVTB() const;

    bool isExportToBCVTBDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setActuatedComponentUnique(const ModelObject& modelObject);

    bool setActuatedComponentType(const std::string& actuatedComponentType);

    bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

    bool setOptionalInitialValue(double optionalInitialValue);

    void resetOptionalInitialValue();

    bool setExportToBCVTB(bool exportToBCVTB);

    void resetExportToBCVTB();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceActuator_Impl;

    explicit ExternalInterfaceActuator(std::shared_ptr<detail::ExternalInterfaceActuator_Impl> impl);

    friend class detail::ExternalInterfaceActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceActuator");
  };

  /** \relates ExternalInterfaceActuator*/
  using OptionalExternalInterfaceActuator = boost::optional<ExternalInterfaceActuator>;

  /** \relates ExternalInterfaceActuator*/
  using ExternalInterfaceActuatorVector = std::vector<ExternalInterfaceActuator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEACTUATOR_HPP
