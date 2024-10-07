/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGDEVICETUBULAR_IMPL_HPP
#define MODEL_DAYLIGHTINGDEVICETUBULAR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class ConstructionBase;
  class ThermalZone;
  class TransitionZone;

  namespace detail {

    /** DaylightingDeviceTubular_Impl is a ModelObject_Impl that is the implementation class for DaylightingDeviceTubular.*/
    class MODEL_API DaylightingDeviceTubular_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      DaylightingDeviceTubular_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DaylightingDeviceTubular_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DaylightingDeviceTubular_Impl(const DaylightingDeviceTubular_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DaylightingDeviceTubular_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      SubSurface subSurfaceDome() const;

      SubSurface subSurfaceDiffuser() const;

      ConstructionBase construction() const;

      double diameter() const;

      double totalLength() const;

      double effectiveThermalResistance() const;

      std::vector<TransitionZone> transitionZones() const;

      unsigned int numberofTransitionZones() const;

      //@}
      /** @name Setters */
      //@{

      bool setConstruction(const ConstructionBase& construction);

      bool setDiameter(double diameter);

      bool setTotalLength(double totalLength);

      bool setEffectiveThermalResistance(double effectiveThermalResistance);

      bool addTransitionZone(const TransitionZone& transitionZone);

      bool addTransitionZone(const ThermalZone& zone, double length);

      bool removeTransitionZone(unsigned groupIndex);

      void removeAllTransitionZones();

      bool addTransitionZones(const std::vector<TransitionZone>& transitionZones);
      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DaylightingDeviceTubular");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICETUBULAR_IMPL_HPP
