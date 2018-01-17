/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  virtual unsigned outletPort() const;

  /** Returns the inlet port for branchIndex.  Branches consequtively
   *  indexed starting from 0.
   */
  virtual unsigned inletPort(unsigned branchIndex) const;

  /** Returns the next available inlet port.  This will be the first port
   *  with no connected objects */
  virtual unsigned nextInletPort() const;

  /** Returns the optional ModelObject connected to the outlet port.
   *  If there is no connected object then the optional will be false.
   */
  virtual boost::optional<ModelObject> outletModelObject() const;

  /** Returns the optional ModelObject connected to the branch designated by branchIndex.
   *  If there is no connected object then the optional will be false.
   */
  virtual boost::optional<ModelObject> inletModelObject(unsigned branchIndex) const;

  /** Returns the optional ModelObject connected to the last branch of the mixer.
   *  If there are no connections to the mixer's inlet ports, then the
   *  optional will be false.
   */
  virtual boost::optional<ModelObject> lastInletModelObject() const;

  /** Returns a vector of all objects connected to the mixer's inlet ports.
   *  If no objects are connected to the mixer then an empty vector will be returned.
   */
  virtual std::vector<ModelObject> inletModelObjects() const;

  /** Returns a new port after the branch specified by branchIndex */
  virtual unsigned newInletPortAfterBranch(unsigned branchIndex);

  /** Returns the branch index for the ModelObject specified by modelObject.
   *  The specified object must be connected to an inlet port of the mixer.
   */
  virtual unsigned branchIndexForInletModelObject( ModelObject modelObject ) const;

  /** Returns the index of the next available branch */
  virtual unsigned nextBranchIndex() const;

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

