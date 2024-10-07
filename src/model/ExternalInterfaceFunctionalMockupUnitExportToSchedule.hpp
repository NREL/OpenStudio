/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitExportToSchedule is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitExport:To:Schedule'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToSchedule : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitExportToSchedule(const Model& model, const std::string& fMUVariableName, double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitExportToSchedule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitExportToSchedule(const ExternalInterfaceFunctionalMockupUnitExportToSchedule& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToSchedule(ExternalInterfaceFunctionalMockupUnitExportToSchedule&& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToSchedule& operator=(const ExternalInterfaceFunctionalMockupUnitExportToSchedule&) = default;
    ExternalInterfaceFunctionalMockupUnitExportToSchedule& operator=(ExternalInterfaceFunctionalMockupUnitExportToSchedule&&) = default;

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
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitExportToSchedule(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToSchedule");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToSchedule*/
  using OptionalExternalInterfaceFunctionalMockupUnitExportToSchedule = boost::optional<ExternalInterfaceFunctionalMockupUnitExportToSchedule>;

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToSchedule*/
  using ExternalInterfaceFunctionalMockupUnitExportToScheduleVector = std::vector<ExternalInterfaceFunctionalMockupUnitExportToSchedule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_HPP
