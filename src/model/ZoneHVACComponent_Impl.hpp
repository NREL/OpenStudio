/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACCOMPONENT_IMPL_HPP
#define MODEL_ZONEHVACCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;
  class ThermalZone;
  class AirLoopHVAC;
  class HVACComponent;

  namespace detail {

    class MODEL_API ZoneHVACComponent_Impl : public HVACComponent_Impl
    {

     public:
      ZoneHVACComponent_Impl(IddObjectType type, Model_Impl* model);

      ZoneHVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACComponent_Impl(const ZoneHVACComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~ZoneHVACComponent_Impl() = default;

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual ModelObject clone(Model model) const override;

      virtual unsigned inletPort() const = 0;

      virtual unsigned outletPort() const = 0;

      boost::optional<Node> inletNode() const;

      boost::optional<Node> outletNode() const;

      virtual boost::optional<ThermalZone> thermalZone() const;

      virtual bool addToThermalZone(ThermalZone& thermalZone);

      virtual void removeFromThermalZone();

      std::vector<openstudio::IdfObject> remove() override;

      bool addToNode(Node& node) override;

      boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      bool removeFromAirLoopHVAC();

      void removeFromInletSideMixer();

      std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      boost::optional<ModelObject> airInletModelObject() const;

      boost::optional<ModelObject> airOutletModelObject() const;

      bool setReturnPlenum(const ThermalZone& plenumZone);

      void removeReturnPlenum();

      boost::optional<AirLoopHVACReturnPlenum> returnPlenum() const;

     protected:
      friend class Model_Impl;

     private:
      boost::optional<Connection> returnPlenumOutletNodeConnection() const;

      REGISTER_LOGGER("openstudio.model.ZoneHVACComponent");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
