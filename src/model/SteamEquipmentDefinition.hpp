/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STEAMEQUIPMENTDEFINITION_HPP
#define MODEL_STEAMEQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SteamEquipmentDefinition_Impl;

  }  // namespace detail

  /** SteamEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object
 *  'OS:SteamEquipment:Definition'. Its fields are derived from the EnergyPlus IDD object
 *  'SteamEquipment'. Note that this object is not tied into any plant loop or water system, so
 *  it should not be used in models for which water use is an important output.
 *  \sa SteamEquipment
 */
  class MODEL_API SteamEquipmentDefinition : public SpaceLoadDefinition
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SteamEquipmentDefinition(const Model& model);

    virtual ~SteamEquipmentDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SteamEquipmentDefinition(const SteamEquipmentDefinition& other) = default;
    SteamEquipmentDefinition(SteamEquipmentDefinition&& other) = default;
    SteamEquipmentDefinition& operator=(const SteamEquipmentDefinition&) = default;
    SteamEquipmentDefinition& operator=(SteamEquipmentDefinition&&) = default;

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

    /// Sets designLevelCalculationMethod to 'DesignLevel', clears wattsperSpaceFloorArea
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
    using ImplType = detail::SteamEquipmentDefinition_Impl;

    friend class detail::SteamEquipmentDefinition_Impl;
    friend class openstudio::detail::IdfObject_Impl;
    friend class IdfObject;
    friend class Model;

    explicit SteamEquipmentDefinition(std::shared_ptr<detail::SteamEquipmentDefinition_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SteamEquipmentDefinition");
  };

  /** \relates SteamEquipmentDefinition*/
  using OptionalSteamEquipmentDefinition = boost::optional<SteamEquipmentDefinition>;

  /** \relates SteamEquipmentDefinition*/
  using SteamEquipmentDefinitionVector = std::vector<SteamEquipmentDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STEAMEQUIPMENTDEFINITION_HPP
