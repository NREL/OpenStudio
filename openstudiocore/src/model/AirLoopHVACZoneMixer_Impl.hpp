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

#ifndef MODEL_AIRLOOPHVACZONEMIXER_IMPL_HPP
#define MODEL_AIRLOOPHVACZONEMIXER_IMPL_HPP

#include "Mixer_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail{

class MODEL_API AirLoopHVACZoneMixer_Impl : public Mixer_Impl 
{
  public:

  // constructor
  AirLoopHVACZoneMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  AirLoopHVACZoneMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                            Model_Impl* model, 
                            bool keepHandle);

  // copy constructor
  AirLoopHVACZoneMixer_Impl(const AirLoopHVACZoneMixer_Impl& other, 
                            Model_Impl* model, 
                            bool keepHandle);

  // virtual destructor
  virtual ~AirLoopHVACZoneMixer_Impl();

  // Get all output variable names that could be associated with this object.
  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  std::vector<openstudio::IdfObject> remove();

  unsigned outletPort();

  unsigned inletPort(unsigned branchIndex);
  
  unsigned nextInletPort();

  virtual bool addToNode(Node & node);

  virtual ModelObject clone(Model model) const;

  void disconnect();

 private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneMixer");

};

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACZONEMIXER_IMPL_HPP

