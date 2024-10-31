/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONTROLLERMECHANICALVENTILATION_HPP
#define MODEL_CONTROLLERMECHANICALVENTILATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ControllerOutdoorAir;

  namespace detail {

    class ControllerMechanicalVentilation_Impl;

  }  // namespace detail

  /** ControllerMechanicalVentilation is a ModelObject that wraps the OpenStudio IDD object 'OS:Controller:MechanicalVentilation'. */
  class MODEL_API ControllerMechanicalVentilation : public ModelObject
  {
   public:
    explicit ControllerMechanicalVentilation(const Model& model);

    virtual ~ControllerMechanicalVentilation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ControllerMechanicalVentilation(const ControllerMechanicalVentilation& other) = default;
    ControllerMechanicalVentilation(ControllerMechanicalVentilation&& other) = default;
    ControllerMechanicalVentilation& operator=(const ControllerMechanicalVentilation&) = default;
    ControllerMechanicalVentilation& operator=(ControllerMechanicalVentilation&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> systemOutdoorAirMethodValues();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    bool demandControlledVentilation() const;

    bool isDemandControlledVentilationDefaulted() const;

    bool setDemandControlledVentilation(bool demandControlledVentilation);

    void setDemandControlledVentilationNoFail(bool demandControlledVentilation);

    void resetDemandControlledVentilation();

    std::string systemOutdoorAirMethod() const;

    bool isSystemOutdoorAirMethodDefaulted() const;

    bool setSystemOutdoorAirMethod(const std::string& systemOutdoorAirMethod);

    void resetSystemOutdoorAirMethod();

    ControllerOutdoorAir controllerOutdoorAir() const;

   protected:
    /// @cond
    using ImplType = detail::ControllerMechanicalVentilation_Impl;

    explicit ControllerMechanicalVentilation(std::shared_ptr<detail::ControllerMechanicalVentilation_Impl> impl);

    friend class detail::ControllerMechanicalVentilation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ControllerMechanicalVentilation");
  };

  /** \relates ControllerMechanicalVentilation*/
  using OptionalControllerMechanicalVentilation = boost::optional<ControllerMechanicalVentilation>;

  /** \relates ControllerMechanicalVentilation*/
  using ControllerMechanicalVentilationVector = std::vector<ControllerMechanicalVentilation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONTROLLERMECHANICALVENTILATION_HPP
