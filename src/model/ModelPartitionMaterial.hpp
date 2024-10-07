/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MODELPARTITIONMATERIAL_HPP
#define MODEL_MODELPARTITIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ModelPartitionMaterial_Impl;
  }

  /** ModelPartitionMaterial is an abstract class derived from Material.
 *
 *  The ModelPartitionMaterial class is the base class for materials that should
 *  exist as the only layer in a Construction.  ModelPartitionMaterial objects
 *  are typically used for the \link Construction Constructions\endlink of \link Surface Surfaces\endlink
 *  that are virtual boundaries between two \link Space Spaces\endlink. */
  class MODEL_API ModelPartitionMaterial : public Material
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ModelPartitionMaterial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ModelPartitionMaterial(const ModelPartitionMaterial& other) = default;
    ModelPartitionMaterial(ModelPartitionMaterial&& other) = default;
    ModelPartitionMaterial& operator=(const ModelPartitionMaterial&) = default;
    ModelPartitionMaterial& operator=(ModelPartitionMaterial&&) = default;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ModelPartitionMaterial_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ModelPartitionMaterial_Impl;

    ModelPartitionMaterial(IddObjectType type, const Model& model);

    explicit ModelPartitionMaterial(std::shared_ptr<detail::ModelPartitionMaterial_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ModelPartitionMaterial");
  };

  /** \relates ModelPartitionMaterial */
  using OptionalModelPartitionMaterial = boost::optional<ModelPartitionMaterial>;

  /** \relates ModelPartitionMaterial */
  using ModelPartitionMaterialVector = std::vector<ModelPartitionMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MODELPARTITIONMATERIAL_HPP
