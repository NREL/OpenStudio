/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "Loop.hpp"
#include "Loop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {
  Loop_Impl::Loop_Impl(IddObjectType type, Model_Impl* model)
    : ParentObject_Impl(type,model)
  {
  }

  Loop_Impl::Loop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
  }

  Loop_Impl::Loop_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
  }

  Loop_Impl::Loop_Impl(const Loop_Impl& other,
      Model_Impl* model,
      bool keepHandles)
    : ParentObject_Impl(other,model,keepHandles)
  {
  }

  const std::vector<std::string>& Loop_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  std::vector<openstudio::IdfObject> Loop_Impl::remove()
  {
    //ModelObjectVector modelObjects;
    //ModelObjectVector::iterator it;
    //modelObjects = this->supplyComponents();
    //for(it = modelObjects.begin();
    //    it != modelObjects.end();
    //    it++)
    //{
    //  if( it->optionalCast<Node>() ) continue;
    //  it->remove();
    //}
    //modelObjects = this->demandComponents();
    //for(it = modelObjects.begin();
    //    it != modelObjects.end();
    //    it++)
    //{
    //  it->remove();
    //}
    return ParentObject_Impl::remove();
  }

  // Recursive depth first search
  // start algorithm with one source node in the visited vector
  // searches all paths of nodes between the source and sink until a handle match is found
  boost::optional<ModelObject> findModelObject(const openstudio::Handle & handle, const HVACComponent & sink, std::vector<HVACComponent> & visited, bool isDemandComponents)
  {
    HVACComponent hvacComponent = visited.back();
    if( handle == hvacComponent.handle() ) {
      return hvacComponent;
    }

    boost::optional<HVACComponent> prev;
    if( visited.size() >= 2u ) prev = visited.rbegin()[1];

    std::vector<HVACComponent> nodes = hvacComponent.getImpl<HVACComponent_Impl>()->edges(prev);

    for(auto it = nodes.begin();
        it != nodes.end();
        it++)
    {
      // if it node has already been visited or node is sink then continue
      if( std::find(visited.begin(), visited.end(), *it) != visited.end() ||
          *it == sink )
      {
        continue;
      }
      visited.push_back(*it);
      boost::optional<ModelObject> foundHandle = findModelObject(handle, sink, visited, isDemandComponents);
      if( foundHandle ) { return *foundHandle; }
      visited.pop_back();
    }
    return boost::none;
  }

  OptionalModelObject Loop_Impl::component(openstudio::Handle handle)
  {
    boost::optional<ModelObject> supplyComp = this->supplyComponent(handle);
    if( supplyComp ) { return supplyComp; }
    return this->demandComponent(handle);
  }

  boost::optional<ModelObject> Loop_Impl::demandComponent(openstudio::Handle handle) const
  {
    auto inletComps = demandInletNodes();
    auto outletComp = demandOutletNode();

    if( handle == outletComp.handle() ) { return outletComp; }

    for( auto const & inletComp : inletComps ) {
      if( handle == inletComp.handle() ) { return inletComp; }
      std::vector<HVACComponent> visited { inletComp };
      if( auto mo = findModelObject(handle, outletComp, visited, true) ) {
        return mo;
      }
    }

    return boost::none;

  }

  boost::optional<ModelObject> Loop_Impl::supplyComponent(openstudio::Handle handle) const
  {
    auto inletComp = supplyInletNode();
    auto outletComps = supplyOutletNodes();

    if( handle == inletComp.handle() ) { return inletComp; }

    for( auto const & outletComp : outletComps ) {
      if( handle == outletComp.handle() ) { return outletComp; }
      std::vector<HVACComponent> visited { inletComp };
      if( auto mo = findModelObject(handle, outletComp, visited, false) ) {
        return mo;
      }
    }

    return boost::none;
  }

  ModelObject Loop_Impl::clone(Model model) const
  {
    return ParentObject_Impl::clone(model);
  }

  std::vector<ModelObject> Loop_Impl::children() const
  {
    return std::vector<ModelObject>();
  }

  std::vector<ModelObject> Loop_Impl::supplyComponents(std::vector<HVACComponent> inletComps,
      std::vector<HVACComponent> outletComps,
      openstudio::IddObjectType type
    ) const
  {
    LOG(Warn,"supplyComponents method for " << briefDescription() << " is not implemented to take multiple inlet and outlet components");
    return std::vector<ModelObject>();
  }

  std::vector<ModelObject> Loop_Impl::demandComponents(std::vector<HVACComponent> inletComps,
      std::vector<HVACComponent> outletComps,
      openstudio::IddObjectType type
    ) const
  {
    return std::vector<ModelObject>();
  }

  boost::optional<ModelObject> Loop_Impl::supplyInletNodeAsModelObject() {
    OptionalModelObject result = supplyInletNode();
    return result;
  }

  boost::optional<ModelObject> Loop_Impl::supplyOutletNodeAsModelObject() {
    OptionalModelObject result = supplyOutletNode();
    return result;
  }

  boost::optional<ModelObject> Loop_Impl::demandInletNodeAsModelObject() {
    OptionalModelObject result = demandInletNode();
    return result;
  }

  boost::optional<ModelObject> Loop_Impl::demandOutletNodeAsModelObject() {
    OptionalModelObject result = demandOutletNode();
    return result;
  }

  // Recursive depth first search
  // start algorithm with one source node in the visited vector
  // when complete, paths will be populated with all nodes between the source node and sink
  void findModelObjects(const HVACComponent & sink, std::vector<HVACComponent> & visited, std::vector<HVACComponent> & paths, bool isDemandComponents)
  {
    boost::optional<HVACComponent> prev;
    if( visited.size() >= 2u ) prev = visited.rbegin()[1];

    std::vector<HVACComponent> nodes = visited.back().getImpl<HVACComponent_Impl>()->edges(prev);

    for(const auto & node : nodes)
    {
      // if it node has already been visited then continue
      if( std::find(visited.begin(),visited.end(),node) != visited.end() )
      {
        continue;
      }
      if( node == sink )
      {
        visited.push_back(node);
        // Avoid pushing duplicate nodes into paths
        if( paths.empty() )
        {
          paths.insert(paths.end(), visited.begin(), visited.end());
        }
        else
        {
          for( const auto & visitedit : visited )
          {
            if( std::find(paths.begin(),paths.end(),visitedit) == paths.end() )
            {
              paths.push_back(visitedit);
            }
          }
        }
        visited.pop_back();
      }
    }

    for(const auto & node : nodes)
    {
      // if it node has already been visited or node is sink then continue
      if( std::find(visited.begin(),visited.end(),node) != visited.end() ||
          node == sink )
      {
        continue;
      }
      visited.push_back(node);
      findModelObjects(sink, visited, paths, isDemandComponents);
      visited.pop_back();
    }
  }

  std::vector<ModelObject> Loop_Impl::demandComponents( HVACComponent inletComp,
                                                        HVACComponent outletComp,
                                                        openstudio::IddObjectType type ) const
  {
    std::vector<HVACComponent> visited;
    visited.push_back(inletComp);
    std::vector<HVACComponent> allPaths;

    if( inletComp == outletComp ) {
      allPaths.push_back(inletComp);
    }
    else {
      findModelObjects(outletComp, visited, allPaths, true);
    }
    std::vector<ModelObject> _demandComponents = std::vector<ModelObject>(allPaths.begin(), allPaths.end());

    // Filter modelObjects for type
    if( type == IddObjectType::Catchall ) {
      return _demandComponents;
    }
    std::vector<ModelObject> reducedModelObjects;

    for(const auto & demandComponent : _demandComponents)
    {
      if( type == demandComponent.iddObject().type() )
      {
        reducedModelObjects.push_back(demandComponent);
      }
    }

    return reducedModelObjects;
  }

  template <typename T>
  struct Duplicate {
    bool operator()(const T& element) {
      return ! s_.insert(element).second; // true if element already in set
    }
   private:
    std::set<T> s_;
  };

  std::vector<ModelObject> Loop_Impl::supplyComponents(openstudio::IddObjectType type) const
  {
    std::vector<ModelObject> result;

    auto t_supplyInletNode = supplyInletNode();
    auto t_supplyOutletNodes = supplyOutletNodes();

    for( auto const & t_supplyOutletNode : t_supplyOutletNodes ) {
      auto components = supplyComponents( t_supplyInletNode,
                                          t_supplyOutletNode,
                                          type );
      result.insert(result.end(),components.begin(),components.end());
    }

    // If there is more than one outlet node (dual duct) we might have duplicates
    if( t_supplyOutletNodes.size() > 1u ) {
      Duplicate<ModelObject> pred;
      auto it = std::remove_if(result.begin(), result.end(), std::ref(pred));
      return std::vector<ModelObject>(result.begin(),it);
    } else {
      return result;
    }
  }

  std::vector<ModelObject> Loop_Impl::demandComponents(openstudio::IddObjectType type) const
  {
    std::vector<ModelObject> result;

    auto t_demandOutletNode = demandOutletNode();
    auto t_demandInletNodes = demandInletNodes();

    for( auto const & t_demandInletNode : t_demandInletNodes ) {
      auto components = demandComponents( t_demandInletNode,
                                          t_demandOutletNode,
                                          type );
      result.insert(result.end(),components.begin(),components.end());
    }

    // If there is more than one inlet node (dual duct) we might have duplicates
    if( t_demandInletNodes.size() > 1u ) {
      Duplicate<ModelObject> pred;
      auto it = std::remove_if(result.begin(), result.end(), std::ref(pred));
      return std::vector<ModelObject>(result.begin(),it);
    } else {
      return result;
    }
  }

  std::vector<ModelObject> Loop_Impl::components(openstudio::IddObjectType type)
  {
    std::vector<ModelObject> result;
    result = this->supplyComponents( type );

    std::vector<ModelObject> demandComponents = this->demandComponents( type );

    result.insert(result.end(),demandComponents.begin(),demandComponents.end());

    return result;
  }

  std::vector<ModelObject> Loop_Impl::supplyComponents( HVACComponent inletComp,
                                                        HVACComponent outletComp,
                                                        openstudio::IddObjectType type) const
  {
    std::vector<HVACComponent> visited;
    visited.push_back(inletComp);
    std::vector<HVACComponent> allPaths;

    if( inletComp == outletComp ) {
      allPaths.push_back(inletComp);
    }
    else {
      findModelObjects(outletComp, visited, allPaths, false);
    }
    std::vector<ModelObject> _supplyComponents = std::vector<ModelObject>(allPaths.begin(), allPaths.end());

    // Filter modelObjects for type
    if( type == IddObjectType::Catchall ) {
      return _supplyComponents;
    }
    std::vector<ModelObject> reducedModelObjects;

    for(const auto & supplyComponent : _supplyComponents)
    {
      if( type == supplyComponent.iddObject().type() )
      {
        reducedModelObjects.push_back(supplyComponent);
      }
    }

    return reducedModelObjects;
  }

  std::vector<ModelObject> Loop_Impl::components(HVACComponent inletComp,
                                                 HVACComponent outletComp,
                                                 openstudio::IddObjectType type)
  {
    if( ( supplyComponent(inletComp.handle()) && supplyComponent(outletComp.handle()) ) )
    {
      return supplyComponents(inletComp,outletComp,type);
    }
    else if( ( demandComponent(inletComp.handle()) && demandComponent(outletComp.handle()) ) )
    {
      return demandComponents(inletComp,outletComp,type);
    }

    return std::vector<ModelObject>();
  }

} // detail

