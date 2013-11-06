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

#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  HVACComponent_Impl::HVACComponent_Impl(IddObjectType type, Model_Impl* model)
    : ParentObject_Impl(type,model)
  {
  }

  HVACComponent_Impl::HVACComponent_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle)
                                           : ParentObject_Impl(idfObject, model, keepHandle)
  {
  }

  HVACComponent_Impl::HVACComponent_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : ParentObject_Impl(other,model,keepHandle)
  {
  }

  HVACComponent_Impl::HVACComponent_Impl(const HVACComponent_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandles)
                                           : ParentObject_Impl(other,model,keepHandles)
  {
  }

  boost::optional<Loop> HVACComponent_Impl::loop() const
  {
    boost::optional<AirLoopHVAC> airLoopHVAC = this->airLoopHVAC();
    boost::optional<PlantLoop> plantLoop = this->plantLoop();

    if( airLoopHVAC )
    {
      return airLoopHVAC->optionalCast<Loop>();
    }
    else if( plantLoop )
    {
      return plantLoop->optionalCast<Loop>();
    }
    else
    {
      return boost::none;
    }
  }

  boost::optional<AirLoopHVAC> HVACComponent_Impl::airLoopHVAC() const
  {
    if( m_airLoopHVAC )
    {
      return m_airLoopHVAC;
    }
    else
    {
      AirLoopHVACVector airLoops = this->model().getModelObjects<AirLoopHVAC>();

      for( AirLoopHVACVector::iterator it = airLoops.begin(),itEnd=airLoops.end();
      it != itEnd;
      ++it )
      {
        OptionalAirLoopHVAC airLoop = it->optionalCast<AirLoopHVAC>();
        if(airLoop)
        {
          if( airLoop->component(this->handle()) )
          {
            m_airLoopHVAC = airLoop;

            return airLoop;
          }
          if( OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoop->airLoopHVACOutdoorAirSystem() )
          {
            if( oaSystem->component(this->handle()) )
            {
              m_airLoopHVAC = airLoop;

              return airLoop;
            }
          }
        }
      }
    }

    return boost::none;
  }

  boost::optional<AirLoopHVACOutdoorAirSystem> HVACComponent_Impl::airLoopHVACOutdoorAirSystem() const
  {
    if( m_airLoopHVACOutdoorAirSystem )
    {
      return m_airLoopHVACOutdoorAirSystem;
    }
    else
    {
      AirLoopHVACOutdoorAirSystemVector oaLoops = this->model().getModelObjects<AirLoopHVACOutdoorAirSystem>();

      for( AirLoopHVACOutdoorAirSystemVector::iterator it = oaLoops.begin(),itEnd=oaLoops.end();
      it != itEnd;
      ++it )
      {
        if( it->component(this->handle()) )
        {
          m_airLoopHVACOutdoorAirSystem = *it;

          return *it;
        }
      }
    }

    return boost::none;
  }

  boost::optional<PlantLoop> HVACComponent_Impl::plantLoop() const
  {
    if( m_plantLoop )
    {
      return m_plantLoop;
    }
    else
    {
      std::vector<PlantLoop> plantLoops = this->model().getModelObjects<PlantLoop>();

      for( std::vector<PlantLoop>::iterator it = plantLoops.begin(),itEnd=plantLoops.end();
      it != itEnd;
      ++it )
      {
        OptionalPlantLoop plantLoop = it->optionalCast<PlantLoop>();
        if(plantLoop)
        {
          if( plantLoop->component(this->handle()) )
          {
            m_plantLoop = plantLoop;

            return plantLoop;
          }
        }
      }
    }

    return boost::none;
  }

  bool HVACComponent_Impl::addToNode(Node & node)
  {
    return false;
  }

  std::vector<ModelObject> HVACComponent_Impl::children() const
  {
    return std::vector<ModelObject>();
  }

  boost::optional<ParentObject> HVACComponent_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  bool HVACComponent_Impl::addToSplitter(Splitter & splitter)
  {
    return false;
  }

  const std::vector<std::string> & HVACComponent_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  void HVACComponent_Impl::disconnect()
  {
    return;
  }

  bool HVACComponent_Impl::isRemovable() const
  {
    if( containingHVACComponent() )
    {
      return false;
    }
    else if( containingZoneHVACComponent() )
    {
      return false;
    }
    else if( containingStraightComponent() )
    {
      return false;
    }    
    else
    {
      return true;
    }
  }

  std::vector<IdfObject> HVACComponent_Impl::remove()
  {
    if( this->isRemovable() )
    {
      return ParentObject_Impl::remove();
    }
    else
    {
      return std::vector<IdfObject>();
    }
  }

  boost::optional<ModelObject> HVACComponent_Impl::airLoopHVACAsModelObject() const {
    OptionalModelObject result;
    OptionalAirLoopHVAC intermediate = airLoopHVAC();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> HVACComponent_Impl::plantLoopAsModelObject() const {
    OptionalModelObject result;
    OptionalPlantLoop intermediate = plantLoop();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> HVACComponent_Impl::airLoopHVACOutdoorAirSystemAsModelObject() const {
    OptionalModelObject result;
    OptionalAirLoopHVACOutdoorAirSystem intermediate = airLoopHVACOutdoorAirSystem();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  ModelObject HVACComponent_Impl::clone(Model model) const
  {
    return ModelObject_Impl::clone(model);
  }

  boost::optional<HVACComponent> HVACComponent_Impl::containingHVACComponent() const
  {
    return boost::none;
  }

  boost::optional<ZoneHVACComponent> HVACComponent_Impl::containingZoneHVACComponent() const
  {
    return boost::none;
  }
  
  boost::optional<StraightComponent> HVACComponent_Impl::containingStraightComponent() const
  {
    return boost::none;
  }

} // detail

HVACComponent::HVACComponent(boost::shared_ptr<detail::HVACComponent_Impl> p)
  : ParentObject(p)
{}

HVACComponent::HVACComponent(IddObjectType type,const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::HVACComponent_Impl>());
}     

boost::optional<Loop> HVACComponent::loop() const
{
  return getImpl<detail::HVACComponent_Impl>()->loop();
}

boost::optional<AirLoopHVAC> HVACComponent::airLoopHVAC() const
{
  return getImpl<detail::HVACComponent_Impl>()->airLoopHVAC();
}

boost::optional<AirLoopHVACOutdoorAirSystem> HVACComponent::airLoopHVACOutdoorAirSystem() const
{
  return getImpl<detail::HVACComponent_Impl>()->airLoopHVACOutdoorAirSystem();
}

boost::optional<PlantLoop> HVACComponent::plantLoop() const
{
  return getImpl<detail::HVACComponent_Impl>()->plantLoop();
}

bool HVACComponent::addToNode(Node & node)
{
  return getImpl<detail::HVACComponent_Impl>()->addToNode( node );
}

std::vector<ModelObject> HVACComponent::children() const
{
  return getImpl<detail::HVACComponent_Impl>()->children();
}

bool HVACComponent::addToSplitter(Splitter & splitter)
{
  return getImpl<detail::HVACComponent_Impl>()->addToSplitter( splitter );
}

void HVACComponent::disconnect()
{
  return getImpl<detail::HVACComponent_Impl>()->disconnect();
}

bool HVACComponent::isRemovable() const
{
  return getImpl<detail::HVACComponent_Impl>()->isRemovable();
}

std::vector<IdfObject> HVACComponent::remove()
{
  return getImpl<detail::HVACComponent_Impl>()->remove();
}

boost::optional<HVACComponent> HVACComponent::containingHVACComponent() const
{
  return getImpl<detail::HVACComponent_Impl>()->containingHVACComponent();
}

boost::optional<ZoneHVACComponent> HVACComponent::containingZoneHVACComponent() const
{
  return getImpl<detail::HVACComponent_Impl>()->containingZoneHVACComponent();
}

boost::optional<StraightComponent> HVACComponent::containingStraightComponent() const
{
  return getImpl<detail::HVACComponent_Impl>()->containingStraightComponent();
}

} // model

} // openstudio

