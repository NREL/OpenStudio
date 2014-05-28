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

#ifndef MODEL_CONNECTORSPLITTER_HPP
#define MODEL_CONNECTORSPLITTER_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {

namespace model {

namespace detail {
  class ConnectorSplitter_Impl;
} // detail

/** ConnectorSplitter is an interface to the EnergyPlus IDD object named "OS:Connector:Splitter"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of splitter objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the splitter.  Branch indexes
 *  are used to refer to the many outlet ports of the mixer
 */
class MODEL_API ConnectorSplitter : public Splitter
{
 public:

  /** Constructs a new Splitter object and places it inside the model. */
  explicit ConnectorSplitter(const Model& model);

  virtual ~ConnectorSplitter() {}

  /** Returns the inlet port to the splitter. */
  unsigned inletPort();

  /** Returns the outlet port for branchIndex.  Branches consequtively
   *  indexed starting from 0.
   */
  unsigned outletPort(unsigned branchIndex);

  /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
  unsigned nextOutletPort();

  virtual bool addToNode(Node & node);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  static IddObjectType iddObjectType();

 protected:

  typedef detail::ConnectorSplitter_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit ConnectorSplitter(std::shared_ptr<detail::ConnectorSplitter_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.ConnectorSplitter");
};

/** \relates ConnectorSplitter */
typedef boost::optional<ConnectorSplitter> OptionalConnectorSplitter;

} // model
} // openstudio

#endif // MODEL_CONNECTORSPLITTER_HPP