Loop::Loop(IddObjectType type,const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::Loop_Impl>());
}

Loop::Loop(std::shared_ptr<detail::Loop_Impl> p)
  : ParentObject(std::move(p))
{}

std::vector<IdfObject> Loop::remove()
{
  return getImpl<detail::Loop_Impl>()->remove();
}

std::vector<ModelObject> Loop::components(openstudio::IddObjectType type)
{
  return getImpl<detail::Loop_Impl>()->components( type );
}

std::vector<ModelObject> Loop::components(HVACComponent inletComp,
                                          HVACComponent outletComp,
                                          openstudio::IddObjectType type)
{
  return getImpl<detail::Loop_Impl>()->components( inletComp, outletComp, type);
}

boost::optional<ModelObject> Loop::component(openstudio::Handle handle)
{
  return getImpl<detail::Loop_Impl>()->component( handle );
}

std::vector<ModelObject> Loop::supplyComponents(std::vector<HVACComponent> inletComps,
    std::vector<HVACComponent> outletComps,
    openstudio::IddObjectType type
  ) const
{
  return getImpl<detail::Loop_Impl>()->supplyComponents( inletComps, outletComps, type);
}

std::vector<ModelObject> Loop::supplyComponents(HVACComponent inletComp,
                                                HVACComponent outletComp,
                                                openstudio::IddObjectType type) const
{
  return getImpl<detail::Loop_Impl>()->supplyComponents( inletComp, outletComp, type);
}

