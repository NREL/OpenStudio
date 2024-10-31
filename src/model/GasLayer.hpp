/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GASLAYER_HPP
#define MODEL_GASLAYER_HPP

#include "ModelAPI.hpp"
#include "FenestrationMaterial.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class GasLayer_Impl;
  }

  /** GasLayer is a FenestrationMaterial that serves as a base class for gaseous layers in
 *  fenestration \link LayeredConstruction LayeredConstructions \endlink. The base class is used
 *  to determine if EnergyPlus's rules for relative placement of fenestration layers are
 *  satisfied. */
  class MODEL_API GasLayer : public FenestrationMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~GasLayer() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GasLayer(const GasLayer& other) = default;
    GasLayer(GasLayer&& other) = default;
    GasLayer& operator=(const GasLayer&) = default;
    GasLayer& operator=(GasLayer&&) = default;

    //@}
   protected:
    /// @cond
    using ImplType = detail::GasLayer_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::GasLayer_Impl;

    GasLayer(IddObjectType type, const Model& model);

    explicit GasLayer(std::shared_ptr<detail::GasLayer_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GasLayer");
  };

  using OptionalGasLayer = boost::optional<GasLayer>;

  using GasLayerVector = std::vector<GasLayer>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GASLAYER_HPP
