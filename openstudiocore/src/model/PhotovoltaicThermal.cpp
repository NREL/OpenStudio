#include "PhotovoltaicThermal.hpp"
#include "PhotovoltaicThermal_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Exterior_PV_Thermal_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"


namespace openstudio {
	namespace model {

		namespace detail {
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, surfaceArea, SurfaceArea, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, SurfaceArea)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, factionActive, FactionActive, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, FactionActive)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, azimuthAngle, AzimuthAngle, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, AzimuthAngle)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, inclinationAngle, InclinationAngle, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, InclinationAngle)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, cellEfficiency, CellEfficiency, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, CellEfficiency)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, gtEfficiency, GTEfficiency, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, GTEfficiency)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, boilerEfficiency, BoilerEfficiency, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, BoilerEfficiency)
				ATTRIBUTE_IMPLEMENTATION(0, 1, 0, collectorEfficiency, CollectorEfficiency, PhotovoltaicThermal, 0, OS_Exterior_PV_Thermal, CollectorEfficiency)
	
			PhotovoltaicThermal_Impl::PhotovoltaicThermal_Impl(const IdfObject& idfObject,
				Model_Impl* model,
				bool keepHandle)
				: ResourceObject_Impl(idfObject, model, keepHandle)
			{
				OS_ASSERT(idfObject.iddObject().type() == PhotovoltaicThermal::iddObjectType());
			}

			PhotovoltaicThermal_Impl::PhotovoltaicThermal_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
				Model_Impl* model,
				bool keepHandle)
				: ResourceObject_Impl(other, model, keepHandle)
			{
				OS_ASSERT(other.iddObject().type() == PhotovoltaicThermal::iddObjectType());
			}

			PhotovoltaicThermal_Impl::PhotovoltaicThermal_Impl(const PhotovoltaicThermal_Impl& other,
				Model_Impl* model,
				bool keepHandle)
				: ResourceObject_Impl(other, model, keepHandle)
			{}

			const std::vector<std::string>& PhotovoltaicThermal_Impl::outputVariableNames() const
			{
				static std::vector<std::string> result;
				if (result.empty()){
				}
				return result;
			}

			IddObjectType PhotovoltaicThermal_Impl::iddObjectType() const {
				return PhotovoltaicThermal::iddObjectType();
			}

			std::vector<std::string> PhotovoltaicThermal_Impl::PVTypeValues() const {
				return PhotovoltaicThermal::PVTypeValues();
			}

			std::string PhotovoltaicThermal_Impl::PVType() const {
				boost::optional<std::string> value = getString(OS_Exterior_PV_ThermalFields::Type, true);
				OS_ASSERT(value);
				return value.get();
			}


			bool PhotovoltaicThermal_Impl::setSurfaceArea(double serfaceArea){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::SurfaceArea, serfaceArea);
				return result;
			}
			
			bool PhotovoltaicThermal_Impl::setPVType(std::string PVType) {
				bool result = setString(OS_Exterior_PV_ThermalFields::Type, PVType);
				return result;
			}

			double PhotovoltaicThermal_Impl::surfaceArea() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::SurfaceArea, true);
				OS_ASSERT(value);
				return value.get();
			}
			
			double PhotovoltaicThermal_Impl::factionActive() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::FactionActive, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool PhotovoltaicThermal_Impl::setFactionActive(double factionActive){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::FactionActive, factionActive);
				return result;
			}

			double PhotovoltaicThermal_Impl::azimuthAngle() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::AzimuthAngle, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool PhotovoltaicThermal_Impl::setAzimuthAngle(double azmuthAngle){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::AzimuthAngle, azmuthAngle);
				return result;
			}

			double PhotovoltaicThermal_Impl::inclinationAngle() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::InclinationAngle, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool PhotovoltaicThermal_Impl::setInclinationAngle(double inclinationAngle){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::AzimuthAngle, inclinationAngle);
				return result;
			}

			double PhotovoltaicThermal_Impl::cellEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::CellEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool PhotovoltaicThermal_Impl::setCellEfficiency(double cellEfficiency){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::CellEfficiency, cellEfficiency);
				return result;
			}

			double PhotovoltaicThermal_Impl::gtEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::GTEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool PhotovoltaicThermal_Impl::setGTEfficiency(double gtEfficiency){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::GTEfficiency, gtEfficiency);
				return result;
			}


			double PhotovoltaicThermal_Impl::PVTypeToValue(std::string type){
				if (type.compare("Monocrystalline") == 0){
					return 15.0;
				}
				else if (type.compare("Polycrystalline") == 0){
					return 12.0;
				}
				else if (type.compare("Amorphous") == 0) {
					return 9.0;
				}
				else{
					return 15.0; //default as Monocrystalline
				}
			}

		
			double PhotovoltaicThermal_Impl::PVTypeToValue(){
				return PhotovoltaicThermal_Impl::PVTypeToValue(PhotovoltaicThermal_Impl::PVType());
			}

			double PhotovoltaicThermal_Impl::collectorEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::CollectorEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}
			
			bool PhotovoltaicThermal_Impl::setCollectorEfficiency(double collectorEfficiency){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::CollectorEfficiency, collectorEfficiency);
				return result;
			}
			
			double PhotovoltaicThermal_Impl::boilerEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PV_ThermalFields::BoilerEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}
			
			bool PhotovoltaicThermal_Impl::setBoilerEfficiency(double boilerEfficiency){
				bool result = setDouble(OS_Exterior_PV_ThermalFields::BoilerEfficiency, boilerEfficiency);
				return result;
			}

			double PhotovoltaicThermal_Impl::calculatePV(){
				// power = Asurface * Factive * Gt * CellEfficiency * collectorEfficiency * time
				// power_saving = power/boilerEfficiency
				double result = PhotovoltaicThermal_Impl::surfaceArea() * (PhotovoltaicThermal_Impl::factionActive() / 100.0) * PhotovoltaicThermal_Impl::gtEfficiency() * (PhotovoltaicThermal_Impl::collectorEfficiency()/100) * (8 * 365);
				result = result / (PhotovoltaicThermal_Impl::boilerEfficiency() / 100);
				return result;
			}

			double PhotovoltaicThermal_Impl::calculatePV(double hours, double days){
				// power = Asurface * Factive * Gt * CellEfficiency * collectorEfficiency * time
				// power_saving = power/boilerEfficiency
				double result = PhotovoltaicThermal_Impl::surfaceArea() * (PhotovoltaicThermal_Impl::factionActive() / 100.0) * PhotovoltaicThermal_Impl::gtEfficiency() * (PhotovoltaicThermal_Impl::collectorEfficiency() / 100) * (days * hours);
				result = result / (PhotovoltaicThermal_Impl::boilerEfficiency() / 100);
				return result;
			}

		} // detail

		PhotovoltaicThermal::PhotovoltaicThermal(const Model& model)
			: ResourceObject(PhotovoltaicThermal::iddObjectType(), model)
		{
			OS_ASSERT(getImpl<detail::PhotovoltaicThermal_Impl>());

			bool ok = setSurfaceArea(0.0);
			OS_ASSERT(ok);

			ok = setFactionActive(0.8);
			OS_ASSERT(ok);

			ok = setGTEfficiency(1709100.0);
			OS_ASSERT(ok);

			ok = setCellEfficiency(0.0);
			OS_ASSERT(ok);

			ok = setAzimuthAngle(0.0);
			OS_ASSERT(ok);

			ok = setInclinationAngle(0.0);
			OS_ASSERT(ok);

			ok = setBoilerEfficiency(0.8);
			OS_ASSERT(ok);

			ok = setCollectorEfficiency(0.7);
			OS_ASSERT(ok);
			ok = setPVType("Monocrystalline");
			OS_ASSERT(ok);

		}

		IddObjectType PhotovoltaicThermal::iddObjectType() {
			IddObjectType result(IddObjectType::OS_Exterior_PV_Thermal);
			return result;
		}

		std::vector<std::string> PhotovoltaicThermal::PVTypeValues() {
			return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Exterior_PV_ThermalFields::Type);
		}

		bool PhotovoltaicThermal::setPVType(std::string PVType) {
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setPVType(PVType);
		}

		double PhotovoltaicThermal::surfaceArea(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->surfaceArea();
		}

		bool PhotovoltaicThermal::setSurfaceArea(double surfaceArea){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setSurfaceArea(surfaceArea);
		}

		double PhotovoltaicThermal::factionActive(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->factionActive();
		}

		bool PhotovoltaicThermal::setFactionActive(double factionActive){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setFactionActive(factionActive);
		}

		double PhotovoltaicThermal::azimuthAngle(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->azimuthAngle();
		}

		bool PhotovoltaicThermal::setAzimuthAngle(double azimuthAngle){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setAzimuthAngle(azimuthAngle);
		}

		double PhotovoltaicThermal::inclinationAngle(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->inclinationAngle();
		}

		bool PhotovoltaicThermal::setInclinationAngle(double inclinationAngle){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setInclinationAngle(inclinationAngle);
		}

		double PhotovoltaicThermal::cellEfficiency(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->cellEfficiency();
		}

		bool PhotovoltaicThermal::setCellEfficiency(double cellEfficiency){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setCellEfficiency(cellEfficiency);
		}

		double PhotovoltaicThermal::gtEfficiency(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->gtEfficiency();
		}

		bool PhotovoltaicThermal::setGTEfficiency(double gtEfficiency){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setGTEfficiency(gtEfficiency);
		}

		double PhotovoltaicThermal::collectorEfficiency(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->collectorEfficiency();
		}
		
		bool PhotovoltaicThermal::setCollectorEfficiency(double collectorEfficiency){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setCollectorEfficiency(collectorEfficiency);
		}
		
		double PhotovoltaicThermal::boilerEfficiency(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->boilerEfficiency();
		}
		
		bool PhotovoltaicThermal::setBoilerEfficiency(double boilerEfficiency){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->setBoilerEfficiency(boilerEfficiency);
		}
		
		double PhotovoltaicThermal::calculatePV(double hours, double days){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->calculatePV(hours, days);
		}

		double PhotovoltaicThermal::calculatePV(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->calculatePV();
		}

		double PhotovoltaicThermal::PVTypeToValue(std::string type){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->PVTypeToValue(type);
		}

		double PhotovoltaicThermal::PVTypeToValue(){
			return getImpl<detail::PhotovoltaicThermal_Impl>()->PVTypeToValue();
		}

/// @cond
PhotovoltaicThermal::PhotovoltaicThermal(std::shared_ptr<detail::PhotovoltaicThermal_Impl> impl)
	: ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

