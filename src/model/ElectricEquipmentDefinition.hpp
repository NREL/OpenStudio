/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICEQUIPMENTDEFINITION_HPP
#define MODEL_ELECTRICEQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ElectricEquipmentDefinition_Impl;

  }  // namespace detail

  /** ElectricEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object
 *  'OS_ElectricEquipment_Definition'. Its fields are derived from the EnergyPlus IDD object
 *  'ElectricEquipment'. \sa ElectricEquipment
 */
  class MODEL_API ElectricEquipmentDefinition : public SpaceLoadDefinition
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricEquipmentDefinition(const Model& model);

    virtual ~ElectricEquipmentDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricEquipmentDefinition(const ElectricEquipmentDefinition& other) = default;
    ElectricEquipmentDefinition(ElectricEquipmentDefinition&& other) = default;
    ElectricEquipmentDefinition& operator=(const ElectricEquipmentDefinition&) = default;
    ElectricEquipmentDefinition& operator=(ElectricEquipmentDefinition&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDesignLevelCalculationMethodValues();

    //@}
    /** @name Getters */
    //@{

    std::string designLevelCalculationMethod() const;
    boost::optional<double> designLevel() const;
    boost::optional<double> wattsperSpaceFloorArea() const;
    boost::optional<double> wattsperPerson() const;

    double fractionLatent() const;
    bool isFractionLatentDefaulted() const;

    double fractionRadiant() const;
    bool isFractionRadiantDefaulted() const;

    double fractionLost() const;
    bool isFractionLostDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Also sets designLevelCalculationMethod accordingly. */
    bool setDesignLevel(double designLevel);

    /** Also sets designLevelCalculationMethod accordingly. */
    bool setWattsperSpaceFloorArea(double wattsperSpaceFloorArea);

    /** Also sets designLevelCalculationMethod accordingly. */
    bool setWattsperPerson(double wattsperPerson);

    bool setFractionLatent(double fractionLatent);
    void resetFractionLatent();

    bool setFractionRadiant(double fractionRadiant);
    void resetFractionRadiant();

    bool setFractionLost(double fractionLost);
    void resetFractionLost();

    //@}
    /** @name Other */
    //@{

    /** Returns the design level represented by this definition, assuming floorArea (m^2) and
   *  numPeople. */
    double getDesignLevel(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    /** If method is a \link validDesignLevelCalculationMethodValues valid design level
   *  calculation method \endlink, changes this definition to an equivalent power level, under
   *  the assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople);

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricEquipmentDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ElectricEquipmentDefinition(std::shared_ptr<detail::ElectricEquipmentDefinition_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricEquipmentDefinition");
  };

  /** \relates ElectricEquipmentDefinition*/
  using OptionalElectricEquipmentDefinition = boost::optional<ElectricEquipmentDefinition>;

  /** \relates ElectricEquipmentDefinition*/
  using ElectricEquipmentDefinitionVector = std::vector<ElectricEquipmentDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTDEFINITION_HPP
