/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    virtual ~Mixer() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Mixer(const Mixer& other) = default;
    Mixer(Mixer&& other) = default;
    Mixer& operator=(const Mixer&) = default;
    Mixer& operator=(Mixer&&) = default;

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
    virtual unsigned branchIndexForInletModelObject(const ModelObject& modelObject) const;

    /** Returns the index of the next available branch */
    virtual unsigned nextBranchIndex() const;

    /** Effectively disconnects anything connected to the inlet port
   *  at the specified branch index.  All branches after the specified
   *  branch index are moved to the next lower branch index, thereby
   *  removing any unconnected ports between branches.
   */
    virtual void removePortForBranch(unsigned branchIndex);

   protected:
    Mixer(IddObjectType type, const Model& model);

    using ImplType = detail::Mixer_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit Mixer(std::shared_ptr<ImplType> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Mixer");
  };

  using OptionalMixer = boost::optional<Mixer>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_MIXER_HPP
