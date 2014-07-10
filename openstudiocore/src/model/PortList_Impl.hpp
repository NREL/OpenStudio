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

#ifndef MODEL_PORTLIST_IMPL_HPP
#define MODEL_PORTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

class Node;

class ThermalZone;

class HVACComponent;

namespace detail {

/** PortList_Impl is a ModelObject_Impl that is the implementation class for PortList.*/
class MODEL_API PortList_Impl : public ModelObject_Impl {
 public:

  PortList_Impl(const IdfObject& idfObject,
                Model_Impl* model,
                bool keepHandle);

  PortList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                Model_Impl* model,
                bool keepHandle);

  PortList_Impl(const PortList_Impl& other,
                Model_Impl* model,
                bool keepHandle);

  virtual ~PortList_Impl() {}

  unsigned port(unsigned portIndex);

  unsigned portIndex(unsigned port);
  
  unsigned nextPort();

  boost::optional<ModelObject> modelObject(unsigned portIndex);

  boost::optional<ModelObject> lastModelObject();

  std::vector<ModelObject> modelObjects();

  unsigned newPortAfterIndex(unsigned portIndex);
  
  unsigned portIndexForModelObject( ModelObject & modelObject, bool * ok = nullptr );

  unsigned nextPortIndex();

  void removePort(unsigned port);

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  ThermalZone thermalZone() const;

  std::vector<openstudio::IdfObject> remove();

  unsigned airLoopHVACPort();

  boost::optional<ModelObject> airLoopHVACModelObject();

  // Return the HVACComponent that the port list is associated with.
  HVACComponent hvacComponent() const;

  // Set the hvacComponent that the port list is associated with.
  bool setHVACComponent(const HVACComponent & hvacComponent);

  protected:

  private:

  REGISTER_LOGGER("openstudio.model.PortList");
};

} // detail

} // model
} // openstudio

#endif // MODEL_PORTLIST_IMPL_HPP

