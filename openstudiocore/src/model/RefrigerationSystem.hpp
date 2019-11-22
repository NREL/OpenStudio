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

  bool setMinimumCondensingTemperature(double minimumCondensingTemperature);

  bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

  bool setSuctionTemperatureControlType(std::string suctionTemperatureControlType);

  void resetSuctionTemperatureControlType();

  bool setMechanicalSubcooler(const RefrigerationSubcoolerMechanical& refrigerationSubcoolerMechanical);

  void resetMechanicalSubcooler();

  bool setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcoolerLiquidSuction& refrigerationSubcoolerLiquidSuction);

  void resetLiquidSuctionHeatExchangerSubcooler();

  bool setSumUASuctionPiping(double sumUASuctionPiping);

  void resetSumUASuctionPiping();

  bool setSuctionPipingZone(const ThermalZone& thermalZone);

  void resetSuctionPipingZone();

  bool setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  bool setIntercoolerType(std::string intercoolerType);

  void resetIntercoolerType();

  bool setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

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
