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

#ifndef MODEL_STANDARDSINFORMATIONMATERIAL_HPP
#define MODEL_STANDARDSINFORMATIONMATERIAL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Material;

namespace detail {

  class StandardsInformationMaterial_Impl;

} // detail

/** StandardsInformationMaterial is a ModelObject that wraps the OpenStudio IDD object 'OS:StandardsInformation:Material'. */
class MODEL_API StandardsInformationMaterial : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit StandardsInformationMaterial(const Model& model);

  virtual ~StandardsInformationMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Material.
  Material material() const;

  boost::optional<std::string> materialStandard() const;

  boost::optional<std::string> materialStandardSource() const;

  boost::optional<std::string> standardsCategory() const;

  boost::optional<std::string> standardsIdentifier() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Material.
  bool setMaterial(const Material& material);

  void setMaterialStandard(std::string materialStandard);

  void resetMaterialStandard();

  void setMaterialStandardSource(std::string materialStandardSource);

  void resetMaterialStandardSource();

  void setStandardsCategory(std::string standardsCategory);

  void resetStandardsCategory();

  void setStandardsIdentifier(std::string standardsIdentifier);

  void resetStandardsIdentifier();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::StandardsInformationMaterial_Impl ImplType;

  explicit StandardsInformationMaterial(std::shared_ptr<detail::StandardsInformationMaterial_Impl> impl);

  friend class detail::StandardsInformationMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
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

