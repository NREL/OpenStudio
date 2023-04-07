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

#ifndef MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP
#define MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP

#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API SetpointManagerScheduled_Impl : public SetpointManager_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerScheduled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerScheduled_Impl(const SetpointManagerScheduled_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~SetpointManagerScheduled_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // virtual bool addToNode(Node & node) override;

      virtual bool isAllowedOnPlantLoop() const override;

      //@}
      /** @name Getters */
      //@{

      virtual boost::optional<Node> setpointNode() const override;

      virtual std::string controlVariable() const override;

      Schedule schedule() const;

      bool hasSchedule() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      /** Sets control variable and schedule simultaneously. Use in the case that the
   *  control variable change implies a change in schedule units. */
      bool setControlVariableAndSchedule(const std::string& controlVariable, Schedule& schedule);

      //@}
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      // converts control variable to the schedule's display name
      std::string scheduleDisplayName() const;

      // as above, but used to check candidate control variables
      std::string scheduleDisplayName(const std::string& candidateControlVariable) const;

      std::vector<std::string> controlVariableValues() const;

      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP
