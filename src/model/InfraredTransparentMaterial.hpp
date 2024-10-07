/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INFRAREDTRANSPARENTMATERIAL_HPP
#define MODEL_INFRAREDTRANSPARENTMATERIAL_HPP

#include "ModelAPI.hpp"
#include "ModelPartitionMaterial.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class InfraredTransparentMaterial_Impl;

  }  // namespace detail

  /** InfraredTransparentMaterial is a ModelPartitionMaterial that wraps the OpenStudio IDD object 'OS:Material:InfraredTransparent'. */
  class MODEL_API InfraredTransparentMaterial : public ModelPartitionMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit InfraredTransparentMaterial(const Model& model);

    virtual ~InfraredTransparentMaterial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InfraredTransparentMaterial(const InfraredTransparentMaterial& other) = default;
    InfraredTransparentMaterial(InfraredTransparentMaterial&& other) = default;
    InfraredTransparentMaterial& operator=(const InfraredTransparentMaterial&) = default;
    InfraredTransparentMaterial& operator=(InfraredTransparentMaterial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::InfraredTransparentMaterial_Impl;

    explicit InfraredTransparentMaterial(std::shared_ptr<detail::InfraredTransparentMaterial_Impl> impl);

    friend class detail::InfraredTransparentMaterial_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.InfraredTransparentMaterial");
  };

  /** \relates InfraredTransparentMaterial*/
  using OptionalInfraredTransparentMaterial = boost::optional<InfraredTransparentMaterial>;

  /** \relates InfraredTransparentMaterial*/
  using InfraredTransparentMaterialVector = std::vector<InfraredTransparentMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INFRAREDTRANSPARENTMATERIAL_HPP
