/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONSTRUCTION_HPP
#define MODEL_CONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "LayeredConstruction.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Construction_Impl;
  }

  /** Construction is a LayeredConstruction that wraps the EnergyPlus IDD object 'Construction'.
 *  A Construction consists of a number of material layers, each of which should belong to the
 *  same base class: OpaqueMaterial, FenestrationMaterial, or ModelPartitionMaterial. In addition
 *  the layers must satisfy some restrictions in order to form a valid EnergyPlus construction,
 *  see the documentation for LayeredConstruction. */
  class MODEL_API Construction : public LayeredConstruction
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Construction(const Model& model);

    explicit Construction(const std::vector<OpaqueMaterial>& opaqueMaterials);

    explicit Construction(const std::vector<FenestrationMaterial>& fenestrationMaterials);

    explicit Construction(const ModelPartitionMaterial& modelPartitionMaterial);

    virtual ~Construction() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Construction(const Construction& other) = default;
    Construction(Construction&& other) = default;
    Construction& operator=(const Construction&) = default;
    Construction& operator=(Construction&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}

    /** Searches the model for a layered construction with layers which are the reverse of this one.  Returns that construction
   *  if found, creates one if not. */
    Construction reverseConstruction() const;

   protected:
    /// @cond
    using ImplType = detail::Construction_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::Construction_Impl;

    explicit Construction(std::shared_ptr<detail::Construction_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Construction");
  };

  /** \relates Construction */
  using OptionalConstruction = boost::optional<Construction>;

  /** \relates Construction */
  using ConstructionVector = std::vector<Construction>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTION_HPP
