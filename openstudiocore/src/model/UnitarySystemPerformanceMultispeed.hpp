/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/
 
#ifndef MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_HPP
#define MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace energyplus {
  class ForwardTranslator;
}
  
namespace model {

namespace detail {
  class UnitarySystemPerformanceMultispeed_Impl;
} // detail


class MODEL_API SupplyAirflowRatioField {
 public:

  /// Default Constructor. Autosizes both heating and cooling.
  SupplyAirflowRatioField();

  /// Constructor that sets values for both heating and cooling.
  SupplyAirflowRatioField(double heatingRatio, double coolingRatio);

  /// "Constructor" that autosizes cooling ratio and sets heating ratio.
  static SupplyAirflowRatioField fromHeatingRatio(double heatingRatio);

  /// "Constructor" that autosizes heating ration and sets cooling ratio.
  static SupplyAirflowRatioField fromCoolingRatio(double coolingRatio);

  /** @name Getters */
  //@{
  boost::optional<double> heatingRatio();
  boost::optional<double> coolingRatio();
  bool isHeatingRatioAutosized();
  bool isCoolingRatioAutosized();
  //@}

 protected:
  std::vector<std::string> getHeatingCoolingRatiosAsStrings() const;

  friend class detail::UnitarySystemPerformanceMultispeed_Impl;
  friend class openstudio::energyplus::ForwardTranslator;

 private:
  SupplyAirflowRatioField(bool isHeating, double value);
  boost::optional<double> m_heatingRatio; // implementation detail, boost::none if autosized, otherwise the value
  boost::optional<double> m_coolingRatio;
};

/** UnitarySystemPerformanceMultispeed is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:UnitarySystemPerformance:Multispeed'. */
class MODEL_API UnitarySystemPerformanceMultispeed : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{
  explicit UnitarySystemPerformanceMultispeed(const Model& model);
  
  virtual ~UnitarySystemPerformanceMultispeed() {}
  //@}
  
  static IddObjectType iddObjectType();
  
  /** @name Getters */
  //@{

  bool singleModeOperation() const;

  std::vector<SupplyAirflowRatioField> supplyAirflowRatioFields();
  
  //@}
  
  //** @name Setters */

  bool setSingleModeOperation(bool singleMode);

  void resetSingleModeOperation();

  bool setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields);

  bool addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio);

  bool addSupplyAirflowRatioField(double heatingRatio, double coolingRatio);

  void resetSupplyAirflowRatioFields();
  
  //@{
  
  //@}

 protected:
  /// @cond
  typedef detail::UnitarySystemPerformanceMultispeed_Impl ImplType;
  
  explicit UnitarySystemPerformanceMultispeed(std::shared_ptr<detail::UnitarySystemPerformanceMultispeed_Impl> impl);
  
  friend class detail::UnitarySystemPerformanceMultispeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.UnitarySystemPerformanceMultispeed");
};

/** \relates UnitarySystemPerformanceMultispeed */
typedef boost::optional<UnitarySystemPerformanceMultispeed> OptionalUnitarySystemPerformanceMultispeed;

/** \relates UnitarySystemPerformanceMultispeed */
typedef std::vector<UnitarySystemPerformanceMultispeed> UnitarySystemPerformanceMultispeedVector;

} // model
} // openstudio

#endif // MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_HPP
