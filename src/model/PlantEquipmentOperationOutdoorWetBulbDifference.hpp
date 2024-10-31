/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULBDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULBDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    class PlantEquipmentOperationOutdoorWetBulbDifference_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorWetBulbDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorWetBulbDifference'. */
  class MODEL_API PlantEquipmentOperationOutdoorWetBulbDifference : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorWetBulbDifference(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorWetBulbDifference() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorWetBulbDifference(const PlantEquipmentOperationOutdoorWetBulbDifference& other) = default;
    PlantEquipmentOperationOutdoorWetBulbDifference(PlantEquipmentOperationOutdoorWetBulbDifference&& other) = default;
    PlantEquipmentOperationOutdoorWetBulbDifference& operator=(const PlantEquipmentOperationOutdoorWetBulbDifference&) = default;
    PlantEquipmentOperationOutdoorWetBulbDifference& operator=(PlantEquipmentOperationOutdoorWetBulbDifference&&) = default;

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
    using ImplType = detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl;

    explicit PlantEquipmentOperationOutdoorWetBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorWetBulbDifference");
  };

  /** \relates PlantEquipmentOperationOutdoorWetBulbDifference*/
  using OptionalPlantEquipmentOperationOutdoorWetBulbDifference = boost::optional<PlantEquipmentOperationOutdoorWetBulbDifference>;

  /** \relates PlantEquipmentOperationOutdoorWetBulbDifference*/
  using PlantEquipmentOperationOutdoorWetBulbDifferenceVector = std::vector<PlantEquipmentOperationOutdoorWetBulbDifference>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORWETBULBDIFFERENCE_HPP
