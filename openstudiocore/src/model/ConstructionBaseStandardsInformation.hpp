/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef MODEL_CONSTRUCTIONBASESTANDARDSINFORMATION_HPP
#define MODEL_CONSTRUCTIONBASESTANDARDSINFORMATION_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {
namespace model {

class ConstructionBase;
class Material;

namespace detail {
  class ConstructionBaseStandardsInformation_Impl;
}

/** ConstructionBaseStandardsInformation is the ModelObject that wraps OpenStudio IDD object 
 *  'OS:StandardsInformation:ConstructionFields'. ConstructionBaseStandardsInformation provides a 
 *  place to register information about a construction that building energy standards often need 
 *  to know, but is irrelevant to simulation. It contains user-specified values that are not 
 *  generally checked against input or output data. */
class MODEL_API ConstructionBaseStandardsInformation : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Create new, blank object. */
  explicit ConstructionBaseStandardsInformation(Model& model);

  /** Create new object as child of construction. */
  explicit ConstructionBaseStandardsInformation(const ConstructionBase& construction);

  virtual ~ConstructionBaseStandardsInformation() {}

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
  boost::optional<ConstructionBase> construction() const;

  /** Return the surface type to which this construction should be applied. */
  std::string intendedSurfaceType() const;

  /** Return the construction type, that is, a descriptor like 'Mass' or 'Wood-Framed' that 
   *  describes the basic type of construction. To be used with standardsinterface, this 
   *  string should match an appropriate enumeration value in the 
   *  standardsinterface::DataDictionary. The enumeration for which this should be a value
   *  generally varies by intendedSurfaceType(). */
  std::string constructionType() const;

  /** Return the layer of construction() whose thickness can be perturbed (usually to reach a 
   *  certain property value for the overall construction), if the construction() exists and is a 
   *  LayeredConstruction. */
  boost::optional<Material> perturbableLayer() const;

  /** Return a description of the perturbableLayer(), for instance, 'Insulation'.*/
  std::string perturbableLayerType() const;

  //@}
  /** @name Setters */
  //@{

  /** Point this object to construction. If this object already points to (a different) construction,
   *  then that relationship will be lost. If you want the two constructions to have the same 
   *  standards information, you will need to clone the original ConstructionBaseStandardsInformation,
   *  then point the clone to the other construction. */
  void setConstruction(const ConstructionBase& construction);

  /** Sets the intendedSurfaceType to type. \sa intendedSurfaceTypeValues */
  bool setIntendedSurfaceType(const std::string& type);

  /** Set the constructionType to type. To be used with standardsinterface, this string should 
   *  match an appropriate enumeration value in the standardsinterface::DataDictionary. The 
   *  enumeration for which this should be a value generally varies by intendedSurfaceType(). */
  void setConstructionType(const std::string& type);

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

  //@}
 protected:
  /// @cond
  typedef detail::ConstructionBaseStandardsInformation_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::ConstructionBaseStandardsInformation_Impl;

  explicit ConstructionBaseStandardsInformation(
      boost::shared_ptr<detail::ConstructionBaseStandardsInformation_Impl> impl);  
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ConstructionBaseStandardsInformation");
};

/** \relates ConstructionBaseStandardsInformation */
typedef boost::optional<ConstructionBaseStandardsInformation> OptionalConstructionBaseStandardsInformation;

/** \relates ConstructionBaseStandardsInformation */
typedef std::vector<ConstructionBaseStandardsInformation> ConstructionBaseStandardsInformationVector;

} // model
} // openstudio 

#endif // MODEL_CONSTRUCTIONBASESTANDARDSINFORMATION_HPP
