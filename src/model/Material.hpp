/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIAL_HPP
#define MODEL_MATERIAL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class StandardsInformationMaterial;

  namespace detail {
    class Material_Impl;
  }

  /** A Material is a ResourceObject that serves as a base class for all objects that can be used
   *  in \link LayeredConstruction LayeredConstructions \endlink. */
  class MODEL_API Material : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Material() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Material(const Material& other) = default;
    Material(Material&& other) = default;
    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) = default;

    //@}
    /** @name Getters */
    //@{

    // Get the thickness of the material. Virtual implementation. For some materials, 0.0 is always returned.
    double thickness() const;

    // Get the visible transmittance of the material. Virtual implementation.
    boost::optional<double> getVisibleTransmittance() const;

    // Get the interiorVisibleAbsorptance of the material. Virtual implementation.
    boost::optional<double> interiorVisibleAbsorptance() const;

    // Get the exteriorVisibleAbsorptance of the material. Virtual implementation.
    boost::optional<double> exteriorVisibleAbsorptance() const;

    /** Returns this materials's standards information, constructing a new object if necessary. */
    StandardsInformationMaterial standardsInformation() const;

    //@}
    /** @name Setters */
    //@{

    // Set thickness to value (m). For some materials, false is always returned.
    bool setThickness(double value);

    //@}
   protected:
    /// @cond
    using ImplType = detail::Material_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::Material_Impl;

    Material(IddObjectType type, const Model& model);

    explicit Material(std::shared_ptr<detail::Material_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Material");
  };

  /** \relates Material */
  using OptionalMaterial = boost::optional<Material>;

  /** \relates Material */
  using MaterialVector = std::vector<Material>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIAL_HPP
