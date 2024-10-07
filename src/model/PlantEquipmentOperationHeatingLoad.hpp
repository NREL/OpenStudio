/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PlantEquipmentOperationHeatingLoad_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationHeatingLoad is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:HeatingLoad'. */
  class MODEL_API PlantEquipmentOperationHeatingLoad : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationHeatingLoad(const Model& model);

    virtual ~PlantEquipmentOperationHeatingLoad() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationHeatingLoad(const PlantEquipmentOperationHeatingLoad& other) = default;
    PlantEquipmentOperationHeatingLoad(PlantEquipmentOperationHeatingLoad&& other) = default;
    PlantEquipmentOperationHeatingLoad& operator=(const PlantEquipmentOperationHeatingLoad&) = default;
    PlantEquipmentOperationHeatingLoad& operator=(PlantEquipmentOperationHeatingLoad&&) = default;

    //@}

    static IddObjectType iddObjectType();

   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationHeatingLoad_Impl;

    explicit PlantEquipmentOperationHeatingLoad(std::shared_ptr<detail::PlantEquipmentOperationHeatingLoad_Impl> impl);

    friend class detail::PlantEquipmentOperationHeatingLoad_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationHeatingLoad");
  };

  /** \relates PlantEquipmentOperationHeatingLoad*/
  using OptionalPlantEquipmentOperationHeatingLoad = boost::optional<PlantEquipmentOperationHeatingLoad>;

  /** \relates PlantEquipmentOperationHeatingLoad*/
  using PlantEquipmentOperationHeatingLoadVector = std::vector<PlantEquipmentOperationHeatingLoad>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_HPP
