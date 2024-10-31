/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_HPP
#define MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class CFactorUndergroundWallConstruction_Impl;
  }

  /** CFactorUndergroundWallConstruction is a ConstructionBase object that wraps EnergyPlus IDD
 *  object 'Construction:CfactorUndergroundWall'. */
  class MODEL_API CFactorUndergroundWallConstruction : public ConstructionBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** TODO default values should be reviewed */
    explicit CFactorUndergroundWallConstruction(const Model& model, double cFactor = 0.1, double height = 0.1);

    virtual ~CFactorUndergroundWallConstruction() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CFactorUndergroundWallConstruction(const CFactorUndergroundWallConstruction& other) = default;
    CFactorUndergroundWallConstruction(CFactorUndergroundWallConstruction&& other) = default;
    CFactorUndergroundWallConstruction& operator=(const CFactorUndergroundWallConstruction&) = default;
    CFactorUndergroundWallConstruction& operator=(CFactorUndergroundWallConstruction&&) = default;

    double cFactor() const;
    bool setCFactor(double cFactor);

    double height() const;
    bool setHeight(double height);

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CFactorUndergroundWallConstruction_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::CFactorUndergroundWallConstruction_Impl;

    explicit CFactorUndergroundWallConstruction(std::shared_ptr<detail::CFactorUndergroundWallConstruction_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CFactorUndergroundWallConstruction");
  };

  /** \relates CFactorUndergroundWallConstruction */
  using OptionalCFactorUndergroundWallConstruction = boost::optional<CFactorUndergroundWallConstruction>;

  /** \relates CFactorUndergroundWallConstruction */
  using CFactorUndergroundWallConstructionVector = std::vector<CFactorUndergroundWallConstruction>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_HPP
