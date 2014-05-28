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

#ifndef MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_HPP
#define MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class RefrigerationCase;
class RefrigerationCompressor;
class RefrigerationWalkIn;
class RefrigerationGasCoolerAirCooled;
class ThermalZone;

namespace detail {

  class RefrigerationTranscriticalSystem_Impl;

} // detail

/** RefrigerationTranscriticalSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:TranscriticalSystem'. */
class MODEL_API RefrigerationTranscriticalSystem : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationTranscriticalSystem(const Model& model);

  virtual ~RefrigerationTranscriticalSystem() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> refrigerationSystemWorkingFluidTypeValues();

  std::vector<IdfObject> remove();

  ModelObject clone(Model model) const;

  bool addMediumTemperatureCase( const RefrigerationCase & refrigerationCase);

  void removeMediumTemperatureCase( const RefrigerationCase & refrigerationCase);

  void removeAllMediumTemperatureCases();

  std::vector<RefrigerationCase> mediumTemperatureCases() const;

  bool addLowTemperatureCase( const RefrigerationCase & refrigerationCase);

  void removeLowTemperatureCase( const RefrigerationCase & refrigerationCase);

  void removeAllLowTemperatureCases();

  std::vector<RefrigerationCase> lowTemperatureCases() const;

  bool addMediumTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeMediumTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeAllMediumTemperatureWalkins();

  std::vector<RefrigerationWalkIn> mediumTemperatureWalkins() const;

  bool addLowTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeLowTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeAllLowTemperatureWalkins();

  std::vector<RefrigerationWalkIn> lowTemperatureWalkins() const;

  bool addHighPressureCompressor( const RefrigerationCompressor & compressor);

  void removeHighPressureCompressor( const RefrigerationCompressor & compressor);

  void removeAllHighPressureCompressors();

  std::vector<RefrigerationCompressor> highPressureCompressors() const;

  bool addLowPressureCompressor( const RefrigerationCompressor & compressor);

  void removeLowPressureCompressor( const RefrigerationCompressor & compressor);

  void removeAllLowPressureCompressors();

  std::vector<RefrigerationCompressor> lowPressureCompressors() const;

  /** @name Getters */
  //@{

  boost::optional<RefrigerationGasCoolerAirCooled> refrigerationGasCooler() const;

  double receiverPressure() const;

  bool isReceiverPressureDefaulted() const;

  double subcoolerEffectiveness() const;

  bool isSubcoolerEffectivenessDefaulted() const;

  std::string refrigerationSystemWorkingFluidType() const;

  double sumUASuctionPipingforMediumTemperatureLoads() const;

  bool isSumUASuctionPipingforMediumTemperatureLoadsDefaulted() const;

  boost::optional<ThermalZone> mediumTemperatureSuctionPipingZone() const;

  double sumUASuctionPipingforLowTemperatureLoads() const;

  bool isSumUASuctionPipingforLowTemperatureLoadsDefaulted() const;

  boost::optional<ThermalZone> lowTemperatureSuctionPipingZone() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setRefrigerationGasCooler(const RefrigerationGasCoolerAirCooled& refrigerationGasCoolerAirCooled);

  void setReceiverPressure(double receiverPressure);

  void resetReceiverPressure();

  void setSubcoolerEffectiveness(double subcoolerEffectiveness);

  void resetSubcoolerEffectiveness();

  bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

  void setSumUASuctionPipingforMediumTemperatureLoads(double sumUASuctionPipingforMediumTemperatureLoads);

  void resetSumUASuctionPipingforMediumTemperatureLoads();

  bool setMediumTemperatureSuctionPipingZone(const ThermalZone& thermalZone);

  void resetMediumTemperatureSuctionPipingZone();

  void setSumUASuctionPipingforLowTemperatureLoads(double sumUASuctionPipingforLowTemperatureLoads);

  void resetSumUASuctionPipingforLowTemperatureLoads();

  bool setLowTemperatureSuctionPipingZone(const ThermalZone& thermalZone);

  void resetLowTemperatureSuctionPipingZone();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationTranscriticalSystem_Impl ImplType;

  explicit RefrigerationTranscriticalSystem(std::shared_ptr<detail::RefrigerationTranscriticalSystem_Impl> impl);

  friend class detail::RefrigerationTranscriticalSystem_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationTranscriticalSystem");
};

/** \relates RefrigerationTranscriticalSystem*/
typedef boost::optional<RefrigerationTranscriticalSystem> OptionalRefrigerationTranscriticalSystem;

/** \relates RefrigerationTranscriticalSystem*/
typedef std::vector<RefrigerationTranscriticalSystem> RefrigerationTranscriticalSystemVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_HPP

