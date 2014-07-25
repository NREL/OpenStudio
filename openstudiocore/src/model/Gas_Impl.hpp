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

#ifndef MODEL_GAS_IMPL_HPP
#define MODEL_GAS_IMPL_HPP

#include "ModelAPI.hpp"
#include "GasLayer_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** Gas_Impl is a GasLayer_Impl that is the implementation class for Gas.*/
  class MODEL_API Gas_Impl : public GasLayer_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string gasType READ gasType WRITE setGasType);
    Q_PROPERTY(std::vector<std::string> gasTypeValues READ gasTypeValues);

    Q_PROPERTY(double thickness READ thickness WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_SI READ thickness_SI WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_IP READ thickness_IP WRITE setThickness);

    Q_PROPERTY(boost::optional<double> conductivityCoefficientA READ conductivityCoefficientA WRITE setConductivityCoefficientA RESET resetConductivityCoefficientA);
    Q_PROPERTY(openstudio::OSOptionalQuantity conductivityCoefficientA_SI READ conductivityCoefficientA_SI WRITE setConductivityCoefficientA RESET resetConductivityCoefficientA);
    Q_PROPERTY(openstudio::OSOptionalQuantity conductivityCoefficientA_IP READ conductivityCoefficientA_IP WRITE setConductivityCoefficientA RESET resetConductivityCoefficientA);

    Q_PROPERTY(boost::optional<double> conductivityCoefficientB READ conductivityCoefficientB WRITE setConductivityCoefficientB RESET resetConductivityCoefficientB);
    Q_PROPERTY(openstudio::OSOptionalQuantity conductivityCoefficientB_SI READ conductivityCoefficientB_SI WRITE setConductivityCoefficientB RESET resetConductivityCoefficientB);
    Q_PROPERTY(openstudio::OSOptionalQuantity conductivityCoefficientB_IP READ conductivityCoefficientB_IP WRITE setConductivityCoefficientB RESET resetConductivityCoefficientB);

    Q_PROPERTY(boost::optional<double> conductivityCoefficientC READ conductivityCoefficientC WRITE setConductivityCoefficientC RESET resetConductivityCoefficientC);
    Q_PROPERTY(openstudio::OSOptionalQuantity conductivityCoefficientC_SI READ conductivityCoefficientC_SI WRITE setConductivityCoefficientC RESET resetConductivityCoefficientC);
    Q_PROPERTY(openstudio::OSOptionalQuantity conductivityCoefficientC_IP READ conductivityCoefficientC_IP WRITE setConductivityCoefficientC RESET resetConductivityCoefficientC);

    Q_PROPERTY(boost::optional<double> viscosityCoefficientA READ viscosityCoefficientA WRITE setViscosityCoefficientA RESET resetViscosityCoefficientA);
    Q_PROPERTY(openstudio::OSOptionalQuantity viscosityCoefficientA_SI READ viscosityCoefficientA_SI WRITE setViscosityCoefficientA RESET resetViscosityCoefficientA);
    Q_PROPERTY(openstudio::OSOptionalQuantity viscosityCoefficientA_IP READ viscosityCoefficientA_IP WRITE setViscosityCoefficientA RESET resetViscosityCoefficientA);

    Q_PROPERTY(boost::optional<double> viscosityCoefficientB READ viscosityCoefficientB WRITE setViscosityCoefficientB RESET resetViscosityCoefficientB);
    Q_PROPERTY(openstudio::OSOptionalQuantity viscosityCoefficientB_SI READ viscosityCoefficientB_SI WRITE setViscosityCoefficientB RESET resetViscosityCoefficientB);
    Q_PROPERTY(openstudio::OSOptionalQuantity viscosityCoefficientB_IP READ viscosityCoefficientB_IP WRITE setViscosityCoefficientB RESET resetViscosityCoefficientB);

    Q_PROPERTY(boost::optional<double> viscosityCoefficientC READ viscosityCoefficientC WRITE setViscosityCoefficientC RESET resetViscosityCoefficientC);
    Q_PROPERTY(openstudio::OSOptionalQuantity viscosityCoefficientC_SI READ viscosityCoefficientC_SI WRITE setViscosityCoefficientC RESET resetViscosityCoefficientC);
    Q_PROPERTY(openstudio::OSOptionalQuantity viscosityCoefficientC_IP READ viscosityCoefficientC_IP WRITE setViscosityCoefficientC RESET resetViscosityCoefficientC);

    Q_PROPERTY(boost::optional<double> specificHeatCoefficientA READ specificHeatCoefficientA WRITE setSpecificHeatCoefficientA RESET resetSpecificHeatCoefficientA);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatCoefficientA_SI READ specificHeatCoefficientA_SI WRITE setSpecificHeatCoefficientA RESET resetSpecificHeatCoefficientA);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatCoefficientA_IP READ specificHeatCoefficientA_IP WRITE setSpecificHeatCoefficientA RESET resetSpecificHeatCoefficientA);

    Q_PROPERTY(boost::optional<double> specificHeatCoefficientB READ specificHeatCoefficientB WRITE setSpecificHeatCoefficientB RESET resetSpecificHeatCoefficientB);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatCoefficientB_SI READ specificHeatCoefficientB_SI WRITE setSpecificHeatCoefficientB RESET resetSpecificHeatCoefficientB);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatCoefficientB_IP READ specificHeatCoefficientB_IP WRITE setSpecificHeatCoefficientB RESET resetSpecificHeatCoefficientB);

    Q_PROPERTY(boost::optional<double> specificHeatCoefficientC READ specificHeatCoefficientC WRITE setSpecificHeatCoefficientC RESET resetSpecificHeatCoefficientC);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatCoefficientC_SI READ specificHeatCoefficientC_SI WRITE setSpecificHeatCoefficientC RESET resetSpecificHeatCoefficientC);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatCoefficientC_IP READ specificHeatCoefficientC_IP WRITE setSpecificHeatCoefficientC RESET resetSpecificHeatCoefficientC);

    Q_PROPERTY(boost::optional<double> molecularWeight READ molecularWeight WRITE setMolecularWeight RESET resetMolecularWeight);
    Q_PROPERTY(openstudio::OSOptionalQuantity molecularWeight_SI READ molecularWeight_SI WRITE setMolecularWeight RESET resetMolecularWeight);
    Q_PROPERTY(openstudio::OSOptionalQuantity molecularWeight_IP READ molecularWeight_IP WRITE setMolecularWeight RESET resetMolecularWeight);

    Q_PROPERTY(boost::optional<double> specificHeatRatio READ specificHeatRatio WRITE setSpecificHeatRatio RESET resetSpecificHeatRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatRatio_SI READ specificHeatRatio_SI WRITE setSpecificHeatRatio RESET resetSpecificHeatRatio);
    Q_PROPERTY(openstudio::OSOptionalQuantity specificHeatRatio_IP READ specificHeatRatio_IP WRITE setSpecificHeatRatio RESET resetSpecificHeatRatio);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    Gas_Impl(const IdfObject& idfObject,
             Model_Impl* model,
             bool keepHandle);

    Gas_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
             Model_Impl* model,
             bool keepHandle);

    Gas_Impl(const Gas_Impl& other,
             Model_Impl* model,
             bool keepHandle);

    virtual ~Gas_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    /** Gets the gas type. Throws if value is not available. */
    std::string gasType() const;

    /** Get the thickness of the material. Will throw if value is not available. */
    virtual double thickness() const;

    /** The conductivity (W/m*K) at temperature (K). Will throw if necessary values are not 
     *  available. */
    double getThermalConductivity(double temperature) const;

    /** The conductance (W/m^2*K) at temperature (K). Will throw if necessary values are not 
     *  available. */
    double getThermalConductance(double temperature) const;

    /** The resistivity (m*K/W) at temperature (K). Will throw if necessary values are not 
     *  available. */
    double getThermalResistivity(double temperature) const;

    /** The resistance (m^2*K/W) at temperature (K). Will throw if necessary values are not 
     *  available. */
    double getThermalResistance(double temperature) const;

    double getViscosity(double temperature) const;

    double getSpecificHeat(double temperature) const;

    boost::optional<double> customConductivityCoefficientA() const;

    boost::optional<double> customConductivityCoefficientB() const;

    boost::optional<double> customConductivityCoefficientC() const;

    boost::optional<double> customViscosityCoefficientA() const;

    boost::optional<double> customViscosityCoefficientB() const;

    boost::optional<double> customViscosityCoefficientC() const;

    boost::optional<double> customSpecificHeatCoefficientA() const;

    boost::optional<double> customSpecificHeatCoefficientB() const;

    boost::optional<double> customSpecificHeatCoefficientC() const;

    boost::optional<double> customMolecularWeight() const;

    Quantity getThickness(bool returnIP=false) const;

    boost::optional<double> conductivityCoefficientA() const;

    OSOptionalQuantity getConductivityCoefficientA(bool returnIP=false) const;

    boost::optional<double> conductivityCoefficientB() const;

    OSOptionalQuantity getConductivityCoefficientB(bool returnIP=false) const;

    boost::optional<double> conductivityCoefficientC() const;

    OSOptionalQuantity getConductivityCoefficientC(bool returnIP=false) const;

    boost::optional<double> viscosityCoefficientA() const;

    OSOptionalQuantity getViscosityCoefficientA(bool returnIP=false) const;

    boost::optional<double> viscosityCoefficientB() const;

    OSOptionalQuantity getViscosityCoefficientB(bool returnIP=false) const;

    boost::optional<double> viscosityCoefficientC() const;

    OSOptionalQuantity getViscosityCoefficientC(bool returnIP=false) const;

    boost::optional<double> specificHeatCoefficientA() const;

    OSOptionalQuantity getSpecificHeatCoefficientA(bool returnIP=false) const;

    boost::optional<double> specificHeatCoefficientB() const;

    OSOptionalQuantity getSpecificHeatCoefficientB(bool returnIP=false) const;

    boost::optional<double> specificHeatCoefficientC() const;

    OSOptionalQuantity getSpecificHeatCoefficientC(bool returnIP=false) const;

    boost::optional<double> molecularWeight() const;

    OSOptionalQuantity getMolecularWeight(bool returnIP=false) const;

    boost::optional<double> specificHeatRatio() const;

    OSOptionalQuantity getSpecificHeatRatio(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    virtual bool setThickness(double value);

    bool setCustomConductivity(double A, double B, double C);

    bool setCustomViscosity(double A, double B, double C);

    bool setCustomSpecificHeat(double A, double B, double C);

    bool setCustomMolecularWeight(double value);

    bool setGasType(std::string gasType);

    bool setThickness(const Quantity& thickness);

    void setConductivityCoefficientA(boost::optional<double> conductivityCoefficientA);

    bool setConductivityCoefficientA(const OSOptionalQuantity& conductivityCoefficientA);

    void resetConductivityCoefficientA();

    void setConductivityCoefficientB(boost::optional<double> conductivityCoefficientB);

    bool setConductivityCoefficientB(const OSOptionalQuantity& conductivityCoefficientB);

    void resetConductivityCoefficientB();

    void setConductivityCoefficientC(boost::optional<double> conductivityCoefficientC);

    bool setConductivityCoefficientC(const OSOptionalQuantity& conductivityCoefficientC);

    void resetConductivityCoefficientC();

    bool setViscosityCoefficientA(boost::optional<double> viscosityCoefficientA);

    bool setViscosityCoefficientA(const OSOptionalQuantity& viscosityCoefficientA);

    void resetViscosityCoefficientA();

    void setViscosityCoefficientB(boost::optional<double> viscosityCoefficientB);

    bool setViscosityCoefficientB(const OSOptionalQuantity& viscosityCoefficientB);

    void resetViscosityCoefficientB();

    void setViscosityCoefficientC(boost::optional<double> viscosityCoefficientC);

    bool setViscosityCoefficientC(const OSOptionalQuantity& viscosityCoefficientC);

    void resetViscosityCoefficientC();

    bool setSpecificHeatCoefficientA(boost::optional<double> specificHeatCoefficientA);

    bool setSpecificHeatCoefficientA(const OSOptionalQuantity& specificHeatCoefficientA);

    void resetSpecificHeatCoefficientA();

    void setSpecificHeatCoefficientB(boost::optional<double> specificHeatCoefficientB);

    bool setSpecificHeatCoefficientB(const OSOptionalQuantity& specificHeatCoefficientB);

    void resetSpecificHeatCoefficientB();

    void setSpecificHeatCoefficientC(boost::optional<double> specificHeatCoefficientC);

    bool setSpecificHeatCoefficientC(const OSOptionalQuantity& specificHeatCoefficientC);

    void resetSpecificHeatCoefficientC();

    bool setMolecularWeight(boost::optional<double> molecularWeight);

    bool setMolecularWeight(const OSOptionalQuantity& molecularWeight);

    void resetMolecularWeight();

    bool setSpecificHeatRatio(boost::optional<double> specificHeatRatio);

    bool setSpecificHeatRatio(const OSOptionalQuantity& specificHeatRatio);

    void resetSpecificHeatRatio();

    //@}
    /** @name Queries */
    //@{

    /** Returns a ValidityReport for this Gas object containing all errors at or below level. 
     *  Adds on to the WorkspaceObject requirements by ensuring that custom values are specified
     *  if needed at StrictnessLevel::Final. */
    virtual ValidityReport validityReport(StrictnessLevel level,bool checkNames) const;

    //@}
    /** @name Other */
    //@{

    //@}
   protected:

    virtual void populateValidityReport(ValidityReport& report,bool checkNames) const;

   private:
    REGISTER_LOGGER("openstudio.model.Gas");

    std::vector<std::string> gasTypeValues() const;
    openstudio::Quantity thickness_SI() const;
    openstudio::Quantity thickness_IP() const;
    openstudio::OSOptionalQuantity conductivityCoefficientA_SI() const;
    openstudio::OSOptionalQuantity conductivityCoefficientA_IP() const;
    openstudio::OSOptionalQuantity conductivityCoefficientB_SI() const;
    openstudio::OSOptionalQuantity conductivityCoefficientB_IP() const;
    openstudio::OSOptionalQuantity conductivityCoefficientC_SI() const;
    openstudio::OSOptionalQuantity conductivityCoefficientC_IP() const;
    openstudio::OSOptionalQuantity viscosityCoefficientA_SI() const;
    openstudio::OSOptionalQuantity viscosityCoefficientA_IP() const;
    openstudio::OSOptionalQuantity viscosityCoefficientB_SI() const;
    openstudio::OSOptionalQuantity viscosityCoefficientB_IP() const;
    openstudio::OSOptionalQuantity viscosityCoefficientC_SI() const;
    openstudio::OSOptionalQuantity viscosityCoefficientC_IP() const;
    openstudio::OSOptionalQuantity specificHeatCoefficientA_SI() const;
    openstudio::OSOptionalQuantity specificHeatCoefficientA_IP() const;
    openstudio::OSOptionalQuantity specificHeatCoefficientB_SI() const;
    openstudio::OSOptionalQuantity specificHeatCoefficientB_IP() const;
    openstudio::OSOptionalQuantity specificHeatCoefficientC_SI() const;
    openstudio::OSOptionalQuantity specificHeatCoefficientC_IP() const;
    openstudio::OSOptionalQuantity molecularWeight_SI() const;
    openstudio::OSOptionalQuantity molecularWeight_IP() const;
    openstudio::OSOptionalQuantity specificHeatRatio_SI() const;
    openstudio::OSOptionalQuantity specificHeatRatio_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GAS_IMPL_HPP

