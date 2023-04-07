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

#ifndef MODEL_GENERATORFUELCELL_HPP
#define MODEL_GENERATORFUELCELL_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

#include "GeneratorFuelCellPowerModule.hpp"
#include "GeneratorFuelCellPowerModule_Impl.hpp"
#include "GeneratorFuelCellAirSupply.hpp"
#include "GeneratorFuelCellAirSupply_Impl.hpp"
#include "GeneratorFuelCellWaterSupply.hpp"
#include "GeneratorFuelCellWaterSupply_Impl.hpp"
#include "GeneratorFuelCellAuxiliaryHeater.hpp"
#include "GeneratorFuelCellAuxiliaryHeater_Impl.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "GeneratorFuelCellElectricalStorage.hpp"
#include "GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "GeneratorFuelCellInverter.hpp"
#include "GeneratorFuelCellInverter_Impl.hpp"
#include "GeneratorFuelCellStackCooler.hpp"
#include "GeneratorFuelCellStackCooler_Impl.hpp"
#include "GeneratorFuelSupply.hpp"
#include "GeneratorFuelSupply_Impl.hpp"

namespace openstudio {
namespace model {

  class GeneratorFuelCellPowerModule;
  class GeneratorFuelCellAirSupply;
  class GeneratorFuelSupply;
  class GeneratorFuelCellWaterSupply;
  class GeneratorFuelCellAuxiliaryHeater;
  class GeneratorFuelCellExhaustGasToWaterHeatExchanger;
  class GeneratorFuelCellElectricalStorage;
  class GeneratorFuelCellInverter;
  class GeneratorFuelCellStackCooler;

  namespace detail {

    class GeneratorFuelCell_Impl;

  }  // namespace detail

  /** GeneratorFuelCell is a Generator that wraps the OpenStudio IDD object 'OS:Generator:FuelCell'. */
  class MODEL_API GeneratorFuelCell : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCell(const Model& model, const GeneratorFuelCellPowerModule& fCPM, const GeneratorFuelCellAirSupply& fCAS,
                               const GeneratorFuelCellWaterSupply& fCWS, const GeneratorFuelCellAuxiliaryHeater& fCAH,
                               const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX, const GeneratorFuelCellElectricalStorage& fCES,
                               const GeneratorFuelCellInverter& fCInverter, const GeneratorFuelSupply& fS);

    explicit GeneratorFuelCell(const Model& model);

    virtual ~GeneratorFuelCell() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCell(const GeneratorFuelCell& other) = default;
    GeneratorFuelCell(GeneratorFuelCell&& other) = default;
    GeneratorFuelCell& operator=(const GeneratorFuelCell&) = default;
    GeneratorFuelCell& operator=(GeneratorFuelCell&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    GeneratorFuelCellPowerModule powerModule() const;

    GeneratorFuelCellAirSupply airSupply() const;

    GeneratorFuelSupply fuelSupply() const;

    GeneratorFuelCellWaterSupply waterSupply() const;

    GeneratorFuelCellAuxiliaryHeater auxiliaryHeater() const;

    GeneratorFuelCellExhaustGasToWaterHeatExchanger heatExchanger() const;

    GeneratorFuelCellElectricalStorage electricalStorage() const;

    GeneratorFuelCellInverter inverter() const;

    boost::optional<GeneratorFuelCellStackCooler> stackCooler() const;

    //@}
    /** @name Setters */
    //@{

    bool setPowerModule(const GeneratorFuelCellPowerModule& fCPM);

    bool setAirSupply(const GeneratorFuelCellAirSupply& fCAirSup);

    bool setFuelSupply(const GeneratorFuelSupply& genFuelSup);

    bool setWaterSupply(const GeneratorFuelCellWaterSupply& fCWaterSup);

    bool setAuxiliaryHeater(const GeneratorFuelCellAuxiliaryHeater& fCAuxHeat);

    bool setHeatExchanger(const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX);

    bool setElectricalStorage(const GeneratorFuelCellElectricalStorage& fCStorage);

    bool setInverter(const GeneratorFuelCellInverter& fCInverter);

    bool setStackCooler(const GeneratorFuelCellStackCooler& fCStackCooler);

    void resetStackCooler();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCell_Impl;

    explicit GeneratorFuelCell(std::shared_ptr<detail::GeneratorFuelCell_Impl> impl);

    friend class detail::GeneratorFuelCell_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCell");
  };

  /** \relates GeneratorFuelCell*/
  using OptionalGeneratorFuelCell = boost::optional<GeneratorFuelCell>;

  /** \relates GeneratorFuelCell*/
  using GeneratorFuelCellVector = std::vector<GeneratorFuelCell>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELL_HPP
