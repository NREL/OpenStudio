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

#ifndef MODEL_WATERTOWATERCOMPONENT_IMPL_HPP
#define MODEL_WATERTOWATERCOMPONENT_IMPL_HPP

#include "WaterToWaterComponent.hpp"

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

class AirLoopHVAC;

namespace detail {

class MODEL_API WaterToWaterComponent_Impl : public HVACComponent_Impl
{
  public:

  WaterToWaterComponent_Impl(IddObjectType type, Model_Impl* model);

  WaterToWaterComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  WaterToWaterComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                             Model_Impl* model, 
                             bool keepHandle);

  WaterToWaterComponent_Impl(const WaterToWaterComponent_Impl& other, Model_Impl* model, bool keepHandles);

  virtual ~WaterToWaterComponent_Impl() {}

  virtual unsigned supplyInletPort() = 0;

  virtual unsigned supplyOutletPort() = 0;

  virtual unsigned demandInletPort() = 0;

  virtual unsigned demandOutletPort() = 0;

  virtual boost::optional<ModelObject> supplyInletModelObject();

  virtual boost::optional<ModelObject> supplyOutletModelObject();

  virtual boost::optional<ModelObject> demandInletModelObject();

  virtual boost::optional<ModelObject> demandOutletModelObject();

  /** This function returns a vector of HVACComponent that are directly downstream
   *  from this object on an AirLoopHVAC or PlantLoop. 
   *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
  **/
  virtual std::vector<HVACComponent> edges(bool isDemandComponent);

  virtual boost::optional<ParentObject> parent() const;

  virtual std::vector<ModelObject> children() const;

  virtual bool addToNode(Node & node);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  void disconnect();

  boost::optional<PlantLoop> plantLoop() const;

  boost::optional<PlantLoop> secondaryPlantLoop() const;

  bool removeFromPlantLoop();

  bool removeFromSecondaryPlantLoop();

  protected:

  friend class Model_Impl;

  mutable boost::optional<PlantLoop> m_secondaryPlantLoop;

  private:

  REGISTER_LOGGER("openstudio.model.WaterToWaterComponent");
};

} // detail

}

}

#endif
