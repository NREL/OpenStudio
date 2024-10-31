/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONCOOLINGLOAD_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONCOOLINGLOAD_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PlantEquipmentOperationCoolingLoad_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationCoolingLoad is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:CoolingLoad'. */
  class MODEL_API PlantEquipmentOperationCoolingLoad : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationCoolingLoad(const Model& model);

    virtual ~PlantEquipmentOperationCoolingLoad() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationCoolingLoad(const PlantEquipmentOperationCoolingLoad& other) = default;
    PlantEquipmentOperationCoolingLoad(PlantEquipmentOperationCoolingLoad&& other) = default;
    PlantEquipmentOperationCoolingLoad& operator=(const PlantEquipmentOperationCoolingLoad&) = default;
    PlantEquipmentOperationCoolingLoad& operator=(PlantEquipmentOperationCoolingLoad&&) = default;

    //@}

    static IddObjectType iddObjectType();

   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationCoolingLoad_Impl;

    explicit PlantEquipmentOperationCoolingLoad(std::shared_ptr<detail::PlantEquipmentOperationCoolingLoad_Impl> impl);

    friend class detail::PlantEquipmentOperationCoolingLoad_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationCoolingLoad");
  };

  /** \relates PlantEquipmentOperationCoolingLoad*/
  using OptionalPlantEquipmentOperationCoolingLoad = boost::optional<PlantEquipmentOperationCoolingLoad>;

  /** \relates PlantEquipmentOperationCoolingLoad*/
  using PlantEquipmentOperationCoolingLoadVector = std::vector<PlantEquipmentOperationCoolingLoad>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONCOOLINGLOAD_HPP
