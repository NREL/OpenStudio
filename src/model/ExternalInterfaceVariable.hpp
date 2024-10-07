/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEVARIABLE_HPP
#define MODEL_EXTERNALINTERFACEVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceVariable_Impl;

  }  // namespace detail

  /** ExternalInterfaceVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:Variable'. */
  class MODEL_API ExternalInterfaceVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceVariable(const Model& model, const std::string& variableName, double initialValue);

    virtual ~ExternalInterfaceVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceVariable(const ExternalInterfaceVariable& other) = default;
    ExternalInterfaceVariable(ExternalInterfaceVariable&& other) = default;
    ExternalInterfaceVariable& operator=(const ExternalInterfaceVariable&) = default;
    ExternalInterfaceVariable& operator=(ExternalInterfaceVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double initialValue() const;

    bool exportToBCVTB() const;

    bool isExportToBCVTBDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setInitialValue(double initialValue);

    bool setExportToBCVTB(bool exportToBCVTB);

    void resetExportToBCVTB();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceVariable_Impl;

    explicit ExternalInterfaceVariable(std::shared_ptr<detail::ExternalInterfaceVariable_Impl> impl);

    friend class detail::ExternalInterfaceVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceVariable");
  };

  /** \relates ExternalInterfaceVariable*/
  using OptionalExternalInterfaceVariable = boost::optional<ExternalInterfaceVariable>;

  /** \relates ExternalInterfaceVariable*/
  using ExternalInterfaceVariableVector = std::vector<ExternalInterfaceVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEVARIABLE_HPP
