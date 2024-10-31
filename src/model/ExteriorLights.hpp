/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLIGHTS_HPP
#define MODEL_EXTERIORLIGHTS_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance.hpp"

namespace openstudio {
namespace model {

  class ExteriorLightsDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    class ExteriorLights_Impl;

  }  // namespace detail

  /** ExteriorLights is a ExteriorLoadInstance that wraps the OpenStudio IDD object
 *  'OS:Exterior:Lights'. ExteriorLights is a child of Facility and instances
 *  ExteriorLightsDefinition. */
  class MODEL_API ExteriorLights : public ExteriorLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** This constructor sets schedule to always on, and provides the option to use the
   *  astronomical clock control option. */
    ExteriorLights(const ExteriorLightsDefinition& definition, bool useControlOptionAstronomicalClock = false);

    /** This constructor requires a user-specified schedule. */
    ExteriorLights(const ExteriorLightsDefinition& definition, Schedule& schedule);

    virtual ~ExteriorLights() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorLights(const ExteriorLights& other) = default;
    ExteriorLights(ExteriorLights&& other) = default;
    ExteriorLights& operator=(const ExteriorLights&) = default;
    ExteriorLights& operator=(ExteriorLights&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlOptionValues();

    /** \deprecated */
    static std::vector<std::string> validControlOptionValues();

    /** @name Getters */
    //@{

    ExteriorLightsDefinition exteriorLightsDefinition() const;

    boost::optional<Schedule> schedule() const;

    std::string controlOption() const;

    bool isControlOptionDefaulted() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    bool setControlOption(const std::string& controlOption);

    void resetControlOption();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    Facility facility() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExteriorLights_Impl;

    explicit ExteriorLights(std::shared_ptr<detail::ExteriorLights_Impl> impl);

    friend class detail::ExteriorLights_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLights");
  };

  /** \relates ExteriorLights*/
  using OptionalExteriorLights = boost::optional<ExteriorLights>;

  /** \relates ExteriorLights*/
  using ExteriorLightsVector = std::vector<ExteriorLights>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLIGHTS_HPP
