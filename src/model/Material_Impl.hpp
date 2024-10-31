/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIAL_IMPL_HPP
#define MODEL_MATERIAL_IMPL_HPP

#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class Material;
  class StandardsInformationMaterial;

  namespace detail {

    class MODEL_API Material_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      Material_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      Material_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      Material_Impl(const Material_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Material_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /** Get the thickness of the material. For some materials, 0.0 is always returned. */
      virtual double thickness() const;

      virtual boost::optional<double> getVisibleTransmittance() const = 0;

      virtual boost::optional<double> interiorVisibleAbsorptance() const;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const;

      StandardsInformationMaterial standardsInformation() const;

      //@}
      /** @name Setters */
      //@{

      /** Set thickness to value (m). For some materials, false is always returned. */
      virtual bool setThickness(double value);

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.Material");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIAL_IMPL_HPP
