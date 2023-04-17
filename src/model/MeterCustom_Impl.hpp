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

#ifndef MODEL_METERCUSTOM_IMPL_HPP
#define MODEL_METERCUSTOM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** MeterCustom_Impl is a ModelObject_Impl that is the implementation class for MeterCustom.*/
    class MODEL_API MeterCustom_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      MeterCustom_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      MeterCustom_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      MeterCustom_Impl(const MeterCustom_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~MeterCustom_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<std::string> fuelType() const;

      // Return a vector of (Key, Var) pairs
      std::vector<std::pair<std::string, std::string>> keyVarGroups();

      // Return the number of (KeyName, OutputVariableorMeterName) groups
      unsigned numKeyVarGroups() const;

      // Lower Level functions
      /** Get the Key Name at index. Indexing starts at 0. */
      boost::optional<std::string> keyName(unsigned index) const;

      /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
      boost::optional<std::string> outputVariableorMeterName(unsigned index) const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelType(const std::string& fuelType);

      void resetFuelType();

      // Add a new (Key, Var) group
      bool addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName);

      // Remove the (Key, Var) group at given index
      bool removeKeyVarGroup(unsigned groupIndex);

      // Remove all the (Key, Var) groups
      void removeAllKeyVarGroups();

      // Lower level functions
      /** Set the Key Name at index. Indexing starts at 0. */
      bool setKeyName(unsigned index, const std::string& str);

      /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
      bool setOutputVariableorMeterName(unsigned index, const std::string& str);

      //@}
      /** @name Other */
      //@{

      //@}
      /** @name Type Casting */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MeterCustom");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_METERCUSTOM_IMPL_HPP
