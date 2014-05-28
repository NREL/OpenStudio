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
#ifndef MODEL_ZONEHVACCOMPONENT_IMPL_HPP
#define MODEL_ZONEHVACCOMPONENT_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {

namespace model {

class Node;
class ThermalZone;

namespace detail {

class MODEL_API ZoneHVACComponent_Impl : public ParentObject_Impl {

  public:

  ZoneHVACComponent_Impl( IddObjectType type, Model_Impl* model);

  ZoneHVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  ZoneHVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

  ZoneHVACComponent_Impl(const ZoneHVACComponent_Impl& other, Model_Impl* model, bool keepHandles);

  virtual ~ZoneHVACComponent_Impl() {}

  virtual boost::optional<ParentObject> parent() const;

  virtual std::vector<ModelObject> children() const;

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual ModelObject clone(Model model) const;

  virtual unsigned inletPort() = 0;

  virtual unsigned outletPort() = 0;

  boost::optional<Node> inletNode();

  boost::optional<Node> outletNode();

  virtual boost::optional<ThermalZone> thermalZone();

  virtual bool addToThermalZone(ThermalZone & thermalZone);

  virtual void removeFromThermalZone();

  std::vector<openstudio::IdfObject> remove();

 protected:

  friend class Model_Impl;

 private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACComponent");
};

} // detail

} // model

} // openstudio

#endif

