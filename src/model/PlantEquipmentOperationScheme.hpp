/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONSCHEME_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONSCHEME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class PlantLoop;

  namespace detail {
    class PlantEquipmentOperationScheme_Impl;
  }

  class MODEL_API PlantEquipmentOperationScheme : public ModelObject
  {

   public:
    PlantEquipmentOperationScheme(IddObjectType type, const Model& model);

    virtual ~PlantEquipmentOperationScheme() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationScheme(const PlantEquipmentOperationScheme& other) = default;
    PlantEquipmentOperationScheme(PlantEquipmentOperationScheme&& other) = default;
    PlantEquipmentOperationScheme& operator=(const PlantEquipmentOperationScheme&) = default;
    PlantEquipmentOperationScheme& operator=(PlantEquipmentOperationScheme&&) = default;

    boost::optional<PlantLoop> plantLoop() const;

   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationScheme_Impl;

    explicit PlantEquipmentOperationScheme(std::shared_ptr<detail::PlantEquipmentOperationScheme_Impl> impl);

    friend class detail::PlantEquipmentOperationScheme_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationScheme");
  };

  using OptionalPlantEquipmentOperationScheme = boost::optional<PlantEquipmentOperationScheme>;

  using PlantEquipmentOperationSchemeVector = std::vector<PlantEquipmentOperationScheme>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONSCHEME_HPP
