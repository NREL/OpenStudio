/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORWATEREQUIPMENT_HPP
#define MODEL_EXTERIORWATEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance.hpp"

namespace openstudio {
namespace model {

  class ExteriorWaterEquipmentDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    class ExteriorWaterEquipment_Impl;

  }  // namespace detail

  /** ExteriorWaterEquipment is a ExteriorLoadInstance that wraps the OpenStudio IDD object
 *  'OS:Exterior:WaterEquipment'. ExteriorWaterEquipment is a child of Facility and instances
 *  ExteriorWaterEquipmentDefinition. */
  class MODEL_API ExteriorWaterEquipment : public ExteriorLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** This constructor sets schedule to always on */
    ExteriorWaterEquipment(const ExteriorWaterEquipmentDefinition& definition);

    /** This constructor requires a user-specified schedule. */
    ExteriorWaterEquipment(const ExteriorWaterEquipmentDefinition& definition, Schedule& schedule);

    virtual ~ExteriorWaterEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorWaterEquipment(const ExteriorWaterEquipment& other) = default;
    ExteriorWaterEquipment(ExteriorWaterEquipment&& other) = default;
    ExteriorWaterEquipment& operator=(const ExteriorWaterEquipment&) = default;
    ExteriorWaterEquipment& operator=(ExteriorWaterEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ExteriorWaterEquipmentDefinition exteriorWaterEquipmentDefinition() const;

    Schedule schedule() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExteriorWaterEquipmentDefinition(const ExteriorWaterEquipmentDefinition& exteriorWaterEquipmentDefinition);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

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
    using ImplType = detail::ExteriorWaterEquipment_Impl;

    explicit ExteriorWaterEquipment(std::shared_ptr<detail::ExteriorWaterEquipment_Impl> impl);

    friend class detail::ExteriorWaterEquipment_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorWaterEquipment");
  };

  /** \relates ExteriorWaterEquipment*/
  using OptionalExteriorWaterEquipment = boost::optional<ExteriorWaterEquipment>;

  /** \relates ExteriorWaterEquipment*/
  using ExteriorWaterEquipmentVector = std::vector<ExteriorWaterEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORWATEREQUIPMENT_HPP
