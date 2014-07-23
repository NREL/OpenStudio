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

#ifndef MODEL_CONNECTORMIXER_HPP
#define MODEL_CONNECTORMIXER_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

namespace detail {
  class ConnectorMixer_Impl;
} // detail

/** Mixer is an interface to the IDD object named "OS:Connector:Mixer"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of mixer objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the mixer.  Branch indexes
 *  are used to refer to the many inlet ports of the mixer
 */
class MODEL_API ConnectorMixer : public Mixer {
  public:

  /** Constructs a new Mixer object and places it inside the model. */
  explicit ConnectorMixer(const Model& model);

  virtual ~ConnectorMixer() {}

  unsigned outletPort();

  unsigned inletPort(unsigned branchIndex);

  unsigned nextInletPort();

  virtual bool addToNode(Node & node);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  static IddObjectType iddObjectType();

  protected:

  typedef detail::ConnectorMixer_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit ConnectorMixer(std::shared_ptr<detail::ConnectorMixer_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.ConnectorMixer");

  ConnectorMixer(const Handle& handle, const Model& model);

};

/** \relates ConnectorMixer */
typedef boost::optional<ConnectorMixer> OptionalConnectorMixer;

} // model

} // openstudio

#endif // MODEL_CONNECTORMIXER_HPP

