/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACETYPE_IMPL_HPP
#define MODEL_SPACETYPE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace Json {
class Value;
}

namespace openstudio {
namespace model {

  // forward declarations
  class Schedule;
  class Space;
  class InternalMass;
  class People;
  class Lights;
  class Luminaire;
  class ElectricEquipment;
  class ElectricEquipmentITEAirCooled;
  class GasEquipment;
  class HotWaterEquipment;
  class SteamEquipment;
  class OtherEquipment;
  class SpaceInfiltrationDesignFlowRate;
  class SpaceInfiltrationEffectiveLeakageArea;
  class SpaceInfiltrationFlowCoefficient;
  class DefaultConstructionSet;
  class DefaultScheduleSet;
  class DefaultScheduleType;
  class RenderingColor;
  class DesignSpecificationOutdoorAir;
  class SpaceType;

  namespace detail {

    /** SpaceType_Impl is a ResourceObject_Impl that is the implementation class for SpaceType.*/
    class MODEL_API SpaceType_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SpaceType_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SpaceType_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SpaceType_Impl(const SpaceType_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceType_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      /// Returns the default construction set.
      boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

      /// Returns the default schedule set.
      boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

      /// Returns the rendering color.
      boost::optional<RenderingColor> renderingColor() const;

      /** This corresponds to the energy Standard that is used: eg: 90.1-2010, 90.1-2013 **/
      boost::optional<std::string> standardsTemplate() const;
      std::vector<std::string> suggestedStandardsTemplates() const;

      /** This corresponds to the Building Types in the template: eg: FullServiceRestaurant, Hospital **/
      boost::optional<std::string> standardsBuildingType() const;
      std::vector<std::string> suggestedStandardsBuildingTypes() const;

      /** This corresponds to the Space Types under the Building Type: eg: Kitchen, Corridor **/
      boost::optional<std::string> standardsSpaceType() const;
      std::vector<std::string> suggestedStandardsSpaceTypes() const;

      //@}
      /** @name Setters */
      //@{

      /// Sets the default construction set.
      bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

      /// Resets the default construction set.
      void resetDefaultConstructionSet();

      /// Sets the default schedule set.
      bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

      /// Resets the default schedule set.
      void resetDefaultScheduleSet();

      /// Sets the rendering color.
      bool setRenderingColor(const RenderingColor& renderingColor);

      /// Resets the rendering color.
      void resetRenderingColor();

      bool setStandardsTemplate(const std::string& standardsTemplate);
      void resetStandardsTemplate();

      bool setStandardsBuildingType(const std::string& standardsBuildingType);
      void resetStandardsBuildingType();

      bool setStandardsSpaceType(const std::string& standardsSpaceType);
      void resetStandardsSpaceType();

      //@}
      /** @name Other */
      //@{

      /// Get all spaces of this space type, includes spaces that inherit this space type
      std::vector<Space> spaces() const;

      /// Returns the default schedule set for the specified type if available by searching (in order):
      /// This space types's default schedule set
      /// The building's default schedule set
      /// The building's space type's default schedule set
      boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

      /// Returns all InternalMass in this space type.
      std::vector<InternalMass> internalMass() const;

      /// Returns all People in this space type.
      std::vector<People> people() const;

      /// Returns all Lights in this space type.
      std::vector<Lights> lights() const;

      /// Returns all \link Luminaire Luminaires \endlink in this space type.
      std::vector<Luminaire> luminaires() const;

      /// Returns all ElectricEquipment in this space type.
      std::vector<ElectricEquipment> electricEquipment() const;

      /// Returns all ElectricEquipmentITEAirCooled in this space type.
      std::vector<ElectricEquipmentITEAirCooled> electricEquipmentITEAirCooled() const;

      /// Returns all GasEquipment in this space type.
      std::vector<GasEquipment> gasEquipment() const;

      /// Returns all HotWaterEquipment in this space type.
      std::vector<HotWaterEquipment> hotWaterEquipment() const;

      /// Returns all SteamEquipment in this space type.
      std::vector<SteamEquipment> steamEquipment() const;

      /// Returns all OtherEquipment in this space type.
      std::vector<OtherEquipment> otherEquipment() const;

      /// Returns all SpaceInfiltrationDesignFlowRate objects in this space type.
      std::vector<SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates() const;

      /// Returns all SpaceInfiltrationEffectiveLeakageArea objects in this space type.
      std::vector<SpaceInfiltrationEffectiveLeakageArea> spaceInfiltrationEffectiveLeakageAreas() const;

      /// Returns all SpaceInfiltrationFlowCoefficient objects in this space type.
      std::vector<SpaceInfiltrationFlowCoefficient> spaceInfiltrationFlowCoefficients() const;

      /// Returns DesignSpecificationOutdoorAir for this space type.
      boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir() const;

      /// Returns true if DesignSpecificationOutdoorAir is not directly specified for this space type.
      bool isDesignSpecificationOutdoorAirDefaulted() const;

      /// Sets DesignSpecificationOutdoorAir for this space type.
      bool setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

      /// Resets DesignSpecificationOutdoorAir for this space type.
      void resetDesignSpecificationOutdoorAir();

      /** Set all schedules for child space loads to their default value if there is one. */
      void hardApplySpaceLoadSchedules();

      /** Returns the total people per space floor area in this space type, if it can be calculated
     *  directly from the underlying people() data (without knowing floorArea). */
      boost::optional<double> peoplePerFloorArea() const;

      /** Sets the peoplePerFloorArea, using people()[0], if it exists, as a template for the
     *  remaining People and PeopleDefinition parameters. All other people() in this SpaceType will
     *  be removed. */
      bool setPeoplePerFloorArea(boost::optional<double> peoplePerFloorArea);

      /** Sets the peoplePerFloorArea, using templatePeople as a template for the remaining
     *  People and PeopleDefinition parameters. All people() in this SpaceType (except for
     *  templatePeople, if applicable), will be removed. */
      bool setPeoplePerFloorArea(double peoplePerFloorArea, const boost::optional<People>& templatePeople);

      /** Returns the floor area per person for this space type, if it can be calculated directly
     *  from the underlying people() data (without knowing floorArea). */
      boost::optional<double> spaceFloorAreaPerPerson() const;

      /** Sets the spaceFloorAreaPerPerson, using people()[0], if it exists, as a template for the
     *  remaining People and PeopleDefinition parameters. All other people() in this SpaceType will
     *  be removed. */
      bool setSpaceFloorAreaPerPerson(boost::optional<double> spaceFloorAreaPerPerson);

      /** Sets the spaceFloorAreaPerPerson, using templatePeople as a template for the remaining
     *  People and PeopleDefinition parameters. All people() in this SpaceType (except for
     *  templatePeople, if applicable), will be removed. */
      bool setSpaceFloorAreaPerPerson(double spaceFloorAreaPerPerson, const boost::optional<People>& templatePeople);

      double getNumberOfPeople(double floorArea) const;

      double getPeoplePerFloorArea(double floorArea) const;

      double getFloorAreaPerPerson(double floorArea) const;

      /** Returns the total lighting power per space floor area, if possible. Only works if there are
     *  no luminaires in the space type, and if all lights are defined on a per space floor area
     *  basis. */
      boost::optional<double> lightingPowerPerFloorArea() const;

      /** Sets the lightingPowerPerFloorArea, using lights()[0], if it exists, as a template for
     *  the remaining Lights and LightsDefinition parameters. All other lights() and luminaires()
     *  in this SpaceType will be removed. */
      bool setLightingPowerPerFloorArea(boost::optional<double> lightingPowerPerFloorArea);

      /** Sets the lightingPowerPerFloorArea, using templateLights as a template for the remaining
     *  Lights and LightsDefinition parameters. All lights() and luminaires() in this SpaceType
     *  (except for templateLights, if applicable), will be removed. */
      bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, const boost::optional<Lights>& templateLights);

