/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORPVWATTS_IMPL_HPP
#define MODEL_GENERATORPVWATTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;

  namespace detail {

    /** GeneratorPVWatts_Impl is a Generator_Impl that is the implementation class for GeneratorPVWatts.*/
    class MODEL_API GeneratorPVWatts_Impl : public Generator_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorPVWatts_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorPVWatts_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorPVWatts_Impl(const GeneratorPVWatts_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorPVWatts_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::string generatorObjectType() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      //@}
      /** @name Getters */
      //@{

      std::string pvWattsVersion() const;

      double dcSystemCapacity() const;

      std::string moduleType() const;

      bool isModuleTypeDefaulted() const;

      std::string arrayType() const;

      bool isArrayTypeDefaulted() const;

      double systemLosses() const;

      bool isSystemLossesDefaulted() const;

      double tiltAngle() const;

      bool isTiltAngleDefaulted() const;

      double azimuthAngle() const;

      bool isAzimuthAngleDefaulted() const;

      boost::optional<PlanarSurface> surface() const;

      double groundCoverageRatio() const;

      bool isGroundCoverageRatioDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setDCSystemCapacity(double dcSystemCapacity);

      bool setModuleType(const std::string& moduleType);

      void resetModuleType();

      bool setArrayType(const std::string& arrayType);

      void resetArrayType();

      bool setSystemLosses(double systemLosses);

      void resetSystemLosses();

      bool setTiltAngle(double tiltAngle);

      void resetTiltAngle();

      bool setAzimuthAngle(double azimuthAngle);

      void resetAzimuthAngle();

      bool setSurface(const PlanarSurface& surface);

      void resetSurface();

      bool setGroundCoverageRatio(double groundCoverageRatio);

      void resetGroundCoverageRatio();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorPVWatts");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORPVWATTS_IMPL_HPP
