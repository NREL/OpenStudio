/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONSTRUCTIONBASE_IMPL_HPP
#define MODEL_CONSTRUCTIONBASE_IMPL_HPP

#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class StandardsInformationConstruction;
  class RenderingColor;

  namespace detail {

    class MODEL_API ConstructionBase_Impl : public ResourceObject_Impl
    {

      // may require simulation results.

      // may require simulation results.

      // may require simulation results.

     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      ConstructionBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ConstructionBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ConstructionBase_Impl(const ConstructionBase_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ConstructionBase_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      /** Returns this construction's standards information, constructing a new object if necessary. */
      StandardsInformationConstruction standardsInformation() const;

      virtual double getNetArea() const;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      //@}
      /** @name Setters */
      //@{

      /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include
     *  appropriate film coefficients. By default, assumes still air indoors and 15 mph outdoor air
     *  speed. */
      virtual bool setUFactor(double value);

      /** Set the u-factor of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a conductance. */
      virtual bool setUFactor(double value, double filmResistance);

      /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
      virtual bool setConductance(double value);

      /** Set the conductance of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a u-factor. */
      virtual bool setConductance(double value, double filmResistance);

      //@}
      /** @name Queries */
      //@{

      virtual bool isOpaque() const = 0;

      virtual bool isFenestration() const = 0;

      virtual bool isSolarDiffusing() const = 0;

      virtual bool isModelPartition() const = 0;

      /** Returns true if the construction has RoofVegetation as the outer layer. */
      virtual bool isGreenRoof() const;

      /** Get the u-factor of this construction. Includes film coefficients. */
      virtual boost::optional<double> uFactor() const;

      /** Get the u-factor of this construction. Includes filmResistance. */
      virtual boost::optional<double> uFactor(double filmResistance) const;

      /** Get the thermal conductance of this construction. Does not include film coefficients. */
      virtual boost::optional<double> thermalConductance() const;

      /** Get the thermal conductance of this construction. filmResistance is subtracted out of the
     *  thermalResistance if necessary. */
      virtual boost::optional<double> thermalConductance(double filmResistance) const;

      /** Get the heat capacity of this construction (J/m^2*K). Only works for
     *  \link LayeredConstruction LayeredConstructions \endlink of \link StandardOpaqueMaterial
     *  StandardOpaqueMaterials \endlink. */
      virtual boost::optional<double> heatCapacity() const;

      /// get interior visible absorptance (unitless)
      virtual boost::optional<double> interiorVisibleAbsorptance() const;

      /// get exterior visible absorptance (unitless)
      virtual boost::optional<double> exteriorVisibleAbsorptance() const;

      /// get visible transmittance (unitless)
      virtual boost::optional<double> visibleTransmittance() const;

      /// Returns the rendering color.
      virtual boost::optional<RenderingColor> renderingColor() const;

      /// Sets the rendering color.
      virtual bool setRenderingColor(const RenderingColor& renderingColor);

      /// Resets the rendering color.
      virtual void resetRenderingColor();

      /// Resets the rendering color.
      virtual int renderingColorIndex() const = 0;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.ConstructionBase");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONBASE_IMPL_HPP
