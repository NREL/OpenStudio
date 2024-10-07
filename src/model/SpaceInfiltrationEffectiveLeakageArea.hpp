/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_HPP
#define MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class SpaceInfiltrationEffectiveLeakageArea_Impl;

  }  // namespace detail

  /** SpaceInfiltrationEffectiveLeakageArea is a SpaceLoad that wraps the OpenStudio
 *  IDD object 'OS:SpaceInfiltration:EffectiveLeakageArea'. */
  class MODEL_API SpaceInfiltrationEffectiveLeakageArea : public SpaceLoad
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SpaceInfiltrationEffectiveLeakageArea(const Model& model);

    virtual ~SpaceInfiltrationEffectiveLeakageArea() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceInfiltrationEffectiveLeakageArea(const SpaceInfiltrationEffectiveLeakageArea& other) = default;
    SpaceInfiltrationEffectiveLeakageArea(SpaceInfiltrationEffectiveLeakageArea&& other) = default;
    SpaceInfiltrationEffectiveLeakageArea& operator=(const SpaceInfiltrationEffectiveLeakageArea&) = default;
    SpaceInfiltrationEffectiveLeakageArea& operator=(SpaceInfiltrationEffectiveLeakageArea&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the (fractional) infiltration schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    double effectiveAirLeakageArea() const;

    double stackCoefficient() const;

    double windCoefficient() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setEffectiveAirLeakageArea(double effectiveAirLeakageArea);

    bool setStackCoefficient(double stackCoefficient);

    bool setWindCoefficient(double windCoefficient);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SpaceInfiltrationEffectiveLeakageArea_Impl;

    explicit SpaceInfiltrationEffectiveLeakageArea(std::shared_ptr<detail::SpaceInfiltrationEffectiveLeakageArea_Impl> impl);

    friend class Model;
    friend class IdfObject;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationEffectiveLeakageArea");
  };

  /** \relates SpaceInfiltrationEffectiveLeakageArea*/
  using OptionalSpaceInfiltrationEffectiveLeakageArea = boost::optional<SpaceInfiltrationEffectiveLeakageArea>;

  /** \relates SpaceInfiltrationEffectiveLeakageArea*/
  using SpaceInfiltrationEffectiveLeakageAreaVector = std::vector<SpaceInfiltrationEffectiveLeakageArea>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_HPP
