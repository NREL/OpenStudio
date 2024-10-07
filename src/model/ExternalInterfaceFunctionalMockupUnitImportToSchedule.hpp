/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOSCHEDULE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOSCHEDULE_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitImportToSchedule is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport:To:Schedule'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitImportToSchedule : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitImportToSchedule(const Model& model, const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                   const std::string& fMUInstanceName, const std::string& fMUVariableName,
                                                                   double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitImportToSchedule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitImportToSchedule(const ExternalInterfaceFunctionalMockupUnitImportToSchedule& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportToSchedule(ExternalInterfaceFunctionalMockupUnitImportToSchedule&& other) = default;
    ExternalInterfaceFunctionalMockupUnitImportToSchedule& operator=(const ExternalInterfaceFunctionalMockupUnitImportToSchedule&) = default;
    ExternalInterfaceFunctionalMockupUnitImportToSchedule& operator=(ExternalInterfaceFunctionalMockupUnitImportToSchedule&&) = default;

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
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitImportToSchedule(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportToSchedule");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitImportToSchedule*/
  using OptionalExternalInterfaceFunctionalMockupUnitImportToSchedule = boost::optional<ExternalInterfaceFunctionalMockupUnitImportToSchedule>;

  /** \relates ExternalInterfaceFunctionalMockupUnitImportToSchedule*/
  using ExternalInterfaceFunctionalMockupUnitImportToScheduleVector = std::vector<ExternalInterfaceFunctionalMockupUnitImportToSchedule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOSCHEDULE_HPP
