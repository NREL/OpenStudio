/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULB_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULB_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PlantEquipmentOperationOutdoorWetBulb_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorWetBulb is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorWetBulb'. */
  class MODEL_API PlantEquipmentOperationOutdoorWetBulb : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorWetBulb(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorWetBulb() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorWetBulb(const PlantEquipmentOperationOutdoorWetBulb& other) = default;
    PlantEquipmentOperationOutdoorWetBulb(PlantEquipmentOperationOutdoorWetBulb&& other) = default;
    PlantEquipmentOperationOutdoorWetBulb& operator=(const PlantEquipmentOperationOutdoorWetBulb&) = default;
    PlantEquipmentOperationOutdoorWetBulb& operator=(PlantEquipmentOperationOutdoorWetBulb&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationOutdoorWetBulb_Impl;

    explicit PlantEquipmentOperationOutdoorWetBulb(std::shared_ptr<detail::PlantEquipmentOperationOutdoorWetBulb_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorWetBulb_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorWetBulb");
  };

  /** \relates PlantEquipmentOperationOutdoorWetBulb*/
  using OptionalPlantEquipmentOperationOutdoorWetBulb = boost::optional<PlantEquipmentOperationOutdoorWetBulb>;

  /** \relates PlantEquipmentOperationOutdoorWetBulb*/
  using PlantEquipmentOperationOutdoorWetBulbVector = std::vector<PlantEquipmentOperationOutdoorWetBulb>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULB_HPP
