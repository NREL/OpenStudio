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

#ifndef MODEL_WATERTOAIRCOMPONENT_IMPL_HPP
#define MODEL_WATERTOAIRCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

class ControllerWaterCoil;

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

  virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & previous) override;

  virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

  virtual boost::optional<PlantLoop> plantLoop() const override;

  virtual boost::optional<Loop> loop() const override;

  virtual unsigned airInletPort() = 0;

  virtual unsigned airOutletPort() = 0;

  virtual unsigned waterInletPort() = 0;

  virtual unsigned waterOutletPort() = 0;

  virtual bool addToNode(Node & node) override;

  virtual bool addToSplitter(Splitter & splitter) override;

  virtual std::vector<openstudio::IdfObject> remove() override;

  virtual ModelObject clone(Model model) const override;

  virtual void disconnectWaterSide();

  virtual void disconnectAirSide();

  virtual void disconnect() override;

  virtual bool removeFromAirLoopHVAC();

  virtual bool removeFromPlantLoop();

  boost::optional<ControllerWaterCoil> controllerWaterCoil();
 private:
  REGISTER_LOGGER("openstudio.model.WaterToAirComponent");
};

} // detail

} // model

} // openstudio

#endif

