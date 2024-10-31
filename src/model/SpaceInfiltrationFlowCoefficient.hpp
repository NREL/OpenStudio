/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_HPP
#define MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class SpaceInfiltrationFlowCoefficient_Impl;

  }  // namespace detail

  /** SpaceInfiltrationFlowCoefficient is a SpaceLoad that wraps the OpenStudio IDD object 'OS:SpaceInfiltration:FlowCoefficient'. */
  class MODEL_API SpaceInfiltrationFlowCoefficient : public SpaceLoad
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SpaceInfiltrationFlowCoefficient(const Model& model);

    virtual ~SpaceInfiltrationFlowCoefficient() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceInfiltrationFlowCoefficient(const SpaceInfiltrationFlowCoefficient& other) = default;
    SpaceInfiltrationFlowCoefficient(SpaceInfiltrationFlowCoefficient&& other) = default;
    SpaceInfiltrationFlowCoefficient& operator=(const SpaceInfiltrationFlowCoefficient&) = default;
    SpaceInfiltrationFlowCoefficient& operator=(SpaceInfiltrationFlowCoefficient&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the (fractional) infiltration schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    double flowCoefficient() const;

    double stackCoefficient() const;

    double pressureExponent() const;

    double windCoefficient() const;

    double shelterFactor() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setFlowCoefficient(double flowCoefficient);

    bool setStackCoefficient(double stackCoefficient);

    bool setPressureExponent(double pressureExponent);

    bool setWindCoefficient(double windCoefficient);

    bool setShelterFactor(double shelterFactor);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SpaceInfiltrationFlowCoefficient_Impl;

    explicit SpaceInfiltrationFlowCoefficient(std::shared_ptr<detail::SpaceInfiltrationFlowCoefficient_Impl> impl);

    friend class Model;
    friend class IdfObject;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationFlowCoefficient");
  };

  /** \relates SpaceInfiltrationFlowCoefficient*/
  using OptionalSpaceInfiltrationFlowCoefficient = boost::optional<SpaceInfiltrationFlowCoefficient>;

  /** \relates SpaceInfiltrationFlowCoefficient*/
  using SpaceInfiltrationFlowCoefficientVector = std::vector<SpaceInfiltrationFlowCoefficient>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_HPP
