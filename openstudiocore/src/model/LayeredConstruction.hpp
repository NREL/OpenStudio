/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