std::vector<ModelObject> Loop::supplyComponents(openstudio::IddObjectType type) const
{
  return getImpl<detail::Loop_Impl>()->supplyComponents(type);
}

std::vector<ModelObject> Loop::demandComponents(std::vector<HVACComponent> inletComps,
    std::vector<HVACComponent> outletComps,
    openstudio::IddObjectType type
  ) const
{
  return getImpl<detail::Loop_Impl>()->demandComponents( inletComps, outletComps, type);
}

std::vector<ModelObject> Loop::demandComponents( HVACComponent inletComp,
                                                 HVACComponent outletComp,
                                                 openstudio::IddObjectType type ) const
{
  return getImpl<detail::Loop_Impl>()->demandComponents( inletComp, outletComp, type);
}

std::vector<ModelObject> Loop::demandComponents(openstudio::IddObjectType type) const
{
  return getImpl<detail::Loop_Impl>()->demandComponents(type);
}

ModelObject Loop::clone(Model model) const
{
  return getImpl<detail::Loop_Impl>()->clone( model );
}

std::vector<ModelObject> Loop::children() const
{
  return getImpl<detail::Loop_Impl>()->children();
}

boost::optional<ModelObject> Loop::demandComponent(openstudio::Handle handle) const
{
  return getImpl<detail::Loop_Impl>()->demandComponent( handle );
}

boost::optional<ModelObject> Loop::supplyComponent(openstudio::Handle handle)
{
  return getImpl<detail::Loop_Impl>()->supplyComponent( handle );
}

Node Loop::supplyInletNode() const
{
  return getImpl<detail::Loop_Impl>()->supplyInletNode();
}

Node Loop::supplyOutletNode() const
{
  return getImpl<detail::Loop_Impl>()->supplyOutletNode();
}

std::vector<Node> Loop::supplyOutletNodes() const
{
  return getImpl<detail::Loop_Impl>()->supplyOutletNodes();
}

Node Loop::demandInletNode() const
{
  return getImpl<detail::Loop_Impl>()->demandInletNode();
}

std::vector<Node> Loop::demandInletNodes() const
{
  return getImpl<detail::Loop_Impl>()->demandInletNodes();
}

Node Loop::demandOutletNode() const
{
  return getImpl<detail::Loop_Impl>()->demandOutletNode();
}

Splitter Loop::demandSplitter() const
{
  return getImpl<detail::Loop_Impl>()->demandSplitter();
}

Mixer Loop::demandMixer() const
{
  return getImpl<detail::Loop_Impl>()->demandMixer();
}

} // model

} // openstudio

