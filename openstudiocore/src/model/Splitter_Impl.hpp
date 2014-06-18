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
#ifndef MODEL_SPLITTER_IMPL_HPP
#define MODEL_SPLITTER_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail {

class MODEL_API Splitter_Impl : public HVACComponent_Impl
{
  public:

  Splitter_Impl(IddObjectType type, Model_Impl* model);

  Splitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  Splitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                Model_Impl* model,
                bool keepHandle);

  Splitter_Impl(const Splitter_Impl& other, Model_Impl* model, bool keepHandles);

  virtual ~Splitter_Impl() {} 

  /** Returns the inlet port to the splitter. */
  virtual unsigned inletPort() = 0;

  /** Returns the outlet port for branchIndex.  Branches consequtively 
   *  indexed starting from 0. 
   */
  virtual unsigned outletPort(unsigned branchIndex) = 0;
  
  /** Returns the next available outlet port.  This will be the first port 
   *  with no connected objects */
  virtual unsigned nextOutletPort() = 0;

  /** This function returns a vector of HVACComponent that are directly downstream
   *  from this object on an AirLoopHVAC or PlantLoop. 
   *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
  **/
  virtual std::vector<HVACComponent> edges(bool isDemandComponent);

  /** Returns the optional ModelObject connected to the inlet port.  
   *  If there is no connected object then the optional will be false. 
   */
  boost::optional<ModelObject> inletModelObject();

  /** Returns the optional ModelObject connected to the branch designated by branchIndex. 
   *  If there is no connected object then the optional will be false.
   */
  boost::optional<ModelObject> outletModelObject(unsigned branchIndex);

  /** Returns the optional ModelObject connected to the last branch of the splitter.
   *  If there are no connections to the splitter's outlet ports, then the 
   *  optional will be false.
   */
  boost::optional<ModelObject> lastOutletModelObject();

  /** Returns a vector of all objects connected to the splitter's outlet ports.
   *  If no objects are connected to the splitter then an empty vector will be returned.
   */
  std::vector<ModelObject> outletModelObjects();

  /** Returns a new port after the branch specified by branchIndex */
  unsigned newOutletPortAfterBranch(unsigned branchIndex);
  
  /** Returns the branch index for the ModelObject specified by modelObject.
   *  The specified object must be connected to an outlet port of the splitter.
   */
  unsigned branchIndexForOutletModelObject( ModelObject modelObject );

  /** Returns the index of the next available branch */
  unsigned nextBranchIndex();

  /** Effectively disconnects anything connected to the outlet port
   *  at the specified branch index.  All branches after the specified 
   *  branch index are moved to the next lower branch index, thereby 
   *  removing any unconnected ports between branches.
   */
  void removePortForBranch(unsigned branchIndex);

  bool isRemovable() const;

  private:

  REGISTER_LOGGER("openstudio.model.Splitter");
};

} // detail

} // model 

} // openstudio

#endif

