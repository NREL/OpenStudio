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

#ifndef MODEL_RADIANCEPARAMETERS_IMPL_HPP
#define MODEL_RADIANCEPARAMETERS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** RadianceParameters_Impl is a ModelObject_Impl that is the implementation class for RadianceParameters.*/
  class MODEL_API RadianceParameters_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(int accumulatedRaysperRecord READ accumulatedRaysperRecord WRITE setAccumulatedRaysperRecord RESET resetAccumulatedRaysperRecord);
    Q_PROPERTY(bool isAccumulatedRaysperRecordDefaulted READ isAccumulatedRaysperRecordDefaulted);

    Q_PROPERTY(double directThreshold READ directThreshold WRITE setDirectThreshold RESET resetDirectThreshold);
    Q_PROPERTY(openstudio::Quantity directThreshold_SI READ directThreshold_SI WRITE setDirectThreshold RESET resetDirectThreshold);
    Q_PROPERTY(openstudio::Quantity directThreshold_IP READ directThreshold_IP WRITE setDirectThreshold RESET resetDirectThreshold);
    Q_PROPERTY(bool isDirectThresholdDefaulted READ isDirectThresholdDefaulted);

    Q_PROPERTY(double directCertainty READ directCertainty WRITE setDirectCertainty RESET resetDirectCertainty);
    Q_PROPERTY(openstudio::Quantity directCertainty_SI READ directCertainty_SI WRITE setDirectCertainty RESET resetDirectCertainty);
    Q_PROPERTY(openstudio::Quantity directCertainty_IP READ directCertainty_IP WRITE setDirectCertainty RESET resetDirectCertainty);
    Q_PROPERTY(bool isDirectCertaintyDefaulted READ isDirectCertaintyDefaulted);

    Q_PROPERTY(double directJitter READ directJitter WRITE setDirectJitter RESET resetDirectJitter);
    Q_PROPERTY(openstudio::Quantity directJitter_SI READ directJitter_SI WRITE setDirectJitter RESET resetDirectJitter);
    Q_PROPERTY(openstudio::Quantity directJitter_IP READ directJitter_IP WRITE setDirectJitter RESET resetDirectJitter);
    Q_PROPERTY(bool isDirectJitterDefaulted READ isDirectJitterDefaulted);

    Q_PROPERTY(double directPretest READ directPretest WRITE setDirectPretest RESET resetDirectPretest);
    Q_PROPERTY(openstudio::Quantity directPretest_SI READ directPretest_SI WRITE setDirectPretest RESET resetDirectPretest);
    Q_PROPERTY(openstudio::Quantity directPretest_IP READ directPretest_IP WRITE setDirectPretest RESET resetDirectPretest);
    Q_PROPERTY(bool isDirectPretestDefaulted READ isDirectPretestDefaulted);

    Q_PROPERTY(int ambientBouncesVMX READ ambientBouncesVMX WRITE setAmbientBouncesVMX RESET resetAmbientBouncesVMX);
    Q_PROPERTY(bool isAmbientBouncesVMXDefaulted READ isAmbientBouncesVMXDefaulted);

    Q_PROPERTY(int ambientBouncesDMX READ ambientBouncesDMX WRITE setAmbientBouncesDMX RESET resetAmbientBouncesDMX);
    Q_PROPERTY(bool isAmbientBouncesDMXDefaulted READ isAmbientBouncesDMXDefaulted);

    Q_PROPERTY(int ambientDivisionsVMX READ ambientDivisionsVMX WRITE setAmbientDivisionsVMX RESET resetAmbientDivisionsVMX);
    Q_PROPERTY(bool isAmbientDivisionsVMXDefaulted READ isAmbientDivisionsVMXDefaulted);

    Q_PROPERTY(int ambientDivisionsDMX READ ambientDivisionsDMX WRITE setAmbientDivisionsDMX RESET resetAmbientDivisionsDMX);
    Q_PROPERTY(bool isAmbientDivisionsDMXDefaulted READ isAmbientDivisionsDMXDefaulted);

    Q_PROPERTY(int ambientSupersamples READ ambientSupersamples WRITE setAmbientSupersamples RESET resetAmbientSupersamples);
    Q_PROPERTY(bool isAmbientSupersamplesDefaulted READ isAmbientSupersamplesDefaulted);

    Q_PROPERTY(double limitWeightVMX READ limitWeightVMX WRITE setLimitWeightVMX RESET resetLimitWeightVMX);
    Q_PROPERTY(openstudio::Quantity limitWeightVMX_SI READ limitWeightVMX_SI WRITE setLimitWeightVMX RESET resetLimitWeightVMX);
    Q_PROPERTY(openstudio::Quantity limitWeightVMX_IP READ limitWeightVMX_IP WRITE setLimitWeightVMX RESET resetLimitWeightVMX);
    Q_PROPERTY(bool isLimitWeightVMXDefaulted READ isLimitWeightVMXDefaulted);

    Q_PROPERTY(double limitWeightDMX READ limitWeightDMX WRITE setLimitWeightDMX RESET resetLimitWeightDMX);
    Q_PROPERTY(openstudio::Quantity limitWeightDMX_SI READ limitWeightDMX_SI WRITE setLimitWeightDMX RESET resetLimitWeightDMX);
    Q_PROPERTY(openstudio::Quantity limitWeightDMX_IP READ limitWeightDMX_IP WRITE setLimitWeightDMX RESET resetLimitWeightDMX);
    Q_PROPERTY(bool isLimitWeightDMXDefaulted READ isLimitWeightDMXDefaulted);

    Q_PROPERTY(int klemsSamplingDensity READ klemsSamplingDensity WRITE setKlemsSamplingDensity RESET resetKlemsSamplingDensity);
    Q_PROPERTY(bool isKlemsSamplingDensityDefaulted READ isKlemsSamplingDensityDefaulted);

    Q_PROPERTY(std::string skyDiscretizationResolution READ skyDiscretizationResolution WRITE setSkyDiscretizationResolution RESET resetSkyDiscretizationResolution);
    Q_PROPERTY(std::vector<std::string> skyDiscretizationResolutionValues READ skyDiscretizationResolutionValues);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    RadianceParameters_Impl(const IdfObject& idfObject,
                            Model_Impl* model,
                            bool keepHandle);

    RadianceParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    RadianceParameters_Impl(const RadianceParameters_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    virtual ~RadianceParameters_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    int accumulatedRaysperRecord() const;

    bool isAccumulatedRaysperRecordDefaulted() const;

    double directThreshold() const;

    Quantity getDirectThreshold(bool returnIP=false) const;

    bool isDirectThresholdDefaulted() const;

    double directCertainty() const;

    Quantity getDirectCertainty(bool returnIP=false) const;

    bool isDirectCertaintyDefaulted() const;

    double directJitter() const;

    Quantity getDirectJitter(bool returnIP=false) const;

    bool isDirectJitterDefaulted() const;

    double directPretest() const;

    Quantity getDirectPretest(bool returnIP=false) const;

    bool isDirectPretestDefaulted() const;

    int ambientBouncesVMX() const;

    bool isAmbientBouncesVMXDefaulted() const;

    int ambientBouncesDMX() const;

    bool isAmbientBouncesDMXDefaulted() const;

    int ambientDivisionsVMX() const;

    bool isAmbientDivisionsVMXDefaulted() const;

    int ambientDivisionsDMX() const;

    bool isAmbientDivisionsDMXDefaulted() const;

    int ambientSupersamples() const;

    bool isAmbientSupersamplesDefaulted() const;

    double limitWeightVMX() const;

    Quantity getLimitWeightVMX(bool returnIP=false) const;

    bool isLimitWeightVMXDefaulted() const;

    double limitWeightDMX() const;

    Quantity getLimitWeightDMX(bool returnIP=false) const;

    bool isLimitWeightDMXDefaulted() const;

    int klemsSamplingDensity() const;

    bool isKlemsSamplingDensityDefaulted() const;

    std::string skyDiscretizationResolution() const;

    //@}
    /** @name Setters */
    //@{

    bool setAccumulatedRaysperRecord(int accumulatedRaysperRecord);

    void resetAccumulatedRaysperRecord();

    bool setDirectThreshold(double directThreshold);

    bool setDirectThreshold(const Quantity& directThreshold);

    void resetDirectThreshold();

    bool setDirectCertainty(double directCertainty);

    bool setDirectCertainty(const Quantity& directCertainty);

    void resetDirectCertainty();

    bool setDirectJitter(double directJitter);

    bool setDirectJitter(const Quantity& directJitter);

    void resetDirectJitter();

    bool setDirectPretest(double directPretest);

    bool setDirectPretest(const Quantity& directPretest);

    void resetDirectPretest();

    bool setAmbientBouncesVMX(int ambientBouncesVMX);

    void resetAmbientBouncesVMX();

    bool setAmbientBouncesDMX(int ambientBouncesDMX);

    void resetAmbientBouncesDMX();

    bool setAmbientDivisionsVMX(int ambientDivisionsVMX);

    void resetAmbientDivisionsVMX();

    bool setAmbientDivisionsDMX(int ambientDivisionsDMX);

    void resetAmbientDivisionsDMX();

    bool setAmbientSupersamples(int ambientSupersamples);

    void resetAmbientSupersamples();

    bool setLimitWeightVMX(double limitWeightVMX);

    bool setLimitWeightVMX(const Quantity& limitWeightVMX);

    void resetLimitWeightVMX();

    void setLimitWeightDMX(double limitWeightDMX);

    bool setLimitWeightDMX(const Quantity& limitWeightDMX);

    void resetLimitWeightDMX();

    bool setKlemsSamplingDensity(int klemsSamplingDensity);

    void resetKlemsSamplingDensity();

    bool setSkyDiscretizationResolution(std::string skyDiscretizationResolution);

    void resetSkyDiscretizationResolution();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RadianceParameters");

    openstudio::Quantity directThreshold_SI() const;
    openstudio::Quantity directThreshold_IP() const;
    openstudio::Quantity directCertainty_SI() const;
    openstudio::Quantity directCertainty_IP() const;
    openstudio::Quantity directJitter_SI() const;
    openstudio::Quantity directJitter_IP() const;
    openstudio::Quantity directPretest_SI() const;
    openstudio::Quantity directPretest_IP() const;
    openstudio::Quantity limitWeightVMX_SI() const;
    openstudio::Quantity limitWeightVMX_IP() const;
    openstudio::Quantity limitWeightDMX_SI() const;
    openstudio::Quantity limitWeightDMX_IP() const;
    std::vector<std::string> skyDiscretizationResolutionValues() const;
 };

} // detail

} // model
} // openstudio

#endif // MODEL_RADIANCEPARAMETERS_IMPL_HPP

