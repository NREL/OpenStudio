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

#ifndef MODEL_INFRAREDTRANSPARENTMATERIAL_HPP
#define MODEL_INFRAREDTRANSPARENTMATERIAL_HPP

#include "ModelAPI.hpp"
#include "ModelPartitionMaterial.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class InfraredTransparentMaterial_Impl;

} // detail

/** InfraredTransparentMaterial is a ModelPartitionMaterial that wraps the OpenStudio IDD object 'OS:Material:InfraredTransparent'. */
class MODEL_API InfraredTransparentMaterial : public ModelPartitionMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit InfraredTransparentMaterial(const Model& model);

  virtual ~InfraredTransparentMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  //@}
  /** @name Setters */
  //@{

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::InfraredTransparentMaterial_Impl ImplType;

  explicit InfraredTransparentMaterial(std::shared_ptr<detail::InfraredTransparentMaterial_Impl> impl);

  friend class detail::InfraredTransparentMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.InfraredTransparentMaterial");
};

/** \relates InfraredTransparentMaterial*/
typedef boost::optional<InfraredTransparentMaterial> OptionalInfraredTransparentMaterial;

/** \relates InfraredTransparentMaterial*/
typedef std::vector<InfraredTransparentMaterial> InfraredTransparentMaterialVector;

} // model
} // openstudio

#endif // MODEL_INFRAREDTRANSPARENTMATERIAL_HPP

