/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_GENERATORFUELCELL_IMPL_HPP
#define MODEL_GENERATORFUELCELL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class FCPM;
class FCAirSup;
class GenFuelSup;
class FCWaterSup;
class FCAuxHeat;
class FCExhaustHX;
class FCStorage;
class FCInverter;
class FCStackCooler;

namespace detail {

  /** GeneratorFuelCell_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCell.*/
  class MODEL_API GeneratorFuelCell_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    GeneratorFuelCell_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

    GeneratorFuelCell_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    GeneratorFuelCell_Impl(const GeneratorFuelCell_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~GeneratorFuelCell_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: FCPM.
    FCPM powerModule() const;

    // TODO: Check return type. From object lists, some candidates are: FCAirSup.
    FCAirSup airSupply() const;

    // TODO: Check return type. From object lists, some candidates are: GenFuelSup.
    GenFuelSup fuelSupply() const;

    // TODO: Check return type. From object lists, some candidates are: FCWaterSup.
    FCWaterSup waterSupply() const;

    // TODO: Check return type. From object lists, some candidates are: FCAuxHeat.
    FCAuxHeat auxiliaryHeater() const;

    // TODO: Check return type. From object lists, some candidates are: FCExhaustHX.
    FCExhaustHX heatExchanger() const;

    // TODO: Check return type. From object lists, some candidates are: FCStorage.
    FCStorage electricalStorage() const;

    // TODO: Check return type. From object lists, some candidates are: FCInverter.
    FCInverter inverter() const;

    // TODO: Check return type. From object lists, some candidates are: FCStackCooler.
    boost::optional<FCStackCooler> stackCooler() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: FCPM.
    bool setPowerModule(const FCPM& fCPM);

    // TODO: Check argument type. From object lists, some candidates are: FCAirSup.
    bool setAirSupply(const FCAirSup& fCAirSup);

    // TODO: Check argument type. From object lists, some candidates are: GenFuelSup.
    bool setFuelSupply(const GenFuelSup& genFuelSup);

    // TODO: Check argument type. From object lists, some candidates are: FCWaterSup.
    bool setWaterSupply(const FCWaterSup& fCWaterSup);

    // TODO: Check argument type. From object lists, some candidates are: FCAuxHeat.
    bool setAuxiliaryHeater(const FCAuxHeat& fCAuxHeat);

    // TODO: Check argument type. From object lists, some candidates are: FCExhaustHX.
    bool setHeatExchanger(const FCExhaustHX& fCExhaustHX);

    // TODO: Check argument type. From object lists, some candidates are: FCStorage.
    bool setElectricalStorage(const FCStorage& fCStorage);

    // TODO: Check argument type. From object lists, some candidates are: FCInverter.
    bool setInverter(const FCInverter& fCInverter);

    // TODO: Check argument type. From object lists, some candidates are: FCStackCooler.
    bool setStackCooler(const FCStackCooler& fCStackCooler);

    void resetStackCooler();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCell");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<FCPM> optionalPowerModule() const;
    boost::optional<FCAirSup> optionalAirSupply() const;
    boost::optional<GenFuelSup> optionalFuelSupply() const;
    boost::optional<FCWaterSup> optionalWaterSupply() const;
    boost::optional<FCAuxHeat> optionalAuxiliaryHeater() const;
    boost::optional<FCExhaustHX> optionalHeatExchanger() const;
    boost::optional<FCStorage> optionalElectricalStorage() const;
    boost::optional<FCInverter> optionalInverter() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELL_IMPL_HPP

