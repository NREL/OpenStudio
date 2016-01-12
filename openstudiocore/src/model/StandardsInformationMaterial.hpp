/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_STANDARDSINFORMATIONMATERIAL_HPP
#define MODEL_STANDARDSINFORMATIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Material;

namespace detail {
  class StandardsInformationMaterial_Impl;
  class Material_Impl;
}

/** StandardsInformationMaterial is a ModelObject that wraps the OpenStudio IDD object 'OS:StandardsInformation:Material'. */
class MODEL_API StandardsInformationMaterial : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~StandardsInformationMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Returns the material that this standards information object refers to. */
  Material material() const;

  /** Returns the standard that specifies this material, e.g. 'CEC Title24-2013'. */
  boost::optional<std::string> materialStandard() const;

  /** Returns a list of suggested material standards. */
  std::vector<std::string> suggestedMaterialStandards() const;

  /** Identifies the table or section in the standard which specifies this material. */
  boost::optional<std::string> materialStandardSource() const;

  /** Returns a list of suggestions for material standard source based on material standard. */
  std::vector<std::string> suggestedMaterialStandardSources() const;

  /** Returns the category of this material, e.g. 'Plastering Materials' 'Wood Framed Wall', or 'Roofing'. */
  boost::optional<std::string> standardsCategory() const;

  /** Returns a list of suggestions for standards category, filters list by material standard if it is set. */
  std::vector<std::string> suggestedStandardsCategories() const;

  /** Returns true if this standards category represents a composite layer (such as framing with cavity insulation). */
  bool isCompositeMaterial() const;

  /** Returns the id of this material within a standard, e.g. 'Wood siding - 1/2 in.'.*/
  boost::optional<std::string> standardsIdentifier() const;

  /** Returns a list of suggestions for standards identifier, filters list by material standard and standards category if set. */
  std::vector<std::string> suggestedStandardsIdentifiers() const;

  /** Returns the framing material for a composite layer, e.g. 'Wood' or 'Metal'. */
  boost::optional<std::string> compositeFramingMaterial() const;

  /** Returns a list of suggestions for composite framing material, filters list by material standard and standards category if set. */
  std::vector<std::string> suggestedCompositeFramingMaterials() const;

  /** Returns the framing configuration for a composite layer, e.g. 'Wall16inOC'. */
  boost::optional<std::string> compositeFramingConfiguration() const;

  /** Returns a list of suggestions for composite framing configurations, filters list by material standard and standards category if set. */
  std::vector<std::string> suggestedCompositeFramingConfigurations() const;

  /** Returns the framing depth for a composite layer, e.g. '3_5In'. */
  boost::optional<std::string> compositeFramingDepth() const;

  /** Returns a list of suggestions for composite framing depths, filters list by material standard and standards category if set. */
  std::vector<std::string> suggestedCompositeFramingDepths() const;

  /** Returns the framing size for a composite layer, e.g. '2x4'. */
  boost::optional<std::string> compositeFramingSize() const;

  /** Returns a list of suggestions for composite framing sizes, filters list by material standard and standards category if set. */
  std::vector<std::string> suggestedCompositeFramingSizes() const;

  /** Returns the cavity insulation for a composite layer, e.g. '30'*/
  boost::optional<std::string> compositeCavityInsulation() const;

  /** Returns a list of suggestions for cavity insulation, filters list by material standard and standards category if set. */
  std::vector<std::string> suggestedCompositeCavityInsulations() const;

  //@}
  /** @name Setters */
  //@{

  void setMaterialStandard(const std::string& materialStandard);
  void resetMaterialStandard();

  void setMaterialStandardSource(const std::string& materialStandardSource);
  void resetMaterialStandardSource();

  void setStandardsCategory(const std::string& standardsCategory);
  void resetStandardsCategory();

  void setStandardsIdentifier(const std::string& standardsIdentifier);
  void resetStandardsIdentifier();

  void setCompositeFramingMaterial(const std::string& compositeFramingMaterial);
  void resetCompositeFramingMaterial();

  void setCompositeFramingConfiguration(const std::string& compositeFramingConfiguration);
  void resetCompositeFramingConfiguration();

  void setCompositeFramingDepth(const std::string& compositeFramingDepth);
  void resetCompositeFramingDepth();

  void setCompositeFramingSize(const std::string& compositeFramingSize);
  void resetCompositeFramingSize();

  void setCompositeCavityInsulation(const std::string& compositeCavityInsulation);
  void resetCompositeCavityInsulation();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::StandardsInformationMaterial_Impl ImplType;

  explicit StandardsInformationMaterial(const Material& material);

  explicit StandardsInformationMaterial(std::shared_ptr<detail::StandardsInformationMaterial_Impl> impl);

  friend class detail::StandardsInformationMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class openstudio::model::Material;
  friend class openstudio::model::detail::Material_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.StandardsInformationMaterial");
};

/** \relates StandardsInformationMaterial*/
typedef boost::optional<StandardsInformationMaterial> OptionalStandardsInformationMaterial;

/** \relates StandardsInformationMaterial*/
typedef std::vector<StandardsInformationMaterial> StandardsInformationMaterialVector;

} // model
} // openstudio

#endif // MODEL_STANDARDSINFORMATIONMATERIAL_HPP

