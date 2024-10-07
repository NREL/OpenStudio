/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINT_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINT_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PlantEquipmentOperationOutdoorDewpoint_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorDewpoint is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDewpoint'. */
  class MODEL_API PlantEquipmentOperationOutdoorDewpoint : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorDewpoint(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorDewpoint() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorDewpoint(const PlantEquipmentOperationOutdoorDewpoint& other) = default;
    PlantEquipmentOperationOutdoorDewpoint(PlantEquipmentOperationOutdoorDewpoint&& other) = default;
    PlantEquipmentOperationOutdoorDewpoint& operator=(const PlantEquipmentOperationOutdoorDewpoint&) = default;
    PlantEquipmentOperationOutdoorDewpoint& operator=(PlantEquipmentOperationOutdoorDewpoint&&) = default;

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
    using ImplType = detail::PlantEquipmentOperationOutdoorDewpoint_Impl;

    explicit PlantEquipmentOperationOutdoorDewpoint(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpoint_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorDewpoint_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDewpoint");
  };

  /** \relates PlantEquipmentOperationOutdoorDewpoint*/
  using OptionalPlantEquipmentOperationOutdoorDewpoint = boost::optional<PlantEquipmentOperationOutdoorDewpoint>;

  /** \relates PlantEquipmentOperationOutdoorDewpoint*/
  using PlantEquipmentOperationOutdoorDewpointVector = std::vector<PlantEquipmentOperationOutdoorDewpoint>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINT_HPP
