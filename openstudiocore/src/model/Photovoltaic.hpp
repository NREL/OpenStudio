#ifndef MODEL_PHOTOVOLTAIC_HPP
#define MODEL_PHOTOVOLTAIC_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class Photovoltaic_Impl;

} // detail

class MODEL_API Photovoltaic : public ResourceObject{
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Photovoltaic(const Model& model);

  virtual ~Photovoltaic() {}

  //@}

  static IddObjectType iddObjectType();
  /** @name Getters */
  //@{

  std::string PVType() const;
  double PVTypeToValue();
  double PVTypeToValue(std::string type);
  double surfaceArea();
  double factionActive();
  double inverterEfficiency();
  double azimuthAngle();
  double inclinationAngle();
  double cellEfficiency();
  double gtEfficiency();
  double calculatePV();
  double calculatePV(double hours, double days);
  static std::vector<std::string> PVTypeValues();
  //@}
  /** @name Setters */
  //@{
  bool setPVType(std::string PVType);
  bool setSurfaceArea(double surfaceArea);
  bool setFactionActive(double factionActive);
  bool setInverterEfficiency(double inverterEfficiency);
  bool setAzimuthAngle(double azimuthAngle);
  bool setInclinationAngle(double inclinationAngle);
  bool setCellEfficiency(double cellEfficiency);
  bool setGTEfficiency(double gtEfficiency);

  //@}
  /** @name Other */
  //@{

  //@}

 protected:
  /// @cond
  typedef detail::Photovoltaic_Impl ImplType;

  explicit Photovoltaic(std::shared_ptr<detail::Photovoltaic_Impl> impl);

  friend class detail::Photovoltaic_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Photovoltaic");
};

/** \relates Photovoltaic*/
typedef boost::optional<Photovoltaic> OptionalPhotovoltaic;

/** \relates Photovoltaic*/
typedef std::vector<Photovoltaic> PhotovoltaicVector;

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAIC_HPP

