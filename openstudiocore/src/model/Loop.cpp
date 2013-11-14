/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <model/Loop.hpp>
#include <model/Loop_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>
#include <model/Mixer.hpp>
#include <model/Mixer_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/WaterToWaterComponent.hpp>
#include <model/WaterToWaterComponent_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/Model.hpp>

#include <utilities/core/Assert.hpp>

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

  OptionalModelObject Loop_Impl::component(openstudio::Handle handle)
  {
    OptionalModelObject result;

    ModelObjectVector allComponents = components();
    ModelObjectVector::iterator it;
    for( it = allComponents.begin();
         it != allComponents.end();
         it++ )
    {
      if( it->handle() == handle )
      {
        return OptionalModelObject(*it);
      }
    }
    return result;
  }

  boost::optional<ModelObject> Loop_Impl::demandComponent(openstudio::Handle handle)
  {
    OptionalModelObject result;

    ModelObjectVector allComponents = demandComponents();
    ModelObjectVector::iterator it;
    for( it = allComponents.begin();
         it != allComponents.end();
         it++ )
    {
      if( it->handle() == handle )
      {
        return OptionalModelObject(*it);
      }
    }
    return result;
  }

  boost::optional<ModelObject> Loop_Impl::supplyComponent(openstudio::Handle handle)
  {
    OptionalModelObject result;

    ModelObjectVector allComponents = supplyComponents();
    ModelObjectVector::iterator it;
    for( it = allComponents.begin();
         it != allComponents.end();
         it++ )
    {
      if( it->handle() == handle )
      {
        return OptionalModelObject(*it);
      }
    }
    return result;
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
    )
  {
    return std::vector<ModelObject>();
  }

  std::vector<ModelObject> Loop_Impl::demandComponents(std::vector<HVACComponent> inletComps, 
      std::vector<HVACComponent> outletComps,
      openstudio::IddObjectType type
    )
  {
    return std::vector<ModelObject>();
  }

  Node Loop_Impl::supplyInletNode() const
  {
    return Node(model());
  }

  Node Loop_Impl::supplyOutletNode() const
  {
    return Node(model());
  }

  Node Loop_Impl::demandInletNode() const
  {
    return Node(model());
  }

  Node Loop_Impl::demandOutletNode() const
  {
    return Node(model());
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

  std::vector<ModelObject> Loop_Impl::demandComponents( HVACComponent inletComp,
                                                        HVACComponent outletComp,
                                                        openstudio::IddObjectType type )
  {
    std::vector<ModelObject> modelObjects;

    OptionalModelObject modelObject(inletComp);

    bool outletNodeFound = false;

    bool isPlantLoop = false;

    if( this->iddObject().type() == IddObjectType::OS_PlantLoop )
    {
      isPlantLoop = true;
    }

    while(modelObject)
    {
      if(OptionalNode node = modelObject->optionalCast<Node>())
      {
        modelObjects.push_back(*node);
        if( *node == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = node->outletModelObject();
        }
      }
      else if(OptionalStraightComponent comp = modelObject->optionalCast<StraightComponent>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = comp->outletModelObject();
        }
      }
      else if(OptionalPortList comp = modelObject->optionalCast<PortList>())
      {
        if( boost::optional<ThermalZone> tz = comp->thermalZone() )
        {
          modelObjects.push_back(*tz);
          if( *tz == outletComp )
          {
            outletNodeFound = true;
            break;
          }
          else
          {
            modelObject = tz->returnAirModelObject();
          }
        }
      }
      else if(OptionalMixer mixer = modelObject->optionalCast<Mixer>())
      {
        modelObjects.push_back(*mixer);
        if( *mixer == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = mixer->outletModelObject();
        }
      }
      else if(OptionalWaterToAirComponent comp = modelObject->optionalCast<WaterToAirComponent>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          if( isPlantLoop )
          {
            modelObject = comp->waterOutletModelObject();
          }
          else
          {
            modelObject = comp->airOutletModelObject();
          }
        }
      }
      else if(OptionalWaterToWaterComponent comp = modelObject->optionalCast<WaterToWaterComponent>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = comp->demandOutletModelObject();
        }
      }
      else if(OptionalSplitter splitter = modelObject->optionalCast<Splitter>())
      {
        modelObject = boost::none;
  
        std::vector<ModelObject> allBranchModelObjects;
        std::vector<ModelObject> thisBranchModelObjects;

        modelObjects.push_back(*splitter);
        if( *splitter == outletComp )
        {
          outletNodeFound = true;
          break;
        }

        std::vector<ModelObject> outletModelObjects = splitter->outletModelObjects();

        for( std::vector<ModelObject>::iterator it = outletModelObjects.begin();
             it != outletModelObjects.end();
             it++ )
        {
          thisBranchModelObjects.clear();
          OptionalModelObject branchObject = OptionalModelObject(*it);

          while(branchObject)
          {
            if( OptionalNode node = branchObject->optionalCast<Node>() )
            {
              thisBranchModelObjects.push_back( node.get() );
              allBranchModelObjects.push_back( node.get() );
              if( *node == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = node->outletModelObject();
              }
            }
            else if(OptionalStraightComponent comp = branchObject->optionalCast<StraightComponent>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = comp->outletModelObject();
              }
            }
            else if(OptionalPortList comp = branchObject->optionalCast<PortList>())
            {
              if( boost::optional<ThermalZone> tz = comp->thermalZone() )
              {
                thisBranchModelObjects.push_back( tz.get() );
                allBranchModelObjects.push_back( tz.get() );
                if( *tz == outletComp )
                {
                  outletNodeFound = true;
                  break;
                }
                else
                {
                  branchObject = tz->returnAirModelObject();
                }
              }
            }
            else if(OptionalWaterToAirComponent comp = branchObject->optionalCast<WaterToAirComponent>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                if( isPlantLoop )
                {
                  branchObject = comp->waterOutletModelObject();
                }
                else
                {
                  branchObject = comp->airOutletModelObject();
                }
              }
            }
            else if(OptionalWaterToWaterComponent comp = branchObject->optionalCast<WaterToWaterComponent>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = comp->demandOutletModelObject();
              }
            }
            else if(OptionalMixer mixer = branchObject->optionalCast<Mixer>() )
            {
              modelObject = mixer;
              break;
            }
            else
            {
              break;

              // Log unhandled component
            }
          }
          if( outletNodeFound ) { break; }
        }
        if( outletNodeFound )
        {
          modelObjects.insert( modelObjects.end(),thisBranchModelObjects.begin(), thisBranchModelObjects.end() );

          break;
        }
        else
        {
          modelObjects.insert( modelObjects.end(),allBranchModelObjects.begin(), allBranchModelObjects.end() );
        }
      }
      else
      {
        modelObject = boost::none;

        break;

        // Log unhandled component
      }
    }

    // Filter modelObjects for type
    if( type != IddObjectType::Catchall )
    {
      std::vector<ModelObject> reducedModelObjects;
      std::vector<ModelObject>::iterator it;

      for(it = modelObjects.begin();
          it != modelObjects.end();
          it++)
      {
        if(it->iddObject().type() == type)
        {
          reducedModelObjects.push_back(*it);
        }
      }
      modelObjects = reducedModelObjects;
    }

    if( outletNodeFound )
    {
      return modelObjects;
    }
    else
    {
      return std::vector<ModelObject>();
    }
  }

  std::vector<ModelObject> Loop_Impl::supplyComponents(openstudio::IddObjectType type)
  {
    return supplyComponents( supplyInletNode(),
                             supplyOutletNode(),
                             type );
  }

  std::vector<ModelObject> Loop_Impl::demandComponents(openstudio::IddObjectType type)
  {
    return demandComponents( demandInletNode(),
                             demandOutletNode(),
                             type );
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
                                                        openstudio::IddObjectType type)
  {
    std::vector<ModelObject> modelObjects;

    OptionalModelObject modelObject(inletComp);

    bool outletNodeFound = false;

    bool isPlantLoop = false;

    if( this->iddObject().type() == IddObjectType::OS_PlantLoop )
    {
      isPlantLoop = true;
    }

    while(modelObject)
    {
      if(OptionalNode node = modelObject->optionalCast<Node>())
      {
        modelObjects.push_back(*node);
        if( *node == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = node->outletModelObject();
        }
      }
      else if(OptionalStraightComponent comp = modelObject->optionalCast<StraightComponent>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = comp->outletModelObject();
        }
      }
      else if(OptionalThermalZone comp = modelObject->optionalCast<ThermalZone>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = comp->returnAirModelObject();
        }
      }
      else if(OptionalMixer mixer = modelObject->optionalCast<Mixer>())
      {
        modelObjects.push_back(*mixer);
        if( *mixer == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = mixer->outletModelObject();
        }
      }
      else if(OptionalWaterToAirComponent comp = modelObject->optionalCast<WaterToAirComponent>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          if( isPlantLoop )
          {
            modelObject = comp->waterOutletModelObject();
          }
          else
          {
            modelObject = comp->airOutletModelObject();
          }
        }
      }
      else if(OptionalWaterToWaterComponent comp = modelObject->optionalCast<WaterToWaterComponent>())
      {
        modelObjects.push_back(*comp);
        if( *comp == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = comp->supplyOutletModelObject();
        }
      }
      else if(OptionalAirLoopHVACOutdoorAirSystem mixer = modelObject->optionalCast<AirLoopHVACOutdoorAirSystem>())
      {
        modelObjects.push_back(*mixer);
        if( *mixer == outletComp )
        {
          outletNodeFound = true;
          break;
        }
        else
        {
          modelObject = mixer->mixedAirModelObject();
        }
      }
      else if(OptionalSplitter splitter = modelObject->optionalCast<Splitter>())
      {
        modelObject = boost::none;
  
        std::vector<ModelObject> allBranchModelObjects;
        std::vector<ModelObject> thisBranchModelObjects;

        modelObjects.push_back(*splitter);
        if( *splitter == outletComp )
        {
          outletNodeFound = true;
          break;
        }

        std::vector<ModelObject> outletModelObjects = splitter->outletModelObjects();

        for( std::vector<ModelObject>::iterator it = outletModelObjects.begin();
             it != outletModelObjects.end();
             it++ )
        {
          thisBranchModelObjects.clear();
          OptionalModelObject branchObject = OptionalModelObject(*it);

          while(branchObject)
          {
            if( OptionalNode node = branchObject->optionalCast<Node>() )
            {
              thisBranchModelObjects.push_back( node.get() );
              allBranchModelObjects.push_back( node.get() );
              if( *node == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = node->outletModelObject();
              }
            }
            else if(OptionalStraightComponent comp = branchObject->optionalCast<StraightComponent>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = comp->outletModelObject();
              }
            }
            else if(OptionalThermalZone comp = branchObject->optionalCast<ThermalZone>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = comp->returnAirModelObject();
              }
            }
            else if(OptionalWaterToAirComponent comp = branchObject->optionalCast<WaterToAirComponent>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                if( isPlantLoop )
                {
                  branchObject = comp->waterOutletModelObject();
                }
                else
                {
                  branchObject = comp->airOutletModelObject();
                }
              }
            }
            else if(OptionalWaterToWaterComponent comp = branchObject->optionalCast<WaterToWaterComponent>())
            {
              thisBranchModelObjects.push_back( comp.get() );
              allBranchModelObjects.push_back( comp.get() );
              if( *comp == outletComp )
              {
                outletNodeFound = true;
                break;
              }
              else
              {
                branchObject = comp->supplyOutletModelObject();
              }
            }
            else if(OptionalMixer mixer = branchObject->optionalCast<Mixer>() )
            {
              modelObject = mixer;
              break;
            }
            else
            {
              break;

              // Log unhandled component
            }
          }
          if( outletNodeFound ) { break; }
        }
        if( outletNodeFound )
        {
          modelObjects.insert( modelObjects.end(),thisBranchModelObjects.begin(), thisBranchModelObjects.end() );

          break;
        }
        else
        {
          modelObjects.insert( modelObjects.end(),allBranchModelObjects.begin(), allBranchModelObjects.end() );
        }
      }
      else
      {
        modelObject = boost::none;

        break;

        // Log unhandled component
      }
    }

    // Filter modelObjects for type
    if( type != IddObjectType::Catchall )
    {
      std::vector<ModelObject> reducedModelObjects;
      std::vector<ModelObject>::iterator it;

      for(it = modelObjects.begin();
          it != modelObjects.end();
          it++)
      {
        if(it->iddObject().type() == type)
        {
          reducedModelObjects.push_back(*it);
        }
      }
      modelObjects = reducedModelObjects;
    }

    if( outletNodeFound )
    {
      return modelObjects;
    }
    else
    {
      return std::vector<ModelObject>();
    }
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

