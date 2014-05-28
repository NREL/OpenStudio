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

#ifndef MODEL_STANDARDSINFORMATIONCONSTRUCTION_HPP
#define MODEL_STANDARDSINFORMATIONCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;
class Material;

namespace detail {
  class ConstructionBase_Impl;
  class StandardsInformationConstruction_Impl;
}

/** StandardsInformationConstruction is the ModelObject that wraps OpenStudio IDD object 
 *  'OS:StandardsInformation:ConstructionFields'. StandardsInformationConstruction provides a 
 *  place to register information about a construction that building energy standards often need 
 *  to know, but is irrelevant to simulation. It contains user-specified values that are not 
 *  generally checked against input or output data. */
class MODEL_API StandardsInformationConstruction : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~StandardsInformationConstruction() {}

  //@}
  /** @name Static Methods */
  //@{

  static std::vector<std::string> standardPerturbableLayerTypeValues();

  static std::vector<std::string> intendedSurfaceTypeValues();

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  /** Return the ConstructionBase object that this object describes. */
  ConstructionBase construction() const;

  /** Return the surface type to which this construction should be applied. */
  boost::optional<std::string> intendedSurfaceType() const;

  /** This is a freeform field used to identify the construction type for standards.
       \note Standards applied to this model will use this field to determine correct constructions.
       \note More information can be found at https://github.com/NREL/openstudio-standards. */
  boost::optional<std::string> standardsConstructionType() const;

  /** Returns a list of suggestions for standards construction type based on intendedSurfaceType. */
  std::vector<std::string> suggestedStandardsConstructionTypes() const;

  /** Return the layer of construction() whose thickness can be perturbed (usually to reach a 
   *  certain property value for the overall construction), if the construction() exists and is a 
   *  LayeredConstruction. */
  boost::optional<Material> perturbableLayer() const;

  /** Return a description of the perturbableLayer(), for instance, 'Insulation'.*/
  boost::optional<std::string> perturbableLayerType() const;

  //@}
  /** @name Setters */
  //@

  /** Sets the intendedSurfaceType to type. \sa intendedSurfaceTypeValues */
  bool setIntendedSurfaceType(const std::string& type);
  void resetIntendedSurfaceType();

  /** Set the constructionType to type. To be used with standardsinterface, this string should 
   *  match an appropriate enumeration value in the standardsinterface::DataDictionary. The 
   *  enumeration for which this should be a value generally varies by intendedSurfaceType(). */
  void setStandardsConstructionType(const std::string& type);
  void resetStandardsConstructionType();

  /** Set the perturbableLayer to the one at layerIndex. Returns false if construction() is not a 
   *  LayeredConstruction or 
   *
   *  \code 
   *  layerIndex >= construction().get().cast<LayeredConstruction>().numLayers(). 
   *  \endcode */
  bool setPerturbableLayer(unsigned layerIndex);

  /** Set the perturbableLayer to material. Returns false if construction() is not a 
   *  LayeredConstruction or 
   *
   *  \code 
   *  MaterialVector layers = construction().get().cast<LayeredConstruction>().layers();
   *  std::find(layers.begin(),layers.end(),material) == layers.end();
   *  \endcode */
  bool setPerturbableLayer(const Material& material);

  /** Clears all PerturbableLayer information, including type. */
  void resetPerturbableLayer();

  /** Set the perturbableLayerType to type. Can be one of the provided types, or a free-form 
   *  string. \sa standardPerturbableLayerTypeValues */
  void setPerturbableLayerType(const std::string& type);
  void resetPerturbableLayerType();

  //@}
 protected:
  /// @cond
  typedef detail::StandardsInformationConstruction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::StandardsInformationConstruction_Impl;
  friend class detail::ConstructionBase_Impl;

  /** Create new object as child of construction. */
  explicit StandardsInformationConstruction(const ConstructionBase& construction);

  explicit StandardsInformationConstruction(
      std::shared_ptr<detail::StandardsInformationConstruction_Impl> impl);  
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.StandardsInformationConstruction");
};

/** \relates StandardsInformationConstruction */
typedef boost::optional<StandardsInformationConstruction> OptionalStandardsInformationConstruction;

/** \relates StandardsInformationConstruction */
typedef std::vector<StandardsInformationConstruction> StandardsInformationConstructionVector;

} // model
} // openstudio 

#endif // MODEL_STANDARDSINFORMATIONCONSTRUCTION_HPP
