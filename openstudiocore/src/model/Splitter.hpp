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

