/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_REFRIGERATIONSYSTEM_HPP
#define MODEL_REFRIGERATIONSYSTEM_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class ModelObjectLists;
class RefrigerationAllTypesCondenser;
class Fluid;
class RefrigerationSubcooler;
class ThermalZone;

namespace detail {

  class RefrigerationSystem_Impl;

} // detail

/** RefrigerationSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:System'. */
class MODEL_API RefrigerationSystem : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationSystem(const Model& model);

  virtual ~RefrigerationSystem() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> suctionTemperatureControlTypeValues();

  static std::vector<std::string> numberofCompressorStagesValues();

  static std::vector<std::string> intercoolerTypeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: ModelObjectLists.
  boost::optional<ModelObjectLists> refrigeratedCaseAndWalkInList() const;

  // TODO: Check return type. From object lists, some candidates are: ModelObjectLists.
  boost::optional<ModelObjectLists> refrigerationTransferLoadList() const;

  // TODO: Check return type. From object lists, some candidates are: RefrigerationAllTypesCondenser.
  RefrigerationAllTypesCondenser refrigerationCondenser() const;

  // TODO: Check return type. From object lists, some candidates are: ModelObjectLists.
  ModelObjectLists compressorList() const;

  double minimumCondensingTemperature() const;

  // TODO: Check return type. From object lists, some candidates are: Fluid.
  Fluid refrigerationSystemWorkingFluidType() const;

  std::string suctionTemperatureControlType() const;

  bool isSuctionTemperatureControlTypeDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: RefrigerationSubcooler.
  boost::optional<RefrigerationSubcooler> mechanicalSubcooler() const;

  // TODO: Check return type. From object lists, some candidates are: RefrigerationSubcooler.
  boost::optional<RefrigerationSubcooler> liquidSuctionHeatExchangerSubcooler() const;

  double sumUASuctionPiping() const;

  bool isSumUASuctionPipingDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> suctionPipingZone() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  std::string numberofCompressorStages() const;

  bool isNumberofCompressorStagesDefaulted() const;

  std::string intercoolerType() const;

  bool isIntercoolerTypeDefaulted() const;

  double shellandCoilIntercoolerEffectiveness() const;

  bool isShellandCoilIntercoolerEffectivenessDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: ModelObjectLists.
  boost::optional<ModelObjectLists> highStageCompressorList() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: ModelObjectLists.
  bool setRefrigeratedCaseAndWalkInList(const ModelObjectLists& modelObjectLists);

  void resetRefrigeratedCaseAndWalkInList();

  // TODO: Check argument type. From object lists, some candidates are: ModelObjectLists.
  bool setRefrigerationTransferLoadList(const ModelObjectLists& modelObjectLists);

  void resetRefrigerationTransferLoadList();

  // TODO: Check argument type. From object lists, some candidates are: RefrigerationAllTypesCondenser.
  bool setRefrigerationCondenser(const RefrigerationAllTypesCondenser& refrigerationAllTypesCondenser);

  // TODO: Check argument type. From object lists, some candidates are: ModelObjectLists.
  bool setCompressorList(const ModelObjectLists& modelObjectLists);

  void setMinimumCondensingTemperature(double minimumCondensingTemperature);

  // TODO: Check argument type. From object lists, some candidates are: Fluid.
  bool setRefrigerationSystemWorkingFluidType(const Fluid& fluid);

  bool setSuctionTemperatureControlType(std::string suctionTemperatureControlType);

  void resetSuctionTemperatureControlType();

  // TODO: Check argument type. From object lists, some candidates are: RefrigerationSubcooler.
  bool setMechanicalSubcooler(const RefrigerationSubcooler& refrigerationSubcooler);

  void resetMechanicalSubcooler();

  // TODO: Check argument type. From object lists, some candidates are: RefrigerationSubcooler.
  bool setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcooler& refrigerationSubcooler);

  void resetLiquidSuctionHeatExchangerSubcooler();

  void setSumUASuctionPiping(double sumUASuctionPiping);

  void resetSumUASuctionPiping();

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setSuctionPipingZone(const ThermalZone& thermalZone);

  void resetSuctionPipingZone();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  bool setNumberofCompressorStages(std::string numberofCompressorStages);

  void resetNumberofCompressorStages();

  bool setIntercoolerType(std::string intercoolerType);

  void resetIntercoolerType();

  void setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

  void resetShellandCoilIntercoolerEffectiveness();

  // TODO: Check argument type. From object lists, some candidates are: ModelObjectLists.
  bool setHighStageCompressorList(const ModelObjectLists& modelObjectLists);

  void resetHighStageCompressorList();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationSystem_Impl ImplType;

  explicit RefrigerationSystem(boost::shared_ptr<detail::RefrigerationSystem_Impl> impl);

  friend class detail::RefrigerationSystem_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationSystem");
};

/** \relates RefrigerationSystem*/
typedef boost::optional<RefrigerationSystem> OptionalRefrigerationSystem;

/** \relates RefrigerationSystem*/
typedef std::vector<RefrigerationSystem> RefrigerationSystemVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONSYSTEM_HPP

