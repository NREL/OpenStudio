/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RADIANCEPARAMETERS_IMPL_HPP
#define MODEL_RADIANCEPARAMETERS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** RadianceParameters_Impl is a ModelObject_Impl that is the implementation class for RadianceParameters.*/
    class MODEL_API RadianceParameters_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      RadianceParameters_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RadianceParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RadianceParameters_Impl(const RadianceParameters_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RadianceParameters_Impl() override;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      int accumulatedRaysperRecord() const;

      bool isAccumulatedRaysperRecordDefaulted() const;

      double directThreshold() const;

      bool isDirectThresholdDefaulted() const;

      double directCertainty() const;

      bool isDirectCertaintyDefaulted() const;

      double directJitter() const;

      bool isDirectJitterDefaulted() const;

      double directPretest() const;

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

      bool isLimitWeightVMXDefaulted() const;

      double limitWeightDMX() const;

      bool isLimitWeightDMXDefaulted() const;

      int klemsSamplingDensity() const;

      bool isKlemsSamplingDensityDefaulted() const;

      std::string skyDiscretizationResolution() const;

      bool isCoarseSettings();

      bool isFineSettings();

      //@}
      /** @name Setters */
      //@{

      bool setAccumulatedRaysperRecord(int accumulatedRaysperRecord);

      void resetAccumulatedRaysperRecord();

      bool setDirectThreshold(double directThreshold);

      void resetDirectThreshold();

      bool setDirectCertainty(double directCertainty);

      void resetDirectCertainty();

      bool setDirectJitter(double directJitter);

      void resetDirectJitter();

      bool setDirectPretest(double directPretest);

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

      void resetLimitWeightVMX();

      bool setLimitWeightDMX(double limitWeightDMX);

      void resetLimitWeightDMX();

      bool setKlemsSamplingDensity(int klemsSamplingDensity);

      void resetKlemsSamplingDensity();

      bool setSkyDiscretizationResolution(const std::string& skyDiscretizationResolution);

      void resetSkyDiscretizationResolution();

      void applyCoarseSettings();

      void applyFineSettings();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RadianceParameters");

      std::vector<std::string> skyDiscretizationResolutionValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RADIANCEPARAMETERS_IMPL_HPP
