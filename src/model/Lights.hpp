/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTS_HPP
#define MODEL_LIGHTS_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class LightsDefinition;

  namespace detail {

    class Lights_Impl;

  }  // namespace detail

  /** Lights is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS:Lights'.
 *  \sa LightsDefinition */
  class MODEL_API Lights : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Lights(const LightsDefinition& lightsDefinition);

    virtual ~Lights() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Lights(const Lights& other) = default;
    Lights(Lights&& other) = default;
    Lights& operator=(const Lights&) = default;
    Lights& operator=(Lights&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the LightsDefinition object. */
    LightsDefinition lightsDefinition() const;

    /** Returns the (fractional) lighting schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    double fractionReplaceable() const;

    bool isFractionReplaceableDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the LightsDefinition object. */
    bool setLightsDefinition(const LightsDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setFractionReplaceable(double fractionReplaceable);

    void resetFractionReplaceable();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    /** Returns the lightingLevel if possible based on the underlying data of
   *  lightsDefinition(). */
    boost::optional<double> lightingLevel() const;

    /** Returns the powerPerFloorArea if possible based on the underlying data of
   *  lightsDefinition(). */
    boost::optional<double> powerPerFloorArea() const;

    /** Returns the powerPerPerson if possible based on the underlying data of
   *  lightsDefinition(). */
    boost::optional<double> powerPerPerson() const;

    /** Returns the lighting level represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getLightingPower(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::Lights_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit Lights(std::shared_ptr<detail::Lights_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Lights");
  };

  /** \relates Lights*/
  using OptionalLights = boost::optional<Lights>;

  /** \relates Lights*/
  using LightsVector = std::vector<Lights>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTS_HPP
