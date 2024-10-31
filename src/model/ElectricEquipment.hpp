/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICEQUIPMENT_HPP
#define MODEL_ELECTRICEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ElectricEquipmentDefinition;

  namespace detail {

    class ElectricEquipment_Impl;

  }  // namespace detail

  /** ElectricEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:ElectricEquipment'. Its fields are derived from the EnergyPlus IDD object
 *  'ElectricEquipment'. \sa ElectricEquipmentDefinition */
  class MODEL_API ElectricEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricEquipment(const ElectricEquipmentDefinition& electricEquipmentDefinition);

    virtual ~ElectricEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricEquipment(const ElectricEquipment& other) = default;
    ElectricEquipment(ElectricEquipment&& other) = default;
    ElectricEquipment& operator=(const ElectricEquipment&) = default;
    ElectricEquipment& operator=(ElectricEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the ElectricEquipmentDefinition object. */
    ElectricEquipmentDefinition electricEquipmentDefinition() const;

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

    /** Sets the ElectricEquipmentDefinition object. */
    bool setElectricEquipmentDefinition(const ElectricEquipmentDefinition& definition);

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
   *  electricEquipmentDefinition(). */
    boost::optional<double> designLevel() const;

    /** Returns the powerPerFloorArea if possible based on the underlying data of
   *  electricEquipmentDefinition(). */
    boost::optional<double> powerPerFloorArea() const;

    /** Returns the powerPerPerson if possible based on the underlying data of
   *  electricEquipmentDefinition(). */
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
    using ImplType = detail::ElectricEquipment_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ElectricEquipment(std::shared_ptr<detail::ElectricEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricEquipment");
  };

  /** \relates ElectricEquipment*/
  using OptionalElectricEquipment = boost::optional<ElectricEquipment>;

  /** \relates ElectricEquipment*/
  using ElectricEquipmentVector = std::vector<ElectricEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENT_HPP