      /** Returns the total lighting power per person, if possible. Only works if there are no
     *  luminaires in the space type, and if all lights are defined on a per person basis. */
      boost::optional<double> lightingPowerPerPerson() const;

      /** Sets the lightingPowerPerPerson, using lights()[0], if it exists, as a template for
     *  the remaining Lights and LightsDefinition parameters. All other lights() and luminaires()
     *  in this SpaceType will be removed. */
      bool setLightingPowerPerPerson(boost::optional<double> lightingPowerPerPerson);

      /** Sets the lightingPowerPerPerson, using templateLights as a template for the remaining
     *  Lights and LightsDefinition parameters. All lights() and luminaires() in this SpaceType
     *  (except for templateLights, if applicable), will be removed. */
      bool setLightingPowerPerPerson(double lightingPowerPerPerson, const boost::optional<Lights>& templateLights);

      /** Returns total lighting power for this space type, including lights and luminaires, assuming
     *  floorArea (m^2) and numPeople. */
      double getLightingPower(double floorArea, double numPeople) const;

      /** Returns the total lighting watts per space floor area for this space type, including lights
     *  and luminaires, assuming floorArea (m^2) and numPeople. */
      double getLightingPowerPerFloorArea(double floorArea, double numPeople) const;

      /** Returns the total lighting watts per person for this space type, including lights and
     *  luminaires, assuming floorArea (m^2) and numPeople. */
      double getLightingPowerPerPerson(double floorArea, double numPeople) const;

