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

#ifndef MODEL_CONNECTORMIXER_IMPL_HPP
#define MODEL_CONNECTORMIXER_IMPL_HPP

#include "Mixer_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail {

class MODEL_API ConnectorMixer_Impl : public Mixer_Impl 
{
 public:

  ConnectorMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  ConnectorMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                            Model_Impl* model, 
                            bool keepHandle);

  ConnectorMixer_Impl(const ConnectorMixer_Impl& other, 
                            Model_Impl* model, 
                            bool keepHandle);

  virtual ~ConnectorMixer_Impl();

  virtual const std::vector<std::string> & outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  std::vector<openstudio::IdfObject> remove() override;

  unsigned outletPort() const override;

  unsigned inletPort(unsigned branchIndex) const override;
  
  unsigned nextInletPort() const override;

  virtual bool addToNode(Node & node) override;

  virtual ModelObject clone(Model model) const override;

  private:

  REGISTER_LOGGER("openstudio.model.ConnectorMixer");

};

} // detail

} // model

} // openstudio

#endif // MODEL_CONNECTORMIXER_IMPL_HPP
