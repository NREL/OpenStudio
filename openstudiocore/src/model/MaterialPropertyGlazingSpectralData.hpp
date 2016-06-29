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

#ifndef MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP
#define MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MaterialPropertyGlazingSpectralData_Impl;

} // detail

/** MaterialPropertyGlazingSpectralData is a ResourceObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:GlazingSpectralData'. */
class MODEL_API MaterialPropertyGlazingSpectralData : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit MaterialPropertyGlazingSpectralData(const Model& model);

  virtual ~MaterialPropertyGlazingSpectralData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::MaterialPropertyGlazingSpectralData_Impl ImplType;

  explicit MaterialPropertyGlazingSpectralData(std::shared_ptr<detail::MaterialPropertyGlazingSpectralData_Impl> impl);

  friend class detail::MaterialPropertyGlazingSpectralData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.MaterialPropertyGlazingSpectralData");
};

/** \relates MaterialPropertyGlazingSpectralData*/
typedef boost::optional<MaterialPropertyGlazingSpectralData> OptionalMaterialPropertyGlazingSpectralData;

/** \relates MaterialPropertyGlazingSpectralData*/
typedef std::vector<MaterialPropertyGlazingSpectralData> MaterialPropertyGlazingSpectralDataVector;

} // model
} // openstudio

#endif // MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP

