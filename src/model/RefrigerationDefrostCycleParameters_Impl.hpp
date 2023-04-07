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

#ifndef MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_IMPL_HPP
#define MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class Time;

namespace model {

  namespace detail {

    /** RefrigerationDefrostCycleParameters_Impl is a ModelObject_Impl that is the implementation class for RefrigerationDefrostCycleParameters.*/
    class MODEL_API RefrigerationDefrostCycleParameters_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationDefrostCycleParameters_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationDefrostCycleParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationDefrostCycleParameters_Impl(const RefrigerationDefrostCycleParameters_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationDefrostCycleParameters_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<int> durationofDefrostCycle() const;

      boost::optional<int> dripDownTime() const;

      boost::optional<openstudio::Time> defrost1StartTime() const;

      boost::optional<openstudio::Time> defrost2StartTime() const;

      boost::optional<openstudio::Time> defrost3StartTime() const;

      boost::optional<openstudio::Time> defrost4StartTime() const;

      boost::optional<openstudio::Time> defrost5StartTime() const;

      boost::optional<openstudio::Time> defrost6StartTime() const;

      boost::optional<openstudio::Time> defrost7StartTime() const;

      boost::optional<openstudio::Time> defrost8StartTime() const;

      //@}
      /** @name Setters */
      //@{

      bool setDurationofDefrostCycle(boost::optional<int> durationofDefrostCycle);

      void resetDurationofDefrostCycle();

      bool setDripDownTime(boost::optional<int> dripDownTime);

      void resetDripDownTime();

      bool setDefrost1StartTime(const openstudio::Time& defrost1StartTime);

      void resetDefrost1StartTime();

      bool setDefrost2StartTime(const openstudio::Time& defrost2StartTime);

      void resetDefrost2StartTime();

      bool setDefrost3StartTime(const openstudio::Time& defrost3StartTime);

      void resetDefrost3StartTime();

      bool setDefrost4StartTime(const openstudio::Time& defrost4StartTime);

      void resetDefrost4StartTime();

      bool setDefrost5StartTime(const openstudio::Time& defrost5StartTime);

      void resetDefrost5StartTime();

      bool setDefrost6StartTime(const openstudio::Time& defrost6StartTime);

      void resetDefrost6StartTime();

      bool setDefrost7StartTime(const openstudio::Time& defrost7StartTime);

      void resetDefrost7StartTime();

      bool setDefrost8StartTime(const openstudio::Time& defrost8StartTime);

      void resetDefrost8StartTime();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationDefrostCycleParameters");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_IMPL_HPP