      /** Returns the total electric equipment power per space floor area, if it can be calculated
     *  directly from the underlying electricEquipment() data (without knowing floorArea and
     *  numPeople). */
      boost::optional<double> electricEquipmentPowerPerFloorArea() const;

      /** Sets the electricEquipmentPowerPerFloorArea, using electricEquipment()[0], if it existss,
     *  as a template for the reamaining ElectricEquipment and ElectricEquipmentDefinition parameters.
     *  All other electricEquipment() in this SpaceType will be removed. */
      bool setElectricEquipmentPowerPerFloorArea(boost::optional<double> electricEquipmentPowerPerFloorArea);

      /** Sets the electricEquipmentPowerPerFloorArea, using templateElectricEquipment as a template
     *  for the remaining ElectricEquipment and ElectricEquipmentDefinition parameters. All other
     *  electricEquipment() in this SpaceType will be removed. */
      bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea,
                                                 const boost::optional<ElectricEquipment>& templateElectricEquipment);

      /** Returns the total electric equipment power per person, if it can be calculated
     *  directly from the underlying electricEquipment() data (without knowing floorArea and
     *  numPeople). */
      boost::optional<double> electricEquipmentPowerPerPerson() const;

      /** Sets the electricEquipmentPowerPerPerson, using electricEquipment()[0], if it existss,
     *  as a template for the reamaining ElectricEquipment and ElectricEquipmentDefinition parameters.
     *  All other electricEquipment() in this SpaceType will be removed. */
      bool setElectricEquipmentPowerPerPerson(boost::optional<double> electricEquipmentPowerPerPerson);

      /** Sets the electricEquipmentPowerPerPerson, using templateElectricEquipment as a template
     *  for the remaining ElectricEquipment and ElectricEquipmentDefinition parameters. All other
     *  electricEquipment() in this SpaceType will be removed. */
      bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson,
                                              const boost::optional<ElectricEquipment>& templateElectricEquipment);

      double getElectricEquipmentDesignLevel(double floorArea, double numPeople) const;

      double getElectricEquipmentPowerPerFloorArea(double floorArea, double numPeople) const;

      double getElectricEquipmentPowerPerPerson(double floorArea, double numPeople) const;

      /** Returns the total IT equipment power per space floor area, if it can be calculated
    *  directly from the underlying electricEquipmentITEAirCooled() data (without knowing floorArea and
    *  numPeople). */
      boost::optional<double> electricEquipmentITEAirCooledPowerPerFloorArea() const;

