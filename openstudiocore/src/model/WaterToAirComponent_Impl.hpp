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

#ifndef MODEL_WATERTOAIRCOMPONENT_IMPL_HPP
#define MODEL_WATERTOAIRCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail {

class MODEL_API WaterToAirComponent_Impl : public HVACComponent_Impl {
 public:

  WaterToAirComponent_Impl(IddObjectType type, Model_Impl* model);

  WaterToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  WaterToAirComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                         Model_Impl* model, 
                         bool keepHandle);

  WaterToAirComponent_Impl(const WaterToAirComponent_Impl& other, Model_Impl* model, bool keepHandles);

  virtual ~WaterToAirComponent_Impl() {}

  virtual boost::optional<ModelObject> airInletModelObject();

  virtual boost::optional<ModelObject> airOutletModelObject();

  virtual boost::optional<ModelObject> waterInletModelObject();

  virtual boost::optional<ModelObject> waterOutletModelObject();

  /** This function returns a vector of HVACComponent that are directly downstream
   *  from this object on an AirLoopHVAC or PlantLoop. 
   *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
  **/
  virtual std::vector<HVACComponent> edges(bool isDemandComponent);

  virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

  virtual boost::optional<PlantLoop> plantLoop() const;

  virtual boost::optional<Loop> loop() const;

  virtual unsigned airInletPort() = 0;
   
  virtual unsigned airOutletPort() = 0;
   
  virtual unsigned waterInletPort() = 0;
   
  virtual unsigned waterOutletPort() = 0;

  virtual bool addToNode(Node & node);

  virtual bool addToSplitter(Splitter & splitter);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  virtual void disconnectWaterSide();

  virtual void disconnectAirSide();

  virtual void disconnect();

  virtual bool removeFromAirLoopHVAC();

  virtual bool removeFromPlantLoop();
 private:
  REGISTER_LOGGER("openstudio.model.WaterToAirComponent");
};

} // detail

} // model

} // openstudio

#endif

