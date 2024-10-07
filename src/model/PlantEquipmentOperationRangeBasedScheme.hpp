/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationScheme.hpp"

namespace openstudio {
namespace model {

  class HVACComponent;

  namespace detail {
    class PlantEquipmentOperationRangeBasedScheme_Impl;
  }

  /** PlantEquipmentOperationRangeBasedScheme instances
 * will be created with a single continuous load range between a minimum and maximum bound.
 * This continuum can be subdivided to create multiple discrete ranges with different equipment assignments.
 * Ranges are identified by the value of their upper limit. A load range is allowed to be empty.
 * This interface is similar to ScheduleDay in OpenStudio, where an entire continuum must be filled in.
 * Here some ranges in the continuum maybe be empty (no equipment), signaling that the no equipment operates during
 * the specified range. Subclasses will determine appropriate minimum and maximum bounds of the operation scheme.
 */
  class MODEL_API PlantEquipmentOperationRangeBasedScheme : public PlantEquipmentOperationScheme
  {

   public:
    PlantEquipmentOperationRangeBasedScheme(IddObjectType type, const Model& model);

    virtual ~PlantEquipmentOperationRangeBasedScheme() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationRangeBasedScheme(const PlantEquipmentOperationRangeBasedScheme& other) = default;
    PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationRangeBasedScheme&& other) = default;
    PlantEquipmentOperationRangeBasedScheme& operator=(const PlantEquipmentOperationRangeBasedScheme&) = default;
    PlantEquipmentOperationRangeBasedScheme& operator=(PlantEquipmentOperationRangeBasedScheme&&) = default;

    friend class openstudio::IdfObject;

    /** The maximum limit of the range based operation scheme. No load range can operate above this value. */
    double maximumUpperLimit() const;

    /** The minimum limit of the range based operation scheme. No load range can operate below this value.
    * This is where the first load range will begin from. A default constructed PlantEquipmentOperationRangeBasedScheme will
    * have a single range defined between the minimumLowerLimit and the maximumUpperLimit. */
    double minimumLowerLimit() const;

    /** Add a new load range, using the adjacent range as the lower limit, and "upperLimit" as the upper limit of the new range.
    * Returns true if the load range was succesfully added. Duplicates wil be removed from the equipment vector **/
    bool addLoadRange(double upperLimit, const std::vector<HVACComponent>& equipment);

    /** Remove an existing load range identified by its upper limit.
    * Every operation scheme in OpenStudio must span the entire continuum between 0 and a predefined upper limit,
    * therefore it is not possible to remove the largest upper limit. Return the hvac equipment that was on the range.
    * The next "higher" load range fills in the place in the load continum that the removed range occupied. */
    std::vector<HVACComponent> removeLoadRange(double upperLimit);

    /** Return a vector of all of the load range upper limits.
    * A default constructed load range scheme, will return a vector with one element. */
    std::vector<double> loadRangeUpperLimits() const;

    /** Return a vector of the equipment at a given load range, where the load range is identified by the upper limit. */
    std::vector<HVACComponent> equipment(double upperLimit) const;

    /** Add an HVACComponent instance to an existing load range. */
    bool addEquipment(double upperLimit, const HVACComponent& equipment);

    /** Replace HVACComponent instances assigned to an existing load range.
   *  This is useful for reording equipment.
   *  */
    bool replaceEquipment(double upperLimit, const std::vector<HVACComponent>& equipment);

    /** Add an HVACComponent instance to the highest load range.
    * This method is useful to add equipment to the single default load range
    * after a load operation scheme is default constructed. */
    bool addEquipment(const HVACComponent& equipment);

    /** Replace HVACComponent instances assigned to the highest load range.
   *  This is useful for reording equipment.
   * */
    bool replaceEquipment(const std::vector<HVACComponent>& equipment);

    /** Remove an HVACComponent instance from an existing load range. */
    bool removeEquipment(double upperLimit, const HVACComponent& equipment);

    /** Remove an HVACComponent instance from all load ranges for which it is a member. */
    bool removeEquipment(const HVACComponent& equipment);

    /** Remove all load ranges except the default range. All equipment will be removed. */
    void clearLoadRanges();

   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationRangeBasedScheme_Impl;

    explicit PlantEquipmentOperationRangeBasedScheme(std::shared_ptr<detail::PlantEquipmentOperationRangeBasedScheme_Impl> impl);

    friend class detail::PlantEquipmentOperationRangeBasedScheme_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationRangeBasedScheme");
  };

  using OptionalPlantEquipmentOperationRangeBasedScheme = boost::optional<PlantEquipmentOperationRangeBasedScheme>;

  using PlantEquipmentOperationRangeBasedSchemeVector = std::vector<PlantEquipmentOperationRangeBasedScheme>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_HPP
