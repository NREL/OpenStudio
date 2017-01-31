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

  virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & prev) override;

  virtual boost::optional<ParentObject> parent() const override;

  virtual std::vector<ModelObject> children() const override;

  virtual bool addToNode(Node & node) override;

  virtual std::vector<openstudio::IdfObject> remove() override;

  virtual ModelObject clone(Model model) const override;

  void disconnect() override;

  boost::optional<PlantLoop> plantLoop() const override;

  boost::optional<PlantLoop> secondaryPlantLoop() const;

  bool removeFromPlantLoop();

  bool removeFromSecondaryPlantLoop();

  virtual unsigned tertiaryInletPort() const;

  virtual unsigned tertiaryOutletPort() const;

  boost::optional<PlantLoop> tertiaryPlantLoop() const;

  bool removeFromTertiaryPlantLoop();

  virtual bool addToTertiaryNode(Node & node);

  boost::optional<ModelObject> tertiaryInletModelObject() const;

  boost::optional<ModelObject> tertiaryOutletModelObject() const;

  protected:

  friend class Model_Impl;

  mutable boost::optional<PlantLoop> m_secondaryPlantLoop;
  mutable boost::optional<PlantLoop> m_tertiaryPlantLoop;

  private:

  REGISTER_LOGGER("openstudio.model.WaterToWaterComponent");
};

} // detail

}

}

#endif