Loop::Loop(boost::shared_ptr<detail::Loop_Impl> p)
  : ParentObject(p)
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
  )
{
  return getImpl<detail::Loop_Impl>()->supplyComponents( inletComps, outletComps, type);
}

std::vector<ModelObject> Loop::supplyComponents(HVACComponent inletComp, 
                                                HVACComponent outletComp, 
                                                openstudio::IddObjectType type)
{
  return getImpl<detail::Loop_Impl>()->supplyComponents( inletComp, outletComp, type);
}
  
std::vector<ModelObject> Loop::supplyComponents(openstudio::IddObjectType type)
{
  return getImpl<detail::Loop_Impl>()->supplyComponents(type);
}

std::vector<ModelObject> Loop::demandComponents(std::vector<HVACComponent> inletComps, 
    std::vector<HVACComponent> outletComps,
    openstudio::IddObjectType type
  )
{
  return getImpl<detail::Loop_Impl>()->demandComponents( inletComps, outletComps, type);
}

std::vector<ModelObject> Loop::demandComponents( HVACComponent inletComp, 
                                                 HVACComponent outletComp, 
                                                 openstudio::IddObjectType type )
{
  return getImpl<detail::Loop_Impl>()->demandComponents( inletComp, outletComp, type);
}
  
std::vector<ModelObject> Loop::demandComponents(openstudio::IddObjectType type)
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

boost::optional<ModelObject> Loop::demandComponent(openstudio::Handle handle)
{
  return getImpl<detail::Loop_Impl>()->demandComponent( handle );
}

boost::optional<ModelObject> Loop::supplyComponent(openstudio::Handle handle)
{
  return getImpl<detail::Loop_Impl>()->supplyComponent( handle );
}

Node Loop::supplyInletNode()
{
  return getImpl<detail::Loop_Impl>()->supplyInletNode();
}

Node Loop::supplyOutletNode()
{
  return getImpl<detail::Loop_Impl>()->supplyOutletNode();
}

Node Loop::demandInletNode()
{
  return getImpl<detail::Loop_Impl>()->demandInletNode();
}

Node Loop::demandOutletNode()
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

