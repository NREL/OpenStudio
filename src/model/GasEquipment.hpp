/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GASEQUIPMENT_HPP
#define MODEL_GASEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class GasEquipmentDefinition;

  namespace detail {

    class GasEquipment_Impl;

  }  // namespace detail

  /** GasEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:GasEquipment'. \sa GasEquipmentDefinition */
  class MODEL_API GasEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GasEquipment(const GasEquipmentDefinition& gasEquipmentDefinition);

    virtual ~GasEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GasEquipment(const GasEquipment& other) = default;
    GasEquipment(GasEquipment&& other) = default;
    GasEquipment& operator=(const GasEquipment&) = default;
    GasEquipment& operator=(GasEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the GasEquipmentDefinition object. */
    GasEquipmentDefinition gasEquipmentDefinition() const;

    /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the GasEquipmentDefinition object. */
    bool setGasEquipmentDefinition(const GasEquipmentDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    /** Returns the designLevel if possible based on the underlying data of
   *  gasEquipmentDefinition(). */
    boost::optional<double> designLevel() const;

    /** Returns the powerPerFloorArea if possible based on the underlying data of
   *  gasEquipmentDefinition(). */
    boost::optional<double> powerPerFloorArea() const;

    /** Returns the powerPerPerson if possible based on the underlying data of
   *  gasEquipmentDefinition(). */
    boost::optional<double> powerPerPerson() const;

    /** Returns the design level represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getDesignLevel(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::GasEquipment_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit GasEquipment(std::shared_ptr<detail::GasEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GasEquipment");
  };

  /** \relates GasEquipment*/
  using OptionalGasEquipment = boost::optional<GasEquipment>;

  /** \relates GasEquipment*/
  using GasEquipmentVector = std::vector<GasEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GASEQUIPMENT_HPP
