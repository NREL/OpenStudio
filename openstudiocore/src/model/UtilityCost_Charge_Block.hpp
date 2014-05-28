/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

  void setTariffName(const std::string& tariffName);

  void setSourceVariable(const std::string& sourceVariable);

  void setSeason(const std::string& season);

  void setCategoryVariableName(const std::string& categoryVariableName);

  void setRemainingIntoVariable(const std::string& remainingIntoVariable);

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
