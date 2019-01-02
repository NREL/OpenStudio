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

#ifndef MODEL_BOILERSTEAM_IMPL_HPP
#define MODEL_BOILERSTEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  /** BoilerSteam_Impl is a StraightComponent_Impl that is the implementation class for BoilerSteam.*/
  class MODEL_API BoilerSteam_Impl : public StraightComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    BoilerSteam_Impl(const IdfObject& idfObject,
                     Model_Impl* model,
                     bool keepHandle);

    BoilerSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    BoilerSteam_Impl(const BoilerSteam_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~BoilerSteam_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual void autosize() override;

    virtual void applySizingValues() override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    std::string fuelType() const;

    boost::optional<double> maximumOperatingPressure() const;

    OSOptionalQuantity getMaximumOperatingPressure(bool returnIP=false) const;

    boost::optional<double> theoreticalEfficiency() const;

    OSOptionalQuantity getTheoreticalEfficiency(bool returnIP=false) const;

    boost::optional<double> designOutletSteamTemperature() const;

    OSOptionalQuantity getDesignOutletSteamTemperature(bool returnIP=false) const;

    boost::optional<double> nominalCapacity() const;

    OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

    bool isNominalCapacityAutosized() const;

    boost::optional<double> minimumPartLoadRatio() const;

    OSOptionalQuantity getMinimumPartLoadRatio(bool returnIP=false) const;

    boost::optional<double> maximumPartLoadRatio() const;

    OSOptionalQuantity getMaximumPartLoadRatio(bool returnIP=false) const;

    boost::optional<double> optimumPartLoadRatio() const;

    OSOptionalQuantity getOptimumPartLoadRatio(bool returnIP=false) const;

    boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const;

    OSOptionalQuantity getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;

    boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;

    OSOptionalQuantity getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;

    boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;

    OSOptionalQuantity getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;

    double sizingFactor() const;

    Quantity getSizingFactor(bool returnIP=false) const;

    bool isSizingFactorDefaulted() const;

    boost::optional<double> autosizedNominalCapacity() const ;

    std::string endUseSubcategory() const;


    //@}
    /** @name Setters */
    //@{

    bool setFuelType(std::string fuelType);

    bool setMaximumOperatingPressure(boost::optional<double> maximumOperatingPressure);

    bool setMaximumOperatingPressure(const OSOptionalQuantity& maximumOperatingPressure);

    void resetMaximumOperatingPressure();

    bool setTheoreticalEfficiency(boost::optional<double> theoreticalEfficiency);

    bool setTheoreticalEfficiency(const OSOptionalQuantity& theoreticalEfficiency);

    void resetTheoreticalEfficiency();

    bool setDesignOutletSteamTemperature(boost::optional<double> designOutletSteamTemperature);

    bool setDesignOutletSteamTemperature(const OSOptionalQuantity& designOutletSteamTemperature);

    void resetDesignOutletSteamTemperature();

    bool setNominalCapacity(boost::optional<double> nominalCapacity);

    bool setNominalCapacity(const OSOptionalQuantity& nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    bool setMinimumPartLoadRatio(boost::optional<double> minimumPartLoadRatio);

    bool setMinimumPartLoadRatio(const OSOptionalQuantity& minimumPartLoadRatio);

    void resetMinimumPartLoadRatio();

    bool setMaximumPartLoadRatio(boost::optional<double> maximumPartLoadRatio);

    bool setMaximumPartLoadRatio(const OSOptionalQuantity& maximumPartLoadRatio);

    void resetMaximumPartLoadRatio();

    bool setOptimumPartLoadRatio(boost::optional<double> optimumPartLoadRatio);

    bool setOptimumPartLoadRatio(const OSOptionalQuantity& optimumPartLoadRatio);

    void resetOptimumPartLoadRatio();

    bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

    bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

    void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

    bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

    bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

    void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

    bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

    bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

    void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

    bool setSizingFactor(double sizingFactor);

    bool setSizingFactor(const Quantity& sizingFactor);

    void resetSizingFactor();

    bool setEndUseSubcategory(const std::string & endUseSubcategory);


    //@}
    /** @name Other */
    //@{


    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.BoilerSteam");

    std::vector<std::string> fuelTypeValues() const;
    openstudio::OSOptionalQuantity maximumOperatingPressure_SI() const;
    openstudio::OSOptionalQuantity maximumOperatingPressure_IP() const;
    openstudio::OSOptionalQuantity theoreticalEfficiency_SI() const;
    openstudio::OSOptionalQuantity theoreticalEfficiency_IP() const;
    openstudio::OSOptionalQuantity designOutletSteamTemperature_SI() const;
    openstudio::OSOptionalQuantity designOutletSteamTemperature_IP() const;
    openstudio::OSOptionalQuantity nominalCapacity_SI() const;
    openstudio::OSOptionalQuantity nominalCapacity_IP() const;
    openstudio::OSOptionalQuantity minimumPartLoadRatio_SI() const;
    openstudio::OSOptionalQuantity minimumPartLoadRatio_IP() const;
    openstudio::OSOptionalQuantity maximumPartLoadRatio_SI() const;
    openstudio::OSOptionalQuantity maximumPartLoadRatio_IP() const;
    openstudio::OSOptionalQuantity optimumPartLoadRatio_SI() const;
    openstudio::OSOptionalQuantity optimumPartLoadRatio_IP() const;
    openstudio::OSOptionalQuantity coefficient1ofFuelUseFunctionofPartLoadRatioCurve_SI() const;
    openstudio::OSOptionalQuantity coefficient1ofFuelUseFunctionofPartLoadRatioCurve_IP() const;
    openstudio::OSOptionalQuantity coefficient2ofFuelUseFunctionofPartLoadRatioCurve_SI() const;
    openstudio::OSOptionalQuantity coefficient2ofFuelUseFunctionofPartLoadRatioCurve_IP() const;
    openstudio::OSOptionalQuantity coefficient3ofFuelUseFunctionofPartLoadRatioCurve_SI() const;
    openstudio::OSOptionalQuantity coefficient3ofFuelUseFunctionofPartLoadRatioCurve_IP() const;
    openstudio::Quantity sizingFactor_SI() const;
    openstudio::Quantity sizingFactor_IP() const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_BOILERSTEAM_IMPL_HPP
