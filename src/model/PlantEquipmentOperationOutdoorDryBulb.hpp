/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PlantEquipmentOperationOutdoorDryBulb_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorDryBulb is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDryBulb'. */
  class MODEL_API PlantEquipmentOperationOutdoorDryBulb : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorDryBulb(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorDryBulb() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorDryBulb(const PlantEquipmentOperationOutdoorDryBulb& other) = default;
    PlantEquipmentOperationOutdoorDryBulb(PlantEquipmentOperationOutdoorDryBulb&& other) = default;
    PlantEquipmentOperationOutdoorDryBulb& operator=(const PlantEquipmentOperationOutdoorDryBulb&) = default;
    PlantEquipmentOperationOutdoorDryBulb& operator=(PlantEquipmentOperationOutdoorDryBulb&&) = default;

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
    using ImplType = detail::PlantEquipmentOperationOutdoorDryBulb_Impl;

    explicit PlantEquipmentOperationOutdoorDryBulb(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulb_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorDryBulb_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDryBulb");
  };

  /** \relates PlantEquipmentOperationOutdoorDryBulb*/
  using OptionalPlantEquipmentOperationOutdoorDryBulb = boost::optional<PlantEquipmentOperationOutdoorDryBulb>;

  /** \relates PlantEquipmentOperationOutdoorDryBulb*/
  using PlantEquipmentOperationOutdoorDryBulbVector = std::vector<PlantEquipmentOperationOutdoorDryBulb>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_HPP
