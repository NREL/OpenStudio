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

#ifndef MODEL_PORTLIST_HPP
#define MODEL_PORTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class Node;

class ThermalZone;

class HVACComponent;

namespace detail {

  class PortList_Impl;

  class ThermalZone_Impl;

} // detail

/** PortList is a ModelObject that wraps the OpenStudio IDD object 'OS:PortList'. */
class MODEL_API PortList : public ModelObject {
  public:

  explicit PortList(const HVACComponent& comp);

  virtual ~PortList() {}

  static IddObjectType iddObjectType();

  /** Returns the port for portIndex.  Ports are consequtively 
   *  indexed starting from 0. 
   */
  unsigned port(unsigned portIndex);

  /** Given a port, return the port index.
   *  Consider that a "port" is simply an unsigned, so 
   *  port and port index share the same type, but they do not share the same value.
   */
  unsigned portIndex(unsigned port);
  
  /** Returns the next available port.  This will be the first port 
   *  with no connected objects */
  unsigned nextPort();

  /** Returns the optional ModelObject connected to the port designated by portIndex. 
   *  If there is no connected object then the optional will be false.
   */
  boost::optional<ModelObject> modelObject(unsigned portIndex);

  /** Returns the optional ModelObject connected to the last port of the PortList.
   *  If there are no connections to the list's outlet ports, then the 
   *  optional will be false.
   */
  boost::optional<ModelObject> lastModelObject();

  /** Returns a vector of all objects connected to the PortList's ports.
   *  If no objects are connected to the PortList then an empty vector will be returned.
   */
  std::vector<ModelObject> modelObjects();
  
  /** Returns the port index for the ModelObject specified by modelObject.
   */
  unsigned portIndexForModelObject( ModelObject & modelObject );

  /** Returns the index of the next available port */
  unsigned nextPortIndex();

  ThermalZone thermalZone() const;

  /** Returns the port connected to an AirLoopHVAC object.
   *  If there is no AirLoopHVAC object, then the next available port is returned
   */
  unsigned airLoopHVACPort();

  /** Returns the object connected to an AirLoopHVAC object. */
  boost::optional<ModelObject> airLoopHVACModelObject();

  protected:

  /// @cond
  typedef detail::PortList_Impl ImplType;

  explicit PortList(std::shared_ptr<detail::PortList_Impl> impl);

  friend class detail::PortList_Impl;
  friend class Model;
  friend class IdfObject;
  friend class ThermalZone;
  friend class detail::ThermalZone_Impl;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.PortList");
};

/** \relates PortList*/
typedef boost::optional<PortList> OptionalPortList;

/** \relates PortList*/
typedef std::vector<PortList> PortListVector;

} // model
} // openstudio

#endif // MODEL_PORTLIST_HPP

