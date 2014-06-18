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

#ifndef MODEL_PIPEADIABATIC_HPP
#define MODEL_PIPEADIABATIC_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class PipeAdiabatic_Impl;
} // detail

/** PipeAdiabatic is an interface to the IDD object named "OS:Pipe:Adiabatic"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  Pipe:Adiabatic objects in energyplus.
 */
class MODEL_API PipeAdiabatic : public StraightComponent {

 public:

  /** Constructs a new PipeAdiabatic object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   */
  explicit PipeAdiabatic(const Model& model);

  virtual ~PipeAdiabatic() {}

  unsigned inletPort();

  unsigned outletPort();

  static IddObjectType iddObjectType();

 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond

  typedef detail::PipeAdiabatic_Impl ImplType;

  explicit PipeAdiabatic(std::shared_ptr<detail::PipeAdiabatic_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.PipeAdiabatic");

  /// @endcond

};

/** \relates PipeAdiabatic */
typedef boost::optional<PipeAdiabatic> OptionalPipeAdiabatic;

/** \relates PipeAdiabatic */
typedef std::vector<PipeAdiabatic> PipeAdiabaticVector;

} // model
} // openstudio

#endif // MODEL_PIPEADIABATIC_HPP

