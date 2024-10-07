/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OTHEREQUIPMENT_HPP
#define MODEL_OTHEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {

class FuelType;

namespace model {

  class OtherEquipmentDefinition;
  class Schedule;

  namespace detail {

    class OtherEquipment_Impl;

  }  // namespace detail

  /** OtherEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:OtherEquipment'. \sa OtherEquipmentDefinition */
  class MODEL_API OtherEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit OtherEquipment(const OtherEquipmentDefinition& definition);

    virtual ~OtherEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OtherEquipment(const OtherEquipment& other) = default;
    OtherEquipment(OtherEquipment&& other) = default;
    OtherEquipment& operator=(const OtherEquipment&) = default;
    OtherEquipment& operator=(OtherEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the End-Use Subcategory **/
    std::string endUseSubcategory() const;

    /** Returns true if the end-use subcategory is defaulted **/
    bool isEndUseSubcategoryDefaulted() const;

    /** Returns a list of valid fuel types. **/
    static std::vector<std::string> validFuelTypeValues();

    /** Gets the equipment fuel type **/
    std::string fuelType() const;

    /** Returns true if the fuel type is defaulted **/
    bool isFuelTypeDefaulted() const;

    OtherEquipmentDefinition otherEquipmentDefinition() const;

    /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the End-Use Subcategory **/
    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    /** Resets the End-Use Subcategory **/
    void resetEndUseSubcategory();

    /** Sets the fuel type. **/
    bool setFuelType(const FuelType& fuelType);

    bool setFuelType(const std::string& fuelType);

    /** Resets the fuel type to its default. **/
    void resetFuelType();

    bool setOtherEquipmentDefinition(const OtherEquipmentDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    //@}
    /** @name Other */
    //@{

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
    using ImplType = detail::OtherEquipment_Impl;

    friend class detail::OtherEquipment_Impl;
    friend class openstudio::detail::IdfObject_Impl;
    friend class IdfObject;
    friend class Model;

    explicit OtherEquipment(std::shared_ptr<detail::OtherEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OtherEquipment");
  };

  /** \relates OtherEquipment*/
  using OptionalOtherEquipment = boost::optional<OtherEquipment>;

  /** \relates OtherEquipment*/
  using OtherEquipmentVector = std::vector<OtherEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OTHEREQUIPMENT_HPP
