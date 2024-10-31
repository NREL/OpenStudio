/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP
#define MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;
  class CurveBicubic;

  namespace detail {

    /** RefrigerationCompressor_Impl is a ParentObject_Impl that is the implementation class for RefrigerationCompressor.*/
    class MODEL_API RefrigerationCompressor_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCompressor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCompressor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCompressor_Impl(const RefrigerationCompressor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCompressor_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      CurveBicubic refrigerationCompressorPowerCurve() const;

      CurveBicubic refrigerationCompressorCapacityCurve() const;

      boost::optional<double> ratedSuperheat() const;

      boost::optional<double> ratedReturnGasTemperature() const;

      boost::optional<double> ratedLiquidTemperature() const;

      boost::optional<double> ratedSubcooling() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      std::string modeofOperation() const;

      // bool isModeofOperationDefaulted() const;

      boost::optional<CurveBicubic> transcriticalCompressorPowerCurve() const;

      boost::optional<CurveBicubic> transcriticalCompressorCapacityCurve() const;

      boost::optional<RefrigerationSystem> system() const;

      //@}
      /** @name Setters */
      //@{

      bool setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic);

      bool setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic);

      bool setRatedSuperheat(boost::optional<double> ratedSuperheat);

      void resetRatedSuperheat();

      bool setRatedReturnGasTemperature(boost::optional<double> ratedReturnGasTemperature);

      void resetRatedReturnGasTemperature();

      bool setRatedLiquidTemperature(boost::optional<double> ratedLiquidTemperature);

      void resetRatedLiquidTemperature();

      bool setRatedSubcooling(boost::optional<double> ratedSubcooling);

      void resetRatedSubcooling();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      // bool setModeofOperation(const std::string& modeofOperation);

      // void resetModeofOperation();

      bool setTranscriticalCompressorPowerCurve(const boost::optional<CurveBicubic>& curveBicubic);

      void resetTranscriticalCompressorPowerCurve();

      bool setTranscriticalCompressorCapacityCurve(const boost::optional<CurveBicubic>& curveBicubic);

      void resetTranscriticalCompressorCapacityCurve();

      void removeFromSystem();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCompressor");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<CurveBicubic> optionalRefrigerationCompressorPowerCurve() const;
      boost::optional<CurveBicubic> optionalRefrigerationCompressorCapacityCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCOMPRESSOR_IMPL_HPP
