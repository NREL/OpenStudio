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

#ifndef MODEL_SPLITTER_HPP
#define MODEL_SPLITTER_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {
  class Splitter_Impl;
};

class MODEL_API Splitter : public HVACComponent
{
  public:
  virtual ~Splitter() {}

  /** Returns the inlet port to the splitter. */
  virtual unsigned inletPort();

  /** Returns the outlet port for branchIndex.  Branches consequtively 
   *  indexed starting from 0. 
   */
  virtual unsigned outletPort(unsigned branchIndex);
  
  /** Returns the next available outlet port.  This will be the first port 
   *  with no connected objects */
  virtual unsigned nextOutletPort();

  /** Returns the optional ModelObject connected to the inlet port.  
   *  If there is no connected object then the optional will be false. 
   */
  virtual boost::optional<ModelObject> inletModelObject();

  /** Returns the optional ModelObject connected to the branch designated by branchIndex. 
   *  If there is no connected object then the optional will be false.
   */
  virtual boost::optional<ModelObject> outletModelObject(unsigned branchIndex);

  /** Returns the optional ModelObject connected to the last branch of the splitter.
   *  If there are no connections to the splitter's outlet ports, then the 
   *  optional will be false.
   */
  virtual boost::optional<ModelObject> lastOutletModelObject();

  /** Returns a vector of all objects connected to the splitter's outlet ports.
   *  If no objects are connected to the splitter then an empty vector will be returned.
   */
  virtual std::vector<ModelObject> outletModelObjects();

  /** Returns a new port after the branch specified by branchIndex */
  virtual unsigned newOutletPortAfterBranch(unsigned branchIndex);
  
  /** Returns the branch index for the ModelObject specified by modelObject.
   *  The specified object must be connected to an outlet port of the splitter.
   */
  virtual unsigned branchIndexForOutletModelObject( ModelObject modelObject );

  /** Returns the index of the next available branch */
  virtual unsigned nextBranchIndex();

  /** Effectively disconnects anything connected to the outlet port
   *  at the specified branch index.  All branches after the specified 
   *  branch index are moved to the next lower branch index, thereby 
   *  removing any unconnected ports between branches.
   */
  virtual void removePortForBranch(unsigned branchIndex);

  bool isRemovable() const;

  protected:

  Splitter(IddObjectType type,const Model& model);

  typedef detail::Splitter_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit Splitter(std::shared_ptr<ImplType> impl);

  private:

  REGISTER_LOGGER("openstudio.model.Splitter");

};

typedef boost::optional<Splitter> OptionalSplitter;

} // model

} // openstudio

#endif // MODEL_SPLITTER_HPP

