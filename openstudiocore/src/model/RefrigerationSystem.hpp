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

#ifndef MODEL_REFRIGERATIONSYSTEM_HPP
#define MODEL_REFRIGERATIONSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class RefrigerationAirChiller;
class RefrigerationCase;
class RefrigerationCompressor;
class RefrigerationWalkIn;
class RefrigerationSecondarySystem;
class RefrigerationCondenserCascade;
class RefrigerationSubcoolerLiquidSuction;
class RefrigerationSubcoolerMechanical;
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

  static std::vector<std::string> refrigerationSystemWorkingFluidTypeValues();

  static std::vector<std::string> suctionTemperatureControlTypeValues();

  static std::vector<std::string> intercoolerTypeValues();

  std::vector<IdfObject> remove();

  ModelObject clone(Model model) const;

  bool addCase( const RefrigerationCase & refrigerationCase);

  void removeCase( const RefrigerationCase & refrigerationCase);

  void removeAllCases();

  std::vector<RefrigerationCase> cases() const;

  bool addWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeAllWalkins();

  std::vector<RefrigerationWalkIn> walkins() const;

  bool addCompressor( const RefrigerationCompressor & compressor);

  void removeCompressor( const RefrigerationCompressor & compressor);

  void removeAllCompressors();

  std::vector<RefrigerationCompressor> compressors() const;

  bool addHighStageCompressor( const RefrigerationCompressor & highStageCompressor);

  void removeHighStageCompressor( const RefrigerationCompressor & highStageCompressor);

  void removeAllHighStageCompressors();

  std::vector<RefrigerationCompressor> highStageCompressors() const;

  bool addSecondarySystemLoad( const RefrigerationSecondarySystem & refrigerationSecondarySystem);

  void removeSecondarySystemLoad( const RefrigerationSecondarySystem & refrigerationSecondarySystem);

  void removeAllSecondarySystemLoads();

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads() const;

  bool addCascadeCondenserLoad( const RefrigerationCondenserCascade & refrigerationCondenserCascade);

  void removeCascadeCondenserLoad( const RefrigerationCondenserCascade & refrigerationCondenserCascade);

  void removeAllCascadeCondenserLoads();

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads() const;

  bool addAirChiller( const RefrigerationAirChiller & airChiller);

  void removeAirChiller( const RefrigerationAirChiller & airChiller);

  void removeAllAirChillers();

  std::vector<RefrigerationAirChiller> airChillers() const;

  /** @name Getters */
  //@{

  boost::optional<ModelObject> refrigerationCondenser() const;

  double minimumCondensingTemperature() const;

  std::string refrigerationSystemWorkingFluidType() const;

  std::string suctionTemperatureControlType() const;

  bool isSuctionTemperatureControlTypeDefaulted() const;

  boost::optional<RefrigerationSubcoolerMechanical> mechanicalSubcooler() const;

  boost::optional<RefrigerationSubcoolerLiquidSuction> liquidSuctionHeatExchangerSubcooler() const;

  double sumUASuctionPiping() const;

  bool isSumUASuctionPipingDefaulted() const;

  boost::optional<ThermalZone> suctionPipingZone() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  std::string numberofCompressorStages() const;

  std::string intercoolerType() const;

  bool isIntercoolerTypeDefaulted() const;

  double shellandCoilIntercoolerEffectiveness() const;

  bool isShellandCoilIntercoolerEffectivenessDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** \fn bool setRefrigerationCondenser(const ModelObject& refrigerationCondenser)
      \brief Sets the refrigeration condenser for this refrigeration system.
      \param[in] refrigerationCondenser The refrigeration condenser.

      Valid inputs: \n
      \ref RefrigerationCondenserAirCooled \n
      \ref RefrigerationCondenserCascade \n
      \ref RefrigerationCondenserEvaporativeCooled \n
      \ref RefrigerationCondenserWaterCooled \n
  */
  bool setRefrigerationCondenser(const ModelObject& refrigerationCondenser);

  void setMinimumCondensingTemperature(double minimumCondensingTemperature);

  bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

  bool setSuctionTemperatureControlType(std::string suctionTemperatureControlType);

  void resetSuctionTemperatureControlType();

  bool setMechanicalSubcooler(const RefrigerationSubcoolerMechanical& refrigerationSubcoolerMechanical);

  void resetMechanicalSubcooler();

  bool setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcoolerLiquidSuction& refrigerationSubcoolerLiquidSuction);

  void resetLiquidSuctionHeatExchangerSubcooler();

  void setSumUASuctionPiping(double sumUASuctionPiping);

  void resetSumUASuctionPiping();

  bool setSuctionPipingZone(const ThermalZone& thermalZone);

  void resetSuctionPipingZone();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  bool setIntercoolerType(std::string intercoolerType);

  void resetIntercoolerType();

  void setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

  void resetShellandCoilIntercoolerEffectiveness();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationSystem_Impl ImplType;

  explicit RefrigerationSystem(std::shared_ptr<detail::RefrigerationSystem_Impl> impl);

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

