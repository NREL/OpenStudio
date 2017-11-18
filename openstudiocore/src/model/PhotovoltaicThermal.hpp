#ifndef MODEL_PHOTOVOLTAICTHERMAL_HPP
#define MODEL_PHOTOVOLTAICTHERMAL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PhotovoltaicThermal_Impl;

} // detail

class MODEL_API PhotovoltaicThermal : public ResourceObject{
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PhotovoltaicThermal(const Model& model);

  virtual ~PhotovoltaicThermal() {}

  //@}

  static IddObjectType iddObjectType();
  /** @name Getters */
  //@{

  std::string PVType() const;
  double PVTypeToValue();
  double PVTypeToValue(std::string type);
  double surfaceArea();
  double factionActive();
  double azimuthAngle();
  double inclinationAngle();
  double cellEfficiency();
  double gtEfficiency();
  double boilerEfficiency();
  double collectorEfficiency();
  double calculatePV();
  double calculatePV(double hours, double days);
  static std::vector<std::string> PVTypeValues();
  //@}
  /** @name Setters */
  //@{
  bool setPVType(std::string PVType);
  bool setSurfaceArea(double surfaceArea);
  bool setFactionActive(double factionActive);
  bool setAzimuthAngle(double azimuthAngle);
  bool setInclinationAngle(double inclinationAngle);
  bool setCellEfficiency(double cellEfficiency);
  bool setGTEfficiency(double gtEfficiency);
  bool setBoilerEfficiency(double boilerEfficiency);
  bool setCollectorEfficiency(double collectorEfficiency);

  //@}
  /** @name Other */
  //@{

  //@}

 protected:
  /// @cond
  typedef detail::PhotovoltaicThermal_Impl ImplType;

  explicit PhotovoltaicThermal(std::shared_ptr<detail::PhotovoltaicThermal_Impl> impl);

  friend class detail::PhotovoltaicThermal_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PhotovoltaicThermal");
};

/** \relates PhotovoltaicThermal*/
typedef boost::optional<PhotovoltaicThermal> OptionalPhotovoltaicThermal;

/** \relates PhotovoltaicThermal*/
typedef std::vector<PhotovoltaicThermal> PhotovoltaicThermalVector;

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICTHERMAL_HPP

