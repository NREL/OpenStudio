/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTINGSIMULATIONCONTROL_HPP
#define MODEL_LIGHTINGSIMULATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class LightingSimulationControl_Impl;

  }  // namespace detail

  /** LightingSimulationControl is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingSimulationControl'. */
  class MODEL_API LightingSimulationControl : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~LightingSimulationControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LightingSimulationControl(const LightingSimulationControl& other) = default;
    LightingSimulationControl(LightingSimulationControl&& other) = default;
    LightingSimulationControl& operator=(const LightingSimulationControl&) = default;
    LightingSimulationControl& operator=(LightingSimulationControl&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    bool runSimulationforDesignDays() const;

    bool isRunSimulationforDesignDaysDefaulted() const;

    bool runSimulationforWeatherFileRunPeriods() const;

    bool isRunSimulationforWeatherFileRunPeriodsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRunSimulationforDesignDays(bool runSimulationforDesignDays);

    void resetRunSimulationforDesignDays();

    bool setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

    void resetRunSimulationforWeatherFileRunPeriods();

    //@}
   protected:
    /// @cond
    using ImplType = detail::LightingSimulationControl_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::LightingSimulationControl_Impl;

    explicit LightingSimulationControl(std::shared_ptr<detail::LightingSimulationControl_Impl> impl);

    explicit LightingSimulationControl(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LightingSimulationControl");
  };

  /** \relates LightingSimulationControl*/
  using OptionalLightingSimulationControl = boost::optional<LightingSimulationControl>;

  /** \relates LightingSimulationControl*/
  using LightingSimulationControlVector = std::vector<LightingSimulationControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGSIMULATIONCONTROL_HPP
