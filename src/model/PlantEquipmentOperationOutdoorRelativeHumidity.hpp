/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PlantEquipmentOperationOutdoorRelativeHumidity_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorRelativeHumidity is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorRelativeHumidity'. */
  class MODEL_API PlantEquipmentOperationOutdoorRelativeHumidity : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorRelativeHumidity(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorRelativeHumidity() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorRelativeHumidity(const PlantEquipmentOperationOutdoorRelativeHumidity& other) = default;
    PlantEquipmentOperationOutdoorRelativeHumidity(PlantEquipmentOperationOutdoorRelativeHumidity&& other) = default;
    PlantEquipmentOperationOutdoorRelativeHumidity& operator=(const PlantEquipmentOperationOutdoorRelativeHumidity&) = default;
    PlantEquipmentOperationOutdoorRelativeHumidity& operator=(PlantEquipmentOperationOutdoorRelativeHumidity&&) = default;

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
    using ImplType = detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl;

    explicit PlantEquipmentOperationOutdoorRelativeHumidity(std::shared_ptr<detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorRelativeHumidity");
  };

  /** \relates PlantEquipmentOperationOutdoorRelativeHumidity*/
  using OptionalPlantEquipmentOperationOutdoorRelativeHumidity = boost::optional<PlantEquipmentOperationOutdoorRelativeHumidity>;

  /** \relates PlantEquipmentOperationOutdoorRelativeHumidity*/
  using PlantEquipmentOperationOutdoorRelativeHumidityVector = std::vector<PlantEquipmentOperationOutdoorRelativeHumidity>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP
