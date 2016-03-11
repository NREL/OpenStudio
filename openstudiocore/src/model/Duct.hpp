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

#ifndef MODEL_DUCT_HPP
#define MODEL_DUCT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class Duct_Impl;

} // detail

class MODEL_API Duct : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Duct(const Model& model);

  virtual ~Duct() {}

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
  typedef detail::Duct_Impl ImplType;

  explicit Duct(std::shared_ptr<detail::Duct_Impl> impl);

  friend class detail::Duct_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Duct");
};

/** \relates Duct*/
typedef boost::optional<Duct> OptionalDuct;

/** \relates Duct*/
typedef std::vector<Duct> DuctVector;

} // model
} // openstudio

#endif // MODEL_DUCT_HPP

