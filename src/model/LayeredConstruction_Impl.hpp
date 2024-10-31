/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LAYEREDCONSTRUCTION_IMPL_HPP
#define MODEL_LAYEREDCONSTRUCTION_IMPL_HPP

#include "ConstructionBase_Impl.hpp"

namespace openstudio {

class ValidityReport;

namespace model {

  // forward declarations
  class LayeredConstruction;
  class Construction;
  class ConstructionWithInternalSource;
  class Material;
  class OpaqueMaterial;
  class FenestrationMaterial;
  class ModelPartitionMaterial;

  namespace detail {

    class MODEL_API LayeredConstruction_Impl : public ConstructionBase_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      LayeredConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      LayeredConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      LayeredConstruction_Impl(const LayeredConstruction_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LayeredConstruction_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      /** Returns the material layers that make up this construction. Listed in order from outside in,
     *  that is, the Material at index 0 faces out, from the point of view of the Zone, should this
     *  LayeredConstruction be associated with a PlanarSurface. */
      std::vector<Material> layers() const;

      Material getLayer(unsigned layerIndex) const;

      //@}
      /** @name Setters */
      //@{

      virtual bool eraseLayer(unsigned layerIndex);

      virtual bool insertLayer(unsigned layerIndex, const Material& material);

      virtual bool setLayer(unsigned layerIndex, const Material& material);

      virtual bool setLayers(const std::vector<Material>& materials);

      virtual bool setLayer(const ModelPartitionMaterial& modelPartitionMaterial);

      /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include
     *  appropriate film coefficients. By default, assumes still air indoors and 15 mph outdoor air
     *  speed. */
      virtual bool setUFactor(double value) override;

      /** Set the u-factor of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a conductance. */
      virtual bool setUFactor(double value, double filmResistance) override;

      /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
      virtual bool setThermalConductance(double value);

      /** Set the conductance of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a u-factor. */
      virtual bool setThermalConductance(double value, double filmResistance);

      /** Same as setThermalConductance. Just adding to support current interface of ConstructionBase.
     *  One of these sets of methods should be deprecated. */
      virtual bool setConductance(double value) override;

      /** Same as setThermalConductance. Just adding to support current interface of ConstructionBase.
     *  One of these sets of methods should be deprecated. */
      virtual bool setConductance(double value, double filmResistance) override;

      //@}
      /** @name Queries */
      //@{

      bool isOpaque() const override;

      bool isFenestration() const override;

      bool isSolarDiffusing() const override;

      bool isModelPartition() const override;

      /** Returns true if the construction has RoofVegetation as the outer layer. */
      virtual bool isGreenRoof() const override;

      /** Returns true if this is a symmetric layered construction. */
      bool isSymmetric() const;

      /** Returns true if this construction has the same layers as other one. */
      bool equalLayers(const LayeredConstruction& other) const;

      /** Returns true if this construction has the same layers but in reverse order as other one. */
      bool reverseEqualLayers(const LayeredConstruction& other) const;

      /** Returns layer indices that do not actually point to a Material. Before simulation, the
     *  returned vector should be empty. */
      std::vector<unsigned> nullLayers() const;

      /** Returns the number of non-null layers in this construction. */
      unsigned numLayers() const;

      std::vector<unsigned> getLayerIndices(const Material& material) const;

      /** Get the u-factor of this construction. Includes film coefficients. */
      virtual boost::optional<double> uFactor() const override;

      /** Get the u-factor of this construction. Includes filmResistance. */
      virtual boost::optional<double> uFactor(double filmResistance) const override;

      /** Get the thermal conductance of this construction. Does not include film coefficients. */
      virtual boost::optional<double> thermalConductance() const override;

      /** Get the thermal conductance of this construction. filmResistance is subtracted out of the
     *  thermalResistance if necessary. */
      virtual boost::optional<double> thermalConductance(double filmResistance) const override;

      /** Get the heat capacity of this construction (J/m^2*K). Only works for
     *  \link LayeredConstruction LayeredConstructions \endlink of \link StandardOpaqueMaterial
     *  StandardOpaqueMaterials \endlink. */
      virtual boost::optional<double> heatCapacity() const override;

      /// get interior visible absorptance (unitless)
      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      /// get exterior visible absorptance (unitless)
      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      /// get visible transmittance (unitless)
      virtual boost::optional<double> visibleTransmittance() const override;

      //@}
      /** @name Standards Methods */
      //@{

      /** Returns the layer designated as insulation, if such a designation has been made. */
      boost::optional<OpaqueMaterial> insulation() const;

      /** Notes that insulationLayer is the perturbable insulation layer. Only works if
     *  insulationLayer is already in this construction. */
      bool setInsulation(const OpaqueMaterial& insulationLayer);

      /** Removes any existing insulation layer designations. Does not touch the actual composition
     *  of the construction. */
      void resetInsulation();

      void ensureUniqueLayers();

      //@}

     protected:
      friend class LayeredConstruction;

      // Erase all nullLayers, and return the new value for layerIndex, if it is affected by erasures.
      // If layerIndex was originally pointing to a nullLayer, upon return it will point to the first
      // non-null layer after its original position.
      unsigned mf_clearNullLayers(unsigned layerIndex = 0);

     private:
      REGISTER_LOGGER("openstudio.model.LayeredConstruction");

      boost::optional<ModelObject> insulationAsModelObject() const;

      bool setInsulationAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LAYEREDCONSTRUCTION_IMPL_HPP
