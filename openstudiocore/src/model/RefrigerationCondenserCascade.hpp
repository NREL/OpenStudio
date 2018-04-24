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

#ifndef MODEL_REFRIGERATIONCONDENSERCASCADE_HPP
#define MODEL_REFRIGERATIONCONDENSERCASCADE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class RefrigerationCondenserCascade_Impl;

} // detail

/** RefrigerationCondenserCascade is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:Cascade'. */
class MODEL_API RefrigerationCondenserCascade : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationCondenserCascade(const Model& model);

  virtual ~RefrigerationCondenserCascade() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> condensingTemperatureControlTypeValues();

  /** @name Getters */
  //@{

  double ratedCondensingTemperature() const;

  double ratedApproachTemperatureDifference() const;

  bool isRatedApproachTemperatureDifferenceDefaulted() const;

  double ratedEffectiveTotalHeatRejectionRate() const;

  std::string condensingTemperatureControlType() const;

  bool isCondensingTemperatureControlTypeDefaulted() const;

  boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

  boost::optional<double> condensateReceiverRefrigerantInventory() const;

  boost::optional<double> condensatePipingRefrigerantInventory() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedCondensingTemperature(double ratedCondensingTemperature);

  bool setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference);

  void resetRatedApproachTemperatureDifference();

  bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

  bool setCondensingTemperatureControlType(std::string condensingTemperatureControlType);

  void resetCondensingTemperatureControlType();

  bool setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

  void resetCondenserRefrigerantOperatingChargeInventory();

  bool setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

  void resetCondensateReceiverRefrigerantInventory();

  bool setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

  void resetCondensatePipingRefrigerantInventory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationCondenserCascade_Impl ImplType;

  explicit RefrigerationCondenserCascade(std::shared_ptr<detail::RefrigerationCondenserCascade_Impl> impl);

  friend class detail::RefrigerationCondenserCascade_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationCondenserCascade");
};

/** \relates RefrigerationCondenserCascade*/
typedef boost::optional<RefrigerationCondenserCascade> OptionalRefrigerationCondenserCascade;

/** \relates RefrigerationCondenserCascade*/
typedef std::vector<RefrigerationCondenserCascade> RefrigerationCondenserCascadeVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSERCASCADE_HPP
