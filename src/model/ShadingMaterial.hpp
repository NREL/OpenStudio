/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADINGMATERIAL_HPP
#define MODEL_SHADINGMATERIAL_HPP

#include "ModelAPI.hpp"
#include "FenestrationMaterial.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ShadingMaterial_Impl;
  }

  /** ShadingMaterial is a FenestrationMaterial that serves as a base class for objects like Blind,
 *  Screen, and Shade. */
  class MODEL_API ShadingMaterial : public FenestrationMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ShadingMaterial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ShadingMaterial(const ShadingMaterial& other) = default;
    ShadingMaterial(ShadingMaterial&& other) = default;
    ShadingMaterial& operator=(const ShadingMaterial&) = default;
    ShadingMaterial& operator=(ShadingMaterial&&) = default;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ShadingMaterial_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ShadingMaterial_Impl;

    ShadingMaterial(IddObjectType type, const Model& model);

    explicit ShadingMaterial(std::shared_ptr<detail::ShadingMaterial_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ShadingMaterial");
  };

  /** \relates ShadingMaterial */
  using OptionalShadingMaterial = boost::optional<ShadingMaterial>;

  /** \relates ShadingMaterial */
  using ShadingMaterialVector = std::vector<ShadingMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGMATERIAL_HPP
