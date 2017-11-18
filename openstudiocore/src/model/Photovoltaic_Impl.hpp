#ifndef MODEL_PHOTOVOLTAIC_IMPL_HPP
#define MODEL_PHOTOVOLTAIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {
	
// forward declaration
class Photovoltaic;

namespace detail {

	class MODEL_API Photovoltaic_Impl : public ResourceObject_Impl {
	
	  /* Q_OBJECT;
	  Q_PROPERTY(std::string PVType READ PVType WRITE setPVType);
	  Q_PROPERTY(std::vector<std::string> PVTypeValues READ PVTypeValues);

	  Q_PROPERTY(double surfaceArea READ surfaceArea WRITE setSurfaceArea);
	  Q_PROPERTY(openstudio::Quantity surfaceArea_SI READ surfaceArea_SI WRITE setSurfaceArea);
	  Q_PROPERTY(openstudio::Quantity surfaceArea_IP READ surfaceArea_IP WRITE setSurfaceArea);

	  Q_PROPERTY(double factionActive READ factionActive WRITE setFactionActive RESET resetFactionActive);
	  Q_PROPERTY(openstudio::Quantity factionActive_SI  READ factionActive_SI   WRITE setFactionActive);
	  Q_PROPERTY(openstudio::Quantity factionActive_IP  READ factionActive_IP   WRITE setFactionActive);
	  
	  Q_PROPERTY(double inverterEfficiency READ inverterEfficiency WRITE setInverterEfficiency RESET resetInverterEfficiency);
	  Q_PROPERTY(openstudio::Quantity inverterEfficiency_SI  READ inverterEfficiency_SI  WRITE setInverterEfficiency);
	  Q_PROPERTY(openstudio::Quantity inverterEfficiency_IP  READ inverterEfficiency_IP  WRITE setInverterEfficiency);

	  Q_PROPERTY(double azimuthAngle READ azimuthAngle WRITE setAzimuthAngle RESET resetAzimuthAngle);
	  Q_PROPERTY(openstudio::Quantity azimuthAngle_SI  READ azimuthAngle_SI  WRITE setAzimuthAngle);
	  Q_PROPERTY(openstudio::Quantity azimuthAngle_IP  READ azimuthAngle_IP  WRITE setAzimuthAngle);

	  Q_PROPERTY(double inclinationAngle READ inclinationAngle WRITE setInclinationAngle RESET resetInclinationAngle);
	  Q_PROPERTY(openstudio::Quantity inclinationAngle_SI  READ inclinationAngle_SI  WRITE setInclinationAngle);
	  Q_PROPERTY(openstudio::Quantity inclinationAngle_IP  READ inclinationAngle_IP  WRITE setInclinationAngle);

	  Q_PROPERTY(double cellEfficiency READ cellEfficiency WRITE setCellEfficiency RESET resetCellEfficiency);
	  Q_PROPERTY(openstudio::Quantity cellEfficiency_SI  READ cellEfficiency_SI	  WRITE setCellEfficiency);
	  Q_PROPERTY(openstudio::Quantity cellEfficiency_IP  READ cellEfficiency_IP   WRITE setCellEfficiency);

	  Q_PROPERTY(double gtEfficiency READ gtEfficiency WRITE setGTEfficiency RESET resetGTEfficiency);
	  Q_PROPERTY(openstudio::Quantity gtEfficiency_SI  READ gtEfficiency_SI	WRITE setGTEfficiency);
	  Q_PROPERTY(openstudio::Quantity gtEfficiency_IP  READ gtEfficiency_IP   WRITE setGTEfficiency); */
	  
		  
   public:
    /** @name Constructors and Destructors */
    //@{

    Photovoltaic_Impl(const IdfObject& idfObject,
                         Model_Impl* model,
                         bool keepHandle);

    Photovoltaic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    Photovoltaic_Impl(const Photovoltaic_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    virtual ~Photovoltaic_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

	virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

	ATTRIBUTE_DEFINITION(0, 1, 0, surfaceArea, SurfaceArea);
	ATTRIBUTE_DEFINITION(0, 1, 0, factionActive, FactionActive);
	ATTRIBUTE_DEFINITION(0, 1, 0, inverterEfficiency, InverterEfficiency); 
	ATTRIBUTE_DEFINITION(0, 1, 0, azimuthAngle, AzimuthAngle); 
	ATTRIBUTE_DEFINITION(0, 1, 0, inclinationAngle, InclinationAngle);
	ATTRIBUTE_DEFINITION(0, 1, 0, cellEfficiency, CellEfficiency);
	ATTRIBUTE_DEFINITION(0, 1, 0, gtEfficiency, GTEfficiency);

	//@}
	/** @name Getters */
	//@{

	std::string PVType() const;
	double calculatePV(double hours, double days);
	double calculatePV();
	double PVTypeToValue(std::string type);
	double PVTypeToValue();
	//@}
	/** @name Setters */
	//@{

	bool setPVType(std::string PVType);
	

	//@}
	/** @name Other */
	//@{

	//@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.Photovoltaic");
	std::vector<std::string> PVTypeValues() const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAIC_IMPL_HPP

