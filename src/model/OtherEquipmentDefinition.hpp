/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OTHEREQUIPMENTDEFINITION_HPP
#define MODEL_OTHEREQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OtherEquipmentDefinition_Impl;

  }  // namespace detail

  /** OtherEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object
 *  'OS:OtherEquipment:Definition'. Its fields are derived from the EnergyPlus IDD object
 *  'OtherEquipment'. Note that this object represents an arbitrary internal gain (or loss)
 *  not tied into any end-use category, and so should be used with caution.
 *  \sa OtherEquipment
 */
  class MODEL_API OtherEquipmentDefinition : public SpaceLoadDefinition
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    explicit OtherEquipmentDefinition(const Model& model);

    virtual ~OtherEquipmentDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OtherEquipmentDefinition(const OtherEquipmentDefinition& other) = default;
    OtherEquipmentDefinition(OtherEquipmentDefinition&& other) = default;
    OtherEquipmentDefinition& operator=(const OtherEquipmentDefinition&) = default;
    OtherEquipmentDefinition& operator=(OtherEquipmentDefinition&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDesignLevelCalculationMethodValues();

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

    /// Sets designLevelCalculationMethod to 'EquipmentLevel', clears wattsperSpaceFloorArea
    /// and wattsperPerson, and sets designLevel.
    bool setDesignLevel(double designLevel);

    /// Sets designLevelCalculationMethod to 'Watts/Area', clears designLevel
    /// and wattsperPerson, and sets wattsperSpaceFloorArea.
    bool setWattsperSpaceFloorArea(double wattsperSpaceFloorArea);

    /// Sets designLevelCalculationMethod to 'Watts/Person', clears designLevel
    /// and wattsperSpaceFloorArea, and sets wattsperPerson.
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
    using ImplType = detail::OtherEquipmentDefinition_Impl;

    friend class detail::OtherEquipmentDefinition_Impl;
    friend class openstudio::detail::IdfObject_Impl;
    friend class IdfObject;
    friend class Model;

    explicit OtherEquipmentDefinition(std::shared_ptr<detail::OtherEquipmentDefinition_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OtherEquipmentDefinition");
  };

  /** \relates OtherEquipmentDefinition*/
  using OptionalOtherEquipmentDefinition = boost::optional<OtherEquipmentDefinition>;

  /** \relates OtherEquipmentDefinition*/
  using OtherEquipmentDefinitionVector = std::vector<OtherEquipmentDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OTHEREQUIPMENTDEFINITION_HPP
