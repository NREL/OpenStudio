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
#ifndef MODEL_MIXER_IMPL_HPP
#define MODEL_MIXER_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail {

class MODEL_API Mixer_Impl : public HVACComponent_Impl
{
  public:

  Mixer_Impl(IddObjectType type, Model_Impl* model);

  Mixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  Mixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
             Model_Impl* model,
             bool keepHandle );

  Mixer_Impl( const Mixer_Impl& other, Model_Impl* model, bool keepHandles );

  virtual ~Mixer_Impl() {}

  virtual unsigned outletPort() = 0;

  virtual unsigned inletPort(unsigned branchIndex) = 0;

  virtual unsigned nextInletPort() = 0;

  virtual boost::optional<ModelObject> outletModelObject();

  virtual boost::optional<ModelObject> inletModelObject(unsigned branchIndex);

  virtual boost::optional<ModelObject> lastInletModelObject();

  virtual std::vector<ModelObject> inletModelObjects();

  virtual unsigned newInletPortAfterBranch(unsigned branchIndex);

  virtual unsigned branchIndexForInletModelObject( ModelObject modelObject );

  virtual unsigned nextBranchIndex();

  virtual void removePortForBranch(unsigned branchIndex);

  /** This function returns a vector of HVACComponent that are directly downstream
   *  from this object on an AirLoopHVAC or PlantLoop. 
   *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
  **/
  virtual std::vector<HVACComponent> edges(bool isDemandComponent);

  bool isRemovable() const;

  private:

  REGISTER_LOGGER("openstudio.model.Mixer");
};

} // detail

} // model 

} // openstudio

#endif

