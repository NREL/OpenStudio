/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_LAYEREDCONSTRUCTION_HPP
#define MODEL_LAYEREDCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

class Material;
class OpaqueMaterial;
class FenestrationMaterial;
class ModelPartitionMaterial;

namespace detail{
  class LayeredConstruction_Impl;
}

/** LayeredConstruction is a ConstructionBase that serves as a base class to concrete construction
 *  classes that are composed of Material layers. */
class MODEL_API LayeredConstruction : public ConstructionBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~LayeredConstruction() {}

  //@}
  /** @name Static Methods */
  //@{

  /** Returns true if materials defines a valid EnergyPlus construction. Returns false if layers are empty.
   *  Checks the following rules if first layer is an OpaqueMaterial:
   *
   *  \li AirGap must have non-AirGap on either side.
   *  \li RoofVegetation must be the exterior layer.
   *
   *  Checks the following rules if first layer is a FenestrationMaterial:
   *
   *  \li SimpleGlazing cannot be combined with other \link Glazing Glazings \endlink or any 
   *      \link GasLayer GasLayers\endlink.
   *  \li \link GasLayer GasLayers\endlink must have non-GasLayer on either side.
   *  \li Ultimately, \link GasLayer GasLayers\endlink must have Glazing on either side.
   *
   *  Checks the following rules if first layer is a ModelPartitionMaterial:
   *
   *  \li Layer size is 1
   *
   *  \todo Verify that this is a comprehensive and accurate list.
   */
  static bool layersAreValid(const std::vector<Material>& materials);
  static bool layersAreValid(const std::vector<OpaqueMaterial>& opaqueMaterials);
  static bool layersAreValid(const std::vector<FenestrationMaterial>& fenestrationMaterials);


  //@}
  /** @name Getters */
  //@{

  /** Returns the material layers that make up this construction. Listed in order from outside in, 
   *  that is, the Material at index 0 faces out from the point of view of the Zone, should this 
   *  LayeredConstruction be associated with a PlanarSurface. */
  std::vector<Material> layers() const;

  /** Get the Material at layerIndex, where layerIndex is between 0 and numLayers() - 1. Throws a
   *  openstudio::Exception if layerIndex >= numLayers. */
  Material getLayer(unsigned layerIndex) const;

  //@}
  /** @name Setters */
  //@{

  /** Erases the Material at layerIndex, if possible. Must have 0 <= layerIndex < numLayers, and
   *  resulting construction must satisfy layersAreValid. */
  bool eraseLayer(unsigned layerIndex);

  /** Insert opaqueMaterial at layerIndex, if possible. Must have 0 <= layerIndex <= numLayers,
   *  and this construction must have no layers or satisfy isOpaque == true. Furthermore, the 
   *  resulting construction must satisfy layersAreValid. */
  bool insertLayer(unsigned layerIndex, const Material& material);

  /** Set the layer at layerIndex to opaqueMaterial, if possible. Must have 0 <= layerIndex <
   *  numLayers, and this construction must satisfy isOpaque == true. Furthermore, the 
   *  resulting construction must satisfy layersAreValid. */
  bool setLayer(unsigned layerIndex, const Material& material);

  /** Set the construction to use opaqueMaterials as its layers, if layersAreValid(opaqueMaterials). 
   *  Clears out all previous Material layers. */
  bool setLayers(const std::vector<Material>& materials);

  /** Sets the construction to use modelPartitionMaterial. Clears out all previous Material 
   *  layers. */
  bool setLayer(const ModelPartitionMaterial& modelPartitionMaterial);

  //@}
  /** @name Queries */
  //@{

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

  /** Returns the indices from which this construction points to material. */
  std::vector<unsigned> getLayerIndices(const Material& material) const;

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

  /** Ensures that all materials in this construction are used by this construction only.  Allows standards
  *   to then appply standards specified material properties to each layer (e.g. SRI). */
  void ensureUniqueLayers(); 

  //@}
 protected:
  /// @cond
  typedef detail::LayeredConstruction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::LayeredConstruction_Impl;

  LayeredConstruction(IddObjectType type,const Model& model);

  explicit LayeredConstruction(std::shared_ptr<detail::LayeredConstruction_Impl> impl);
  
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.LayeredConstruction");
};

/** \relates LayeredConstruction */
typedef boost::optional<LayeredConstruction> OptionalLayeredConstruction;

/** \relates LayeredConstruction */
typedef std::vector<LayeredConstruction> LayeredConstructionVector;

} // model
} // openstudio

#endif // MODEL_LAYEREDCONSTRUCTION_HPP
