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

#ifndef MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkLinkage_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class AirflowNetworkNode;
  class AirflowNetworkComponent;
  class ThermalZone;

  namespace detail {

    /** AirflowNetworkDistributionLinkage_Impl is a AirflowNetworkLinkage_Impl that is the implementation class for AirflowNetworkDistributionLinkage.*/
    class MODEL_API AirflowNetworkDistributionLinkage_Impl : public AirflowNetworkLinkage_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkDistributionLinkage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkDistributionLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkDistributionLinkage_Impl(const AirflowNetworkDistributionLinkage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkDistributionLinkage_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      // TODO: Check return type. From object lists, some candidates are: AirflowNetworkNodeAndZone.
      AirflowNetworkNode node1() const;

      // TODO: Check return type. From object lists, some candidates are: AirflowNetworkNodeAndZone.
      AirflowNetworkNode node2() const;

      // TODO: Check return type. From object lists, some candidates are: AirflowNetworkComponent.
      AirflowNetworkComponent component() const;

      // TODO: Check return type. From object lists, some candidates are: Zone.
      boost::optional<ThermalZone> thermalZone() const;

      //@}
      /** @name Setters */
      //@{

      /*
    template <typename T> bool setNode1(const T& airflowNetworkNode)
    {
      bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::Node1Name, airflowNetworkNode.handle());
      return result;
    }

    template <typename T> bool setNode2(const T& airflowNetworkNode)
    {
      bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::Node2Name, airflowNetworkNode.handle());
      return result;
    }

    template <typename T> bool setComponent(const T& airflowNetworkComponent)
    {
      bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::ComponentName, airflowNetworkComponent.handle());
      return result;
    }
    */

      bool setNode1(const AirflowNetworkNode& airflowNetworkNode);

      bool setNode2(const AirflowNetworkNode& airflowNetworkNode);

      bool setComponent(const AirflowNetworkComponent& airflowNetworkComponent);

      // TODO: Check argument type. From object lists, some candidates are: Zone.
      bool setThermalZone(const ThermalZone& zone);

      void resetThermalZone();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkDistributionLinkage");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<AirflowNetworkNode> optionalNode1() const;
      boost::optional<AirflowNetworkNode> optionalNode2() const;
      boost::optional<AirflowNetworkComponent> optionalComponent() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_IMPL_HPP
