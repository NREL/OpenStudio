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

#ifndef MODEL_MIXER_HPP
#define MODEL_MIXER_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {
  class Mixer_Impl;
};

class MODEL_API Mixer : public HVACComponent
{
  public:
  virtual ~Mixer() {}

  /** Returns the outlet port to the zone mixer. */
  virtual unsigned outletPort();

  /** Returns the inlet port for branchIndex.  Branches consequtively 
   *  indexed starting from 0. 
   */
  virtual unsigned inletPort(unsigned branchIndex);
  
  /** Returns the next available inlet port.  This will be the first port 
   *  with no connected objects */
  virtual unsigned nextInletPort();

  /** Returns the optional ModelObject connected to the outlet port.  
   *  If there is no connected object then the optional will be false. 
   */
  virtual boost::optional<ModelObject> outletModelObject();

  /** Returns the optional ModelObject connected to the branch designated by branchIndex. 
   *  If there is no connected object then the optional will be false.
   */
  virtual boost::optional<ModelObject> inletModelObject(unsigned branchIndex);

  /** Returns the optional ModelObject connected to the last branch of the mixer.
   *  If there are no connections to the mixer's inlet ports, then the 
   *  optional will be false.
   */
  virtual boost::optional<ModelObject> lastInletModelObject();

  /** Returns a vector of all objects connected to the mixer's inlet ports.
   *  If no objects are connected to the mixer then an empty vector will be returned.
   */
  virtual std::vector<ModelObject> inletModelObjects();

  /** Returns a new port after the branch specified by branchIndex */
  virtual unsigned newInletPortAfterBranch(unsigned branchIndex);
  
  /** Returns the branch index for the ModelObject specified by modelObject.
   *  The specified object must be connected to an inlet port of the mixer.
   */
  virtual unsigned branchIndexForInletModelObject( ModelObject modelObject );

  /** Returns the index of the next available branch */
  virtual unsigned nextBranchIndex();

  /** Effectively disconnects anything connected to the inlet port
   *  at the specified branch index.  All branches after the specified 
   *  branch index are moved to the next lower branch index, thereby 
   *  removing any unconnected ports between branches.
   */
  virtual void removePortForBranch(unsigned branchIndex);

  bool isRemovable() const;

  protected:

  Mixer(IddObjectType type,const Model& model);

  typedef detail::Mixer_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit Mixer(std::shared_ptr<ImplType> impl);

  private:

  REGISTER_LOGGER("openstudio.model.Mixer");

};

typedef boost::optional<Mixer> OptionalMixer;

} // model

} // openstudio

#endif // MODEL_MIXER_HPP

