/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    class PlantEquipmentOperationOutdoorDryBulbDifference_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorDryBulbDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDryBulbDifference'. */
  class MODEL_API PlantEquipmentOperationOutdoorDryBulbDifference : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorDryBulbDifference(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorDryBulbDifference() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorDryBulbDifference(const PlantEquipmentOperationOutdoorDryBulbDifference& other) = default;
    PlantEquipmentOperationOutdoorDryBulbDifference(PlantEquipmentOperationOutdoorDryBulbDifference&& other) = default;
    PlantEquipmentOperationOutdoorDryBulbDifference& operator=(const PlantEquipmentOperationOutdoorDryBulbDifference&) = default;
    PlantEquipmentOperationOutdoorDryBulbDifference& operator=(PlantEquipmentOperationOutdoorDryBulbDifference&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> referenceTemperatureNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceTemperatureNode(const Node& node);

    void resetReferenceTemperatureNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl;

    explicit PlantEquipmentOperationOutdoorDryBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDryBulbDifference");
  };

  /** \relates PlantEquipmentOperationOutdoorDryBulbDifference*/
  using OptionalPlantEquipmentOperationOutdoorDryBulbDifference = boost::optional<PlantEquipmentOperationOutdoorDryBulbDifference>;

  /** \relates PlantEquipmentOperationOutdoorDryBulbDifference*/
  using PlantEquipmentOperationOutdoorDryBulbDifferenceVector = std::vector<PlantEquipmentOperationOutdoorDryBulbDifference>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP
