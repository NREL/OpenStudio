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

#ifndef MODEL_EXTERIORLIGHTSDEFINITION_HPP
#define MODEL_EXTERIORLIGHTSDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ExteriorLightsDefinition_Impl;

} // detail

/** ExteriorLightsDefinition is a ResourceObject that wraps the OpenStudio IDD object
 *  'OS:Exterior:Lights:Definition'. */
class MODEL_API ExteriorLightsDefinition : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Defaults design level to 0.0 W. */
  explicit ExteriorLightsDefinition(const Model& model);

  virtual ~ExteriorLightsDefinition() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double designLevel() const;

  //@}
  /** @name Setters */
  //@{

  bool setDesignLevel(double designLevel);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ExteriorLightsDefinition_Impl ImplType;

  explicit ExteriorLightsDefinition(std::shared_ptr<detail::ExteriorLightsDefinition_Impl> impl);

  friend class detail::ExteriorLightsDefinition_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ExteriorLightsDefinition");
};

/** \relates ExteriorLightsDefinition*/
typedef boost::optional<ExteriorLightsDefinition> OptionalExteriorLightsDefinition;

/** \relates ExteriorLightsDefinition*/
typedef std::vector<ExteriorLightsDefinition> ExteriorLightsDefinitionVector;

} // model
} // openstudio

#endif // MODEL_EXTERIORLIGHTSDEFINITION_HPP

