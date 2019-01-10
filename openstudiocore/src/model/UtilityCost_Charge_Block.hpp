/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_UTILITYCOST_CHARGE_BLOCK_HPP
#define MODEL_UTILITYCOST_CHARGE_BLOCK_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Charge_Block_Impl;
}

/** UtilityCost_Charge_Block derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Charge:Block".
 *
 *  UtilityCost_Charge_Block is used to compute energy and demand charges that are structured in blocks of charges.
 *  It may also be used for taxes, surcharges and any other charges that occur on a utility bill but those are more commonly simple flat charges
 *  so UtilityCost_Charge_Simple is more commonly used for those functions. As many UtilityCost_Charge_Block objects as needed may be defined for
 *  a single tariff and they will be added together. Blocks are a structure used by almost all utilities for calculating energy and demand charges
 *  and they allow the utility to charge more or less per unit of energy or demand if more units are used.
 */
class MODEL_API UtilityCost_Charge_Block : public ParentObject{

public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Charge_Block object in the model.
  explicit UtilityCost_Charge_Block(const Model& model);

  virtual ~UtilityCost_Charge_Block() {}

  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> sourceVariable() const;

  boost::optional<std::string> season() const;

  boost::optional<std::string> categoryVariableName() const;

  boost::optional<std::string> remainingIntoVariable() const;

  /** Get the block size value or variable name at index. Indexing starts at 0. */
  boost::optional<std::string> blockSizeValueOrVariableName(unsigned index) const;

  /** Get the cost per unit for block at index. Indexing starts at 0. */
  boost::optional<std::string> blockCostPerUnitValueOrVariableName(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  bool setTariffName(const std::string& tariffName);

  bool setSourceVariable(const std::string& sourceVariable);

  bool setSeason(const std::string& season);

  bool setCategoryVariableName(const std::string& categoryVariableName);

  bool setRemainingIntoVariable(const std::string& remainingIntoVariable);

  /** Set the block size value or variable name at index. Indexing starts at 0. */
  bool setBlockSizeValueOrVariableName(unsigned index, const std::string& blockSizeValueOrVariableName);

  /** Set the cost per unit for block at index. Indexing starts at 0. */
  bool setBlockCostPerUnitValueOrVariableName(unsigned index, const std::string& blockCostPerUnitValueOrVariableName);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

  /// Returns the number of blocks for which data is specified, e.g. the number of extensible fields.
  unsigned numBlocks() const;

  /// Returns the maximum number of blocks for which data may be specified, e.g. the maximum number of extensible fields.
  unsigned maxNumBlocks() const;

protected:

  /// @cond

  typedef detail::UtilityCost_Charge_Block_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Charge_Block(std::shared_ptr<detail::UtilityCost_Charge_Block_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Charge_Block");

  /// @endcond

};

/** \relates UtilityCost_Charge_Block */
typedef boost::optional<UtilityCost_Charge_Block> OptionalUtilityCost_Charge_Block;

/** \relates UtilityCost_Charge_Block */
typedef std::vector<UtilityCost_Charge_Block> UtilityCost_Charge_BlockVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_CHARGE_BLOCK_HPP
