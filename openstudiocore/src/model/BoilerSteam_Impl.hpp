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
    Q_OBJECT;

    Q_PROPERTY(std::string fuelType READ fuelType WRITE setFuelType);
    Q_PROPERTY(std::vector<std::string> fuelTypeValues READ fuelTypeValues);

    Q_PROPERTY(boost::optional<double> maximumOperatingPressure READ maximumOperatingPressure WRITE setMaximumOperatingPressure RESET resetMaximumOperatingPressure);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumOperatingPressure_SI READ maximumOperatingPressure_SI WRITE setMaximumOperatingPressure RESET resetMaximumOperatingPressure);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumOperatingPressure_IP READ maximumOperatingPressure_IP WRITE setMaximumOperatingPressure RESET resetMaximumOperatingPressure);

    Q_PROPERTY(boost::optional<double> theoreticalEfficiency READ theoreticalEfficiency WRITE setTheoreticalEfficiency RESET resetTheoreticalEfficiency);
    Q_PROPERTY(openstudio::OSOptionalQuantity theoreticalEfficiency_SI READ theoreticalEfficiency_SI WRITE setTheoreticalEfficiency RESET resetTheoreticalEfficiency);
    Q_PROPERTY(openstudio::OSOptionalQuantity theoreticalEfficiency_IP READ theoreticalEfficiency_IP WRITE setTheoreticalEfficiency RESET resetTheoreticalEfficiency);

    Q_PROPERTY(boost::optional<double> designOutletSteamTemperature READ designOutletSteamTemperature WRITE setDesignOutletSteamTemperature RESET resetDesignOutletSteamTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity designOutletSteamTemperature_SI READ designOutletSteamTemperature_SI WRITE setDesignOutletSteamTemperature RESET resetDesignOutletSteamTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity designOutletSteamTemperature_IP READ designOutletSteamTemperature_IP WRITE setDesignOutletSteamTemperature RESET resetDesignOutletSteamTemperature);

    Q_PROPERTY(boost::optional<double> nominalCapacity READ nominalCapacity WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalCapacity_SI READ nominalCapacity_SI WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalCapacity_IP READ nominalCapacity_IP WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(bool isNominalCapacityAutosized READ isNominalCapacityAutosized);

    Q_PROPERTY(boost::optional<double> minimumPartLoadRatio READ minimumPartLoadRatio WRITE setMinimumPartLoadRatio RESET resetMinimumPartLoadRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity minimumPartLoadRatio_SI READ minimumPartLoadRatio_SI WRITE setMinimumPartLoadRatio RESET resetMinimumPartLoadRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity minimumPartLoadRatio_IP READ minimumPartLoadRatio_IP WRITE setMinimumPartLoadRatio RESET resetMinimumPartLoadRatio);

    Q_PROPERTY(boost::optional<double> maximumPartLoadRatio READ maximumPartLoadRatio WRITE setMaximumPartLoadRatio RESET resetMaximumPartLoadRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumPartLoadRatio_SI READ maximumPartLoadRatio_SI WRITE setMaximumPartLoadRatio RESET resetMaximumPartLoadRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumPartLoadRatio_IP READ maximumPartLoadRatio_IP WRITE setMaximumPartLoadRatio RESET resetMaximumPartLoadRatio);

    Q_PROPERTY(boost::optional<double> optimumPartLoadRatio READ optimumPartLoadRatio WRITE setOptimumPartLoadRatio RESET resetOptimumPartLoadRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity optimumPartLoadRatio_SI READ optimumPartLoadRatio_SI WRITE setOptimumPartLoadRatio RESET resetOptimumPartLoadRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity optimumPartLoadRatio_IP READ optimumPartLoadRatio_IP WRITE setOptimumPartLoadRatio RESET resetOptimumPartLoadRatio);

    Q_PROPERTY(boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve READ coefficient1ofFuelUseFunctionofPartLoadRatioCurve WRITE setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve);
    Q_PROPERTY(openstudio::OSOptionalQuantity coefficient1ofFuelUseFunctionofPartLoadRatioCurve_SI READ coefficient1ofFuelUseFunctionofPartLoadRatioCurve_SI WRITE setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve);
    Q_PROPERTY(openstudio::OSOptionalQuantity coefficient1ofFuelUseFunctionofPartLoadRatioCurve_IP READ coefficient1ofFuelUseFunctionofPartLoadRatioCurve_IP WRITE setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve);

    Q_PROPERTY(boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve READ coefficient2ofFuelUseFunctionofPartLoadRatioCurve WRITE setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve);
    Q_PROPERTY(openstudio::OSOptionalQuantity coefficient2ofFuelUseFunctionofPartLoadRatioCurve_SI READ coefficient2ofFuelUseFunctionofPartLoadRatioCurve_SI WRITE setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve);
    Q_PROPERTY(openstudio::OSOptionalQuantity coefficient2ofFuelUseFunctionofPartLoadRatioCurve_IP READ coefficient2ofFuelUseFunctionofPartLoadRatioCurve_IP WRITE setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve);

    Q_PROPERTY(boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve READ coefficient3ofFuelUseFunctionofPartLoadRatioCurve WRITE setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve);
    Q_PROPERTY(openstudio::OSOptionalQuantity coefficient3ofFuelUseFunctionofPartLoadRatioCurve_SI READ coefficient3ofFuelUseFunctionofPartLoadRatioCurve_SI WRITE setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve);
    Q_PROPERTY(openstudio::OSOptionalQuantity coefficient3ofFuelUseFunctionofPartLoadRatioCurve_IP READ coefficient3ofFuelUseFunctionofPartLoadRatioCurve_IP WRITE setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve RESET resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve);

    Q_PROPERTY(double sizingFactor READ sizingFactor WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(openstudio::Quantity sizingFactor_SI READ sizingFactor_SI WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(openstudio::Quantity sizingFactor_IP READ sizingFactor_IP WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(bool isSizingFactorDefaulted READ isSizingFactorDefaulted);

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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

    //@}
    /** @name Setters */
    //@{

    bool setFuelType(std::string fuelType);

    void setMaximumOperatingPressure(boost::optional<double> maximumOperatingPressure);

    bool setMaximumOperatingPressure(const OSOptionalQuantity& maximumOperatingPressure);

    void resetMaximumOperatingPressure();

    bool setTheoreticalEfficiency(boost::optional<double> theoreticalEfficiency);

    bool setTheoreticalEfficiency(const OSOptionalQuantity& theoreticalEfficiency);

    void resetTheoreticalEfficiency();

    void setDesignOutletSteamTemperature(boost::optional<double> designOutletSteamTemperature);

    bool setDesignOutletSteamTemperature(const OSOptionalQuantity& designOutletSteamTemperature);

    void resetDesignOutletSteamTemperature();

    void setNominalCapacity(boost::optional<double> nominalCapacity);

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

    void setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

    bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

    void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

    void setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

    bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

    void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

    void setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

    bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

    void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

    bool setSizingFactor(double sizingFactor);

    bool setSizingFactor(const Quantity& sizingFactor);

    void resetSizingFactor();

    //@}
    /** @name Other */
    //@{

    unsigned inletPort();

    unsigned outletPort();

    bool addToNode(Node & node);

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

