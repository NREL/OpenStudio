/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORFUELEQUIPMENTDEFINITION_HPP
#define MODEL_EXTERIORFUELEQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ExteriorFuelEquipmentDefinition_Impl;

  }  // namespace detail

  /** ExteriorFuelEquipmentDefinition is a ExteriorLoadDefinition that wraps the OpenStudio IDD object
 *  'OS:Exterior:FuelEquipment:Definition'. */
  class MODEL_API ExteriorFuelEquipmentDefinition : public ExteriorLoadDefinition
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Defaults design level to 0.0 W. */
    explicit ExteriorFuelEquipmentDefinition(const Model& model);

    virtual ~ExteriorFuelEquipmentDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorFuelEquipmentDefinition(const ExteriorFuelEquipmentDefinition& other) = default;
    ExteriorFuelEquipmentDefinition(ExteriorFuelEquipmentDefinition&& other) = default;
    ExteriorFuelEquipmentDefinition& operator=(const ExteriorFuelEquipmentDefinition&) = default;
    ExteriorFuelEquipmentDefinition& operator=(ExteriorFuelEquipmentDefinition&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double designLevel() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignLevel(double designLevel);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExteriorFuelEquipmentDefinition_Impl;

    explicit ExteriorFuelEquipmentDefinition(std::shared_ptr<detail::ExteriorFuelEquipmentDefinition_Impl> impl);

    friend class detail::ExteriorFuelEquipmentDefinition_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorFuelEquipmentDefinition");
  };

  /** \relates ExteriorFuelEquipmentDefinition*/
  using OptionalExteriorFuelEquipmentDefinition = boost::optional<ExteriorFuelEquipmentDefinition>;

  /** \relates ExteriorFuelEquipmentDefinition*/
  using ExteriorFuelEquipmentDefinitionVector = std::vector<ExteriorFuelEquipmentDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORFUELEQUIPMENTDEFINITION_HPP
