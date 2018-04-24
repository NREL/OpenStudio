/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_UTILITYCOST_CHARGE_BLOCK_IMPL_HPP
#define MODEL_UTILITYCOST_CHARGE_BLOCK_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "UtilityCost_Charge_Block.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
namespace detail {

class MODEL_API UtilityCost_Charge_Block_Impl : public ParentObject_Impl{

public:
  // constructor
  UtilityCost_Charge_Block_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  // construct from workspace
  UtilityCost_Charge_Block_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

  // clone copy constructor
  UtilityCost_Charge_Block_Impl(const UtilityCost_Charge_Block_Impl& other,Model_Impl* model,bool keepHandle);

  // virtual destructor
  virtual ~UtilityCost_Charge_Block_Impl(){}

  OptionalString tariffName() const;
  bool setTariffName(const std::string& str);

  OptionalString sourceVariable() const;
  bool setSourceVariable(const std::string& str);

  OptionalString season() const;
  bool setSeason(const std::string& str);

  OptionalString categoryVariableName() const;
  bool setCategoryVariableName(const std::string& str);

  OptionalString remainingIntoVariable() const;
  bool setRemainingIntoVariable(const std::string& str);

  /** Get the block size value or variable name at index. Indexing starts at 0. */
  boost::optional<std::string> blockSizeValueOrVariableName(unsigned index) const;
  bool setBlockSizeValueOrVariableName(unsigned index, const std::string& str);

  /** Get the cost per unit for block at index. Indexing starts at 0. */
  boost::optional<std::string> blockCostPerUnitValueOrVariableName(unsigned index) const;
  bool setBlockCostPerUnitValueOrVariableName(unsigned index, const std::string& str);

  unsigned numBlocks() const;
  unsigned maxNumBlocks() const;

  // return the parent object in the hierarchy
  virtual boost::optional<ParentObject> parent() const override;

  // set the parent, child may have to call methods on the parent
  virtual bool setParent(ParentObject& newParent) override;

  // return any children objects in the hierarchy
  virtual std::vector<ModelObject> children() const override;

  /// get a vector of allowable children types
  virtual std::vector<IddObjectType> allowableChildTypes() const override;

  // Get all output variable names that could be associated with this object.
  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override {return UtilityCost_Charge_Block::iddObjectType();}

private:
  REGISTER_LOGGER("openstudio.model.UtilityCost_Charge_Block");

};

} // detail
} // model
} // openstudio

#endif // MODEL_UTILITYCOST_CHARGE_BLOCK_IMPL_HPP
