#include "AlfalfaJSON.hpp"
#include "../alfalfa/AlfalfaConstant.hpp"
#include "../alfalfa/AlfalfaMeter.hpp"
#include "../alfalfa/AlfalfaActuator.hpp"
#include "../alfalfa/AlfalfaOutputVariable.hpp"
#include "../alfalfa/AlfalfaGlobalVariable.hpp"
#include "../core/PathHelpers.hpp"

namespace openstudio {
namespace alfalfa {

  AlfalfaJSON::AlfalfaJSON() {}

  AlfalfaJSON::AlfalfaJSON(const std::string& s) {}

  AlfalfaJSON::AlfalfaJSON(const openstudio::path& p) {}

  AlfalfaPoint AlfalfaJSON::addConstant(const std::string display_name, const float value) {
    AlfalfaConstant component(value);
    AlfalfaPoint point(display_name);
    point.setInput(component);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  AlfalfaPoint AlfalfaJSON::addMeter(const std::string display_name, const std::string meter_name) {
    AlfalfaMeter component(meter_name);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  AlfalfaPoint AlfalfaJSON::addActuator(const std::string display_name, const std::string component_type, const std::string control_type,
                                        const std::string actuator_key) {
    AlfalfaActuator component(component_type, control_type, actuator_key);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    point.setInput(component);
    addPoint(point);
    return point;
  }

  AlfalfaPoint AlfalfaJSON::addOutputVariable(const std::string display_name, const std::string variable_name, const std::string variable_key) {
    AlfalfaOutputVariable component(variable_name, variable_key);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  AlfalfaPoint AlfalfaJSON::addGlobalVariable(const std::string display_name, const std::string variable_name) {
    AlfalfaGlobalVariable component(variable_name);
    AlfalfaPoint point(display_name);
    point.setInput(component);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  void AlfalfaJSON::addPoint(const AlfalfaPoint& point) {
    m_points.push_back(point);
  }

  bool AlfalfaJSON::saveAs(const openstudio::path& p) {
    m_JSONPath = p;
    return save();
  }

  bool AlfalfaJSON::save() const {
    if (makeParentFolder(m_JSONPath)) {
      Json::Value root;
      for (AlfalfaPoint point : m_points) {
        root[point.id()] = point.toJSON();
      }
      std::ofstream outFile(openstudio::toSystemFilename(m_JSONPath));

      if (outFile) {
        try {
          outFile << root;
          outFile.close();
          return true;
        } catch (...) {
          LOG(Error, "Unable to write file to path '" << toString(m_JSONPath) << "'.");
          return false;
        }
      }
    }
    LOG(Error, "Unable to write file to path '" << toString(m_JSONPath) << "', because parent directory "
                                                << "could not be created.");

    return false;
  }

}  // namespace alfalfa
}  // namespace openstudio
