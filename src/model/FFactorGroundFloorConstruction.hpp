/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP
#define MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class FFactorGroundFloorConstruction_Impl;
  }

  /** FFactorGroundFloorConstruction is a ConstructionBase that wraps EnergyPlus IDD object
 *  'Construction:FfactorGroundFloor'. This is a simple, unlayered construction object that may
 *  be applied to a Surface (typically a ground floor). */
  class MODEL_API FFactorGroundFloorConstruction : public ConstructionBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FFactorGroundFloorConstruction(const Model& model, double fFactor = 0.1, double area = 0.1, double perimeterExposed = 0.1);

    virtual ~FFactorGroundFloorConstruction() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FFactorGroundFloorConstruction(const FFactorGroundFloorConstruction& other) = default;
    FFactorGroundFloorConstruction(FFactorGroundFloorConstruction&& other) = default;
    FFactorGroundFloorConstruction& operator=(const FFactorGroundFloorConstruction&) = default;
    FFactorGroundFloorConstruction& operator=(FFactorGroundFloorConstruction&&) = default;

    double fFactor() const;
    bool setFFactor(double fFactor);

    double area() const;
    bool setArea(double area);

    double perimeterExposed() const;
    bool setPerimeterExposed(double perimeterExposed);

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
   protected:
    /// @cond
    using ImplType = detail::FFactorGroundFloorConstruction_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::FFactorGroundFloorConstruction_Impl;

    explicit FFactorGroundFloorConstruction(std::shared_ptr<detail::FFactorGroundFloorConstruction_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FFactorGroundFloorConstruction");
  };

  /** \relates FFactorGroundFloorConstruction */
  using OptionalFFactorGroundFloorConstruction = boost::optional<FFactorGroundFloorConstruction>;

  /** \relates FFactorGroundFloorConstruction */
  using FFactorGroundFloorConstructionVector = std::vector<FFactorGroundFloorConstruction>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP
