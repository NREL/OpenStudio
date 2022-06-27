/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_IMPL_HPP
#define MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Node;
  class Node;

  namespace detail {

    /** SetpointManagerSystemNodeResetHumidity_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSystemNodeResetHumidity.*/
    class MODEL_API SetpointManagerSystemNodeResetHumidity_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerSystemNodeResetHumidity_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSystemNodeResetHumidity_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSystemNodeResetHumidity_Impl(const SetpointManagerSystemNodeResetHumidity_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerSystemNodeResetHumidity_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string controlVariable() const;

      double setpointatLowReferenceHumidityRatio() const;

      double setpointatHighReferenceHumidityRatio() const;

      double lowReferenceHumidityRatio() const;

      double highReferenceHumidityRatio() const;

      // TODO: Check return type. From object lists, some candidates are: Node.
      boost::optional<Node> referenceNode() const;

      // TODO: Check return type. From object lists, some candidates are: Node.
      boost::optional<Node> setpointNodeorNodeList() const;

      //@}
      /** @name Setters */
      //@{

      bool setControlVariable(const std::string& controlVariable);

      bool setSetpointatLowReferenceHumidityRatio(double setpointatLowReferenceHumidityRatio);

      bool setSetpointatHighReferenceHumidityRatio(double setpointatHighReferenceHumidityRatio);

      bool setLowReferenceHumidityRatio(double lowReferenceHumidityRatio);

      bool setHighReferenceHumidityRatio(double highReferenceHumidityRatio);

      // TODO: Check argument type. From object lists, some candidates are: Node.
      bool setReferenceNode(const Node& node);

      void resetReferenceNode();

      // TODO: Check argument type. From object lists, some candidates are: Node.
      bool setSetpointNodeorNodeList(const Node& node);

      void resetSetpointNodeorNodeList();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SetpointManagerSystemNodeResetHumidity");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_IMPL_HPP
