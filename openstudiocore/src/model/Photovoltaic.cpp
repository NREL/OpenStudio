#include "Photovoltaic.hpp"
#include "Photovoltaic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Exterior_PV_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"


namespace openstudio {
	namespace model {

		namespace detail {
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, surfaceArea, SurfaceArea, Photovoltaic, 0, OS_Exterior_PV, SurfaceArea)
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, factionActive, FactionActive, Photovoltaic, 0, OS_Exterior_PV, FactionActive)
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, inverterEfficiency, InverterEfficiency, Photovoltaic, 0, OS_Exterior_PV, InverterEfficiency)
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, azimuthAngle, AzimuthAngle, Photovoltaic, 0, OS_Exterior_PV, AzimuthAngle)
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, inclinationAngle, InclinationAngle, Photovoltaic, 0, OS_Exterior_PV, InclinationAngle)
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, cellEfficiency, CellEfficiency, Photovoltaic, 0, OS_Exterior_PV, CellEfficiency)
			ATTRIBUTE_IMPLEMENTATION(0, 1, 0, gtEfficiency, GTEfficiency, Photovoltaic, 0, OS_Exterior_PV, GTEfficiency)


			Photovoltaic_Impl::Photovoltaic_Impl(const IdfObject& idfObject,
				Model_Impl* model,
				bool keepHandle)
				: ResourceObject_Impl(idfObject, model, keepHandle)
			{
				OS_ASSERT(idfObject.iddObject().type() == Photovoltaic::iddObjectType());
			}

			Photovoltaic_Impl::Photovoltaic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
				Model_Impl* model,
				bool keepHandle)
				: ResourceObject_Impl(other, model, keepHandle)
			{
				OS_ASSERT(other.iddObject().type() == Photovoltaic::iddObjectType());
			}

			Photovoltaic_Impl::Photovoltaic_Impl(const Photovoltaic_Impl& other,
				Model_Impl* model,
				bool keepHandle)
				: ResourceObject_Impl(other, model, keepHandle)
			{}

			const std::vector<std::string>& Photovoltaic_Impl::outputVariableNames() const
			{
				static std::vector<std::string> result;
				if (result.empty()){
				}
				return result;
			}

			IddObjectType Photovoltaic_Impl::iddObjectType() const {
				return Photovoltaic::iddObjectType();
			}

			std::vector<std::string> Photovoltaic_Impl::PVTypeValues() const {
				return Photovoltaic::PVTypeValues();
			}

			std::string Photovoltaic_Impl::PVType() const {
				boost::optional<std::string> value = getString(OS_Exterior_PVFields::Type, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setSurfaceArea(double serfaceArea){
				bool result = setDouble(OS_Exterior_PVFields::SurfaceArea, serfaceArea);
				return result;
			}
			
			bool Photovoltaic_Impl::setPVType(std::string PVType) {
				bool result = setString(OS_Exterior_PVFields::Type, PVType);
				return result;
			}

			double Photovoltaic_Impl::surfaceArea() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::SurfaceArea, true);
				OS_ASSERT(value);
				return value.get();
			}
			
			double Photovoltaic_Impl::factionActive() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::FactionActive, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setFactionActive(double factionActive){
				bool result = setDouble(OS_Exterior_PVFields::FactionActive, factionActive);
				return result;
			}

			double Photovoltaic_Impl::inverterEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::InverterEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setInverterEfficiency(double inverterEfficiency){
				bool result = setDouble(OS_Exterior_PVFields::InverterEfficiency, inverterEfficiency);
				return result;
			}

			double Photovoltaic_Impl::azimuthAngle() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::AzimuthAngle, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setAzimuthAngle(double azmuthAngle){
				bool result = setDouble(OS_Exterior_PVFields::AzimuthAngle, azmuthAngle);
				return result;
			}

			double Photovoltaic_Impl::inclinationAngle() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::InclinationAngle, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setInclinationAngle(double inclinationAngle){
				bool result = setDouble(OS_Exterior_PVFields::AzimuthAngle, inclinationAngle);
				return result;
			}

			double Photovoltaic_Impl::cellEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::CellEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setCellEfficiency(double cellEfficiency){
				bool result = setDouble(OS_Exterior_PVFields::CellEfficiency, cellEfficiency);
				return result;
			}

			double Photovoltaic_Impl::gtEfficiency() const {
				boost::optional<double> value = getDouble(OS_Exterior_PVFields::GTEfficiency, true);
				OS_ASSERT(value);
				return value.get();
			}

			bool Photovoltaic_Impl::setGTEfficiency(double gtEfficiency){
				bool result = setDouble(OS_Exterior_PVFields::GTEfficiency, gtEfficiency);
				return result;
			}


			double Photovoltaic_Impl::PVTypeToValue(std::string type){
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

			double Photovoltaic_Impl::PVTypeToValue(){
				return Photovoltaic_Impl::PVTypeToValue(Photovoltaic_Impl::PVType());
			}


			double Photovoltaic_Impl::calculatePV(){
				// power = Asurface * Factive * Gt * CellEfficiency * inverterEfficiency * time
				double result = Photovoltaic_Impl::surfaceArea() * Photovoltaic_Impl::PVTypeToValue() * (Photovoltaic_Impl::factionActive() / 100.0) * Photovoltaic_Impl::gtEfficiency() * (Photovoltaic_Impl::inverterEfficiency() / 100.0) * (8 * 365);
				return result;
			}

			double Photovoltaic_Impl::calculatePV(double hours, double days){
				// power = Asurface * Factive * Gt * CellEfficiency * inverterEfficiency * time
				double result = Photovoltaic_Impl::surfaceArea() * Photovoltaic_Impl::PVTypeToValue() * (Photovoltaic_Impl::factionActive() / 100.0) * Photovoltaic_Impl::gtEfficiency() * (Photovoltaic_Impl::inverterEfficiency() / 100.0) * (days * hours);
				return result;
			}

		} // detail

		Photovoltaic::Photovoltaic(const Model& model)
			: ResourceObject(Photovoltaic::iddObjectType(), model)
		{
			OS_ASSERT(getImpl<detail::Photovoltaic_Impl>());

			bool ok = setSurfaceArea(0.0);
			OS_ASSERT(ok);

			ok = setFactionActive(0.9);
			OS_ASSERT(ok);

			ok = setInverterEfficiency(98.5);
			OS_ASSERT(ok);

			ok = setGTEfficiency(1709100.0);
			OS_ASSERT(ok);

			ok = setCellEfficiency(0.0);
			OS_ASSERT(ok);

			ok = setAzimuthAngle(0.0);
			OS_ASSERT(ok);

			ok = setInclinationAngle(0.0);
			OS_ASSERT(ok);

			ok = setPVType("Monocrystalline");
			OS_ASSERT(ok);

		}

		IddObjectType Photovoltaic::iddObjectType() {
			IddObjectType result(IddObjectType::OS_Exterior_PV);
			return result;
		}

		std::vector<std::string> Photovoltaic::PVTypeValues() {
			return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
				OS_Exterior_PVFields::Type);
		}

    std::string Photovoltaic::PVType() const {
      return getImpl<detail::Photovoltaic_Impl>()->PVType();
    }

    bool Photovoltaic::setPVType(std::string PVType) {
			return getImpl<detail::Photovoltaic_Impl>()->setPVType(PVType);
		}

		double Photovoltaic::surfaceArea(){
			return getImpl<detail::Photovoltaic_Impl>()->surfaceArea();
		}

		bool Photovoltaic::setSurfaceArea(double surfaceArea){
			return getImpl<detail::Photovoltaic_Impl>()->setSurfaceArea(surfaceArea);
		}

		double Photovoltaic::factionActive(){
			return getImpl<detail::Photovoltaic_Impl>()->factionActive();
		}

		bool Photovoltaic::setFactionActive(double factionActive){
			return getImpl<detail::Photovoltaic_Impl>()->setFactionActive(factionActive);
		}

		double Photovoltaic::inverterEfficiency(){
			return getImpl<detail::Photovoltaic_Impl>()->inverterEfficiency();
		}

		bool Photovoltaic::setInverterEfficiency(double inverterEfficiency){
			return getImpl<detail::Photovoltaic_Impl>()->setInverterEfficiency(inverterEfficiency);
		}

		double Photovoltaic::azimuthAngle(){
			return getImpl<detail::Photovoltaic_Impl>()->azimuthAngle();
		}

		bool Photovoltaic::setAzimuthAngle(double azimuthAngle){
			return getImpl<detail::Photovoltaic_Impl>()->setAzimuthAngle(azimuthAngle);
		}

		double Photovoltaic::inclinationAngle(){
			return getImpl<detail::Photovoltaic_Impl>()->inclinationAngle();
		}

		bool Photovoltaic::setInclinationAngle(double inclinationAngle){
			return getImpl<detail::Photovoltaic_Impl>()->setInclinationAngle(inclinationAngle);
		}

		double Photovoltaic::cellEfficiency(){
			return getImpl<detail::Photovoltaic_Impl>()->cellEfficiency();
		}

		bool Photovoltaic::setCellEfficiency(double cellEfficiency){
			return getImpl<detail::Photovoltaic_Impl>()->setCellEfficiency(cellEfficiency);
		}

		double Photovoltaic::gtEfficiency(){
			return getImpl<detail::Photovoltaic_Impl>()->gtEfficiency();
		}

		bool Photovoltaic::setGTEfficiency(double gtEfficiency){
			return getImpl<detail::Photovoltaic_Impl>()->setGTEfficiency(gtEfficiency);
		}

		double Photovoltaic::calculatePV(double hours, double days){
			return getImpl<detail::Photovoltaic_Impl>()->calculatePV(hours, days);
		}

		double Photovoltaic::calculatePV(){
			return getImpl<detail::Photovoltaic_Impl>()->calculatePV();
		}

		double Photovoltaic::PVTypeToValue(std::string type){
			return getImpl<detail::Photovoltaic_Impl>()->PVTypeToValue(type);
		}

		double Photovoltaic::PVTypeToValue(){
			return getImpl<detail::Photovoltaic_Impl>()->PVTypeToValue();
		}

/// @cond
Photovoltaic::Photovoltaic(std::shared_ptr<detail::Photovoltaic_Impl> impl)
	: ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

