/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    class PlantEquipmentOperationOutdoorDewpointDifference_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorDewpointDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDewpointDifference'. */
  class MODEL_API PlantEquipmentOperationOutdoorDewpointDifference : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorDewpointDifference(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorDewpointDifference() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorDewpointDifference(const PlantEquipmentOperationOutdoorDewpointDifference& other) = default;
    PlantEquipmentOperationOutdoorDewpointDifference(PlantEquipmentOperationOutdoorDewpointDifference&& other) = default;
    PlantEquipmentOperationOutdoorDewpointDifference& operator=(const PlantEquipmentOperationOutdoorDewpointDifference&) = default;
    PlantEquipmentOperationOutdoorDewpointDifference& operator=(PlantEquipmentOperationOutdoorDewpointDifference&&) = default;

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
    using ImplType = detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl;

    explicit PlantEquipmentOperationOutdoorDewpointDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDewpointDifference");
  };

  /** \relates PlantEquipmentOperationOutdoorDewpointDifference*/
  using OptionalPlantEquipmentOperationOutdoorDewpointDifference = boost::optional<PlantEquipmentOperationOutdoorDewpointDifference>;

  /** \relates PlantEquipmentOperationOutdoorDewpointDifference*/
  using PlantEquipmentOperationOutdoorDewpointDifferenceVector = std::vector<PlantEquipmentOperationOutdoorDewpointDifference>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_HPP
