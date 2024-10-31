/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORFUELEQUIPMENT_HPP
#define MODEL_EXTERIORFUELEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance.hpp"

namespace openstudio {

class FuelType;

namespace model {

  class ExteriorFuelEquipmentDefinition;
  class Facility;
  class Schedule;

  namespace detail {

    class ExteriorFuelEquipment_Impl;

  }  // namespace detail

  /** ExteriorFuelEquipment is a ExteriorLoadInstance that wraps the OpenStudio IDD object
 *  'OS:Exterior:FuelEquipment'. ExteriorFuelEquipment is a child of Facility and instances
 *  ExteriorFuelEquipmentDefinition. */
  class MODEL_API ExteriorFuelEquipment : public ExteriorLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** This constructor sets schedule to always on, defaults to Fuel Type = Electricity */
    ExteriorFuelEquipment(const ExteriorFuelEquipmentDefinition& definition);

    /** This constructor requires a user-specified schedule. */
    ExteriorFuelEquipment(const ExteriorFuelEquipmentDefinition& definition, Schedule& schedule);

    virtual ~ExteriorFuelEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorFuelEquipment(const ExteriorFuelEquipment& other) = default;
    ExteriorFuelEquipment(ExteriorFuelEquipment&& other) = default;
    ExteriorFuelEquipment& operator=(const ExteriorFuelEquipment&) = default;
    ExteriorFuelEquipment& operator=(ExteriorFuelEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    /** \deprecated */
    static std::vector<std::string> validFuelTypeValues();

    /** @name Getters */
    //@{

    ExteriorFuelEquipmentDefinition exteriorFuelEquipmentDefinition() const;

    Schedule schedule() const;

    std::string fuelType() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExteriorFuelEquipmentDefinition(const ExteriorFuelEquipmentDefinition& exteriorFuelEquipmentDefinition);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    bool setFuelType(const FuelType& fuelType);
    bool setFuelType(const std::string& fuelType);

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
    using ImplType = detail::ExteriorFuelEquipment_Impl;

    explicit ExteriorFuelEquipment(std::shared_ptr<detail::ExteriorFuelEquipment_Impl> impl);

    friend class detail::ExteriorFuelEquipment_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorFuelEquipment");
  };

  /** \relates ExteriorFuelEquipment*/
  using OptionalExteriorFuelEquipment = boost::optional<ExteriorFuelEquipment>;

  /** \relates ExteriorFuelEquipment*/
  using ExteriorFuelEquipmentVector = std::vector<ExteriorFuelEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORFUELEQUIPMENT_HPP
