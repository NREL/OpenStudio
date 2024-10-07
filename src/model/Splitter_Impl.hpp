/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

      Splitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Splitter_Impl(const Splitter_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Splitter_Impl() override = default;

      /** Returns the inlet port to the splitter. */
      virtual unsigned inletPort() const = 0;

      /** Returns the outlet port for branchIndex.  Branches consequtively
   *  indexed starting from 0.
   */
      virtual unsigned outletPort(unsigned branchIndex) const = 0;

      /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
      virtual unsigned nextOutletPort() const = 0;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      /** Returns the optional ModelObject connected to the inlet port.
   *  If there is no connected object then the optional will be false.
   */
      boost::optional<ModelObject> inletModelObject() const;

      /** Returns the optional ModelObject connected to the branch designated by branchIndex.
   *  If there is no connected object then the optional will be false.
   */
      boost::optional<ModelObject> outletModelObject(unsigned branchIndex) const;

      /** Returns the optional ModelObject connected to the last branch of the splitter.
   *  If there are no connections to the splitter's outlet ports, then the
   *  optional will be false.
   */
      boost::optional<ModelObject> lastOutletModelObject() const;

      /** Returns a vector of all objects connected to the splitter's outlet ports.
   *  If no objects are connected to the splitter then an empty vector will be returned.
   */
      std::vector<ModelObject> outletModelObjects() const;

      /** Returns a new port after the branch specified by branchIndex */
      unsigned newOutletPortAfterBranch(unsigned branchIndex);

      /** Returns the branch index for the ModelObject specified by modelObject.
   *  The specified object must be connected to an outlet port of the splitter.
   */
      unsigned branchIndexForOutletModelObject(ModelObject modelObject) const;

      /** Returns the index of the next available branch */
      unsigned nextBranchIndex() const;

      /** Effectively disconnects anything connected to the outlet port
   *  at the specified branch index.  All branches after the specified
   *  branch index are moved to the next lower branch index, thereby
   *  removing any unconnected ports between branches.
   */
      void removePortForBranch(unsigned branchIndex);

      bool isRemovable() const override;

      void disconnect() override;

     private:
      REGISTER_LOGGER("openstudio.model.Splitter");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
