/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACESCHEDULE_HPP
#define MODEL_EXTERNALINTERFACESCHEDULE_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceSchedule_Impl;

  }  // namespace detail

  /** ExternalInterfaceSchedule is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:Schedule'. */
  class MODEL_API ExternalInterfaceSchedule : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceSchedule(const Model& model, double initialValue);

    explicit ExternalInterfaceSchedule(const Model& model);

    virtual ~ExternalInterfaceSchedule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceSchedule(const ExternalInterfaceSchedule& other) = default;
    ExternalInterfaceSchedule(ExternalInterfaceSchedule&& other) = default;
    ExternalInterfaceSchedule& operator=(const ExternalInterfaceSchedule&) = default;
    ExternalInterfaceSchedule& operator=(ExternalInterfaceSchedule&&) = default;

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
    using ImplType = detail::ExternalInterfaceSchedule_Impl;

    explicit ExternalInterfaceSchedule(std::shared_ptr<detail::ExternalInterfaceSchedule_Impl> impl);

    friend class detail::ExternalInterfaceSchedule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceSchedule");
  };

  /** \relates ExternalInterfaceSchedule*/
  using OptionalExternalInterfaceSchedule = boost::optional<ExternalInterfaceSchedule>;

  /** \relates ExternalInterfaceSchedule*/
  using ExternalInterfaceScheduleVector = std::vector<ExternalInterfaceSchedule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACESCHEDULE_HPP
