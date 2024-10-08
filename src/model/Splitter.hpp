/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    virtual ~Splitter() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Splitter(const Splitter& other) = default;
    Splitter(Splitter&& other) = default;
    Splitter& operator=(const Splitter&) = default;
    Splitter& operator=(Splitter&&) = default;

    /** Returns the inlet port to the splitter. */
    virtual unsigned inletPort() const;

    /** Returns the outlet port for branchIndex.  Branches consequtively
   *  indexed starting from 0.
   */
    virtual unsigned outletPort(unsigned branchIndex) const;

    /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
    virtual unsigned nextOutletPort() const;

    /** Returns the optional ModelObject connected to the inlet port.
   *  If there is no connected object then the optional will be false.
   */
    virtual boost::optional<ModelObject> inletModelObject() const;

    /** Returns the optional ModelObject connected to the branch designated by branchIndex.
   *  If there is no connected object then the optional will be false.
   */
    virtual boost::optional<ModelObject> outletModelObject(unsigned branchIndex) const;

    /** Returns the optional ModelObject connected to the last branch of the splitter.
   *  If there are no connections to the splitter's outlet ports, then the
   *  optional will be false.
   */
    virtual boost::optional<ModelObject> lastOutletModelObject() const;

    /** Returns a vector of all objects connected to the splitter's outlet ports.
   *  If no objects are connected to the splitter then an empty vector will be returned.
   */
    virtual std::vector<ModelObject> outletModelObjects() const;

    /** Returns a new port after the branch specified by branchIndex */
    virtual unsigned newOutletPortAfterBranch(unsigned branchIndex);

    /** Returns the branch index for the ModelObject specified by modelObject.
   *  The specified object must be connected to an outlet port of the splitter.
   */
    virtual unsigned branchIndexForOutletModelObject(ModelObject modelObject) const;

    /** Returns the index of the next available branch */
    virtual unsigned nextBranchIndex() const;

    /** Effectively disconnects anything connected to the outlet port
   *  at the specified branch index.  All branches after the specified
   *  branch index are moved to the next lower branch index, thereby
   *  removing any unconnected ports between branches.
   */
    virtual void removePortForBranch(unsigned branchIndex);

   protected:
    Splitter(IddObjectType type, const Model& model);

    using ImplType = detail::Splitter_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit Splitter(std::shared_ptr<ImplType> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Splitter");
  };

  using OptionalSplitter = boost::optional<Splitter>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SPLITTER_HPP
