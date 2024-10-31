/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORT_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitImport_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitImport is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitImport : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitImport(const Model& model, const std::string& fmuName);

    virtual ~ExternalInterfaceFunctionalMockupUnitImport() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitImport(const ExternalInterfaceFunctionalMockupUnitImport& other) = default;
    ExternalInterfaceFunctionalMockupUnitImport(ExternalInterfaceFunctionalMockupUnitImport&& other) = default;
    ExternalInterfaceFunctionalMockupUnitImport& operator=(const ExternalInterfaceFunctionalMockupUnitImport&) = default;
    ExternalInterfaceFunctionalMockupUnitImport& operator=(ExternalInterfaceFunctionalMockupUnitImport&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double fMUTimeout() const;

    bool isFMUTimeoutDefaulted() const;

    int fMULoggingOn() const;

    bool isFMULoggingOnDefaulted() const;

    std::string fMUFileName() const;

    //@}
    /** @name Setters */
    //@{

    bool setFMUTimeout(double fMUTimeout);

    void resetFMUTimeout();

    bool setFMULoggingOn(int fMULoggingOn);

    void resetFMULoggingOn();

    bool setFMUFileName(const std::string& fMUName);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitImport_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitImport(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitImport_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImport");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitImport*/
  using OptionalExternalInterfaceFunctionalMockupUnitImport = boost::optional<ExternalInterfaceFunctionalMockupUnitImport>;

  /** \relates ExternalInterfaceFunctionalMockupUnitImport*/
  using ExternalInterfaceFunctionalMockupUnitImportVector = std::vector<ExternalInterfaceFunctionalMockupUnitImport>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORT_HPP