      /** Returns the total gas equipment power per space floor area, if it can be calculated
     *  directly from the underlying gasEquipment() data (without knowing floorArea and
     *  numPeople). */
      boost::optional<double> gasEquipmentPowerPerFloorArea() const;

      /** Sets the gasEquipmentPowerPerFloorArea, using gasEquipment()[0], if it existss,
     *  as a template for the reamaining GasEquipment and GasEquipmentDefinition parameters.
     *  All other gasEquipment() in this SpaceType will be removed. */
      bool setGasEquipmentPowerPerFloorArea(boost::optional<double> gasEquipmentPowerPerFloorArea);

      /** Sets the gasEquipmentPowerPerFloorArea, using templateGasEquipment as a template
     *  for the remaining GasEquipment and GasEquipmentDefinition parameters. All other
     *  gasEquipment() in this SpaceType will be removed. */
      bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea, const boost::optional<GasEquipment>& templateGasEquipment);

      /** Returns the total gas equipment power per person, if it can be calculated directly from
     *  the underlying gasEquipment() data (without knowing floorArea and numPeople). */
      boost::optional<double> gasEquipmentPowerPerPerson() const;

      /** Sets the gasEquipmentPowerPerPerson, using gasEquipment()[0], if it existss,
     *  as a template for the reamaining GasEquipment and GasEquipmentDefinition parameters.
     *  All other gasEquipment() in this SpaceType will be removed. */
      bool setGasEquipmentPowerPerPerson(boost::optional<double> gasEquipmentPowerPerPerson);

      /** Sets the gasEquipmentPowerPerPerson, using templateGasEquipment as a template
     *  for the remaining GasEquipment and GasEquipmentDefinition parameters. All other
     *  gasEquipment() in this SpaceType will be removed. */
      bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson, const boost::optional<GasEquipment>& templateGasEquipment);

      double getGasEquipmentDesignLevel(double floorArea, double numPeople) const;

      double getGasEquipmentPowerPerFloorArea(double floorArea, double numPeople) const;

      double getGasEquipmentPowerPerPerson(double floorArea, double numPeople) const;

      double floorArea() const;

      boost::optional<std::string> setNameProtected(const std::string& newName);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SpaceType");

      boost::optional<ModelObject> defaultConstructionSetAsModelObject() const;
      boost::optional<ModelObject> defaultScheduleSetAsModelObject() const;
      boost::optional<ModelObject> renderingColorAsModelObject() const;
      std::vector<ModelObject> internalMassAsModelObjects() const;
      std::vector<ModelObject> peopleAsModelObjects() const;
      std::vector<ModelObject> lightsAsModelObjects() const;
      std::vector<ModelObject> luminairesAsModelObjects() const;
      std::vector<ModelObject> electricEquipmentAsModelObjects() const;
      std::vector<ModelObject> electricEquipmentITEAirCooledAsModelObjects() const;
      std::vector<ModelObject> gasEquipmentAsModelObjects() const;
      std::vector<ModelObject> hotWaterEquipmentAsModelObjects() const;
      std::vector<ModelObject> steamEquipmentAsModelObjects() const;
      std::vector<ModelObject> otherEquipmentAsModelObjects() const;
      std::vector<ModelObject> spaceInfiltrationDesignFlowRatesAsModelObjects() const;
      std::vector<ModelObject> spaceInfiltrationEffectiveLeakageAreasAsModelObjects() const;
      std::vector<ModelObject> spaceInfiltrationFlowCoefficientsAsModelObjects() const;
      std::vector<ModelObject> spaceVentilationDesignFlowRatesAsModelObjects() const;

      bool setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setRenderingColorAsModelObject(const boost::optional<ModelObject>& modelObject);

      template <typename T, typename TDef>
      boost::optional<T> getMySpaceLoadInstance(const boost::optional<T>& templateSpaceLoadInstance);

      template <typename T>
      void removeAllButOneSpaceLoadInstance(std::vector<T>& instances, const T& instanceToKeep);

      static const Json::Value& getStandardsJSON();
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACETYPE_IMPL_HPP
