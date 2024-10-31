/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORWATEREQUIPMENTDEFINITION_HPP
#define MODEL_EXTERIORWATEREQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ExteriorWaterEquipmentDefinition_Impl;

  }  // namespace detail

  /** ExteriorWaterEquipmentDefinition is a ExteriorLoadDefinition that wraps the OpenStudio IDD object
 *  'OS:Exterior:WaterEquipment:Definition'. */
  class MODEL_API ExteriorWaterEquipmentDefinition : public ExteriorLoadDefinition
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Defaults design level to 0.0 W. */
    explicit ExteriorWaterEquipmentDefinition(const Model& model);

    virtual ~ExteriorWaterEquipmentDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorWaterEquipmentDefinition(const ExteriorWaterEquipmentDefinition& other) = default;
    ExteriorWaterEquipmentDefinition(ExteriorWaterEquipmentDefinition&& other) = default;
    ExteriorWaterEquipmentDefinition& operator=(const ExteriorWaterEquipmentDefinition&) = default;
    ExteriorWaterEquipmentDefinition& operator=(ExteriorWaterEquipmentDefinition&&) = default;

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
    using ImplType = detail::ExteriorWaterEquipmentDefinition_Impl;

    explicit ExteriorWaterEquipmentDefinition(std::shared_ptr<detail::ExteriorWaterEquipmentDefinition_Impl> impl);

    friend class detail::ExteriorWaterEquipmentDefinition_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorWaterEquipmentDefinition");
  };

  /** \relates ExteriorWaterEquipmentDefinition*/
  using OptionalExteriorWaterEquipmentDefinition = boost::optional<ExteriorWaterEquipmentDefinition>;

  /** \relates ExteriorWaterEquipmentDefinition*/
  using ExteriorWaterEquipmentDefinitionVector = std::vector<ExteriorWaterEquipmentDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORWATEREQUIPMENTDEFINITION_HPP
