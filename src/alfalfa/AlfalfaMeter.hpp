#ifndef ALFALFA_COMPONENT_METER_HPP
#define ALFALFA_COMPONENT_METER_HPP

#include "ComponentBase.hpp"
#include "../utilities/idf/IdfObject.hpp"

#include "AlfalfaAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaMeter : public ComponentBase {
    public:
    /**
       * Create an AlfalfaMeter Component from a meter_name which reflects the Output:Meter EneregyPlus IDD
       */
    AlfalfaMeter(const std::string& meter_name);

    /**
       * Create an AlfalfaMeter Component from an IdfObject.
       * Valid idf_object types are OutputMeter and IdfObject(IddObjectType::Output_Meter).
       */
    AlfalfaMeter(const IdfObject& output_meter);

    std::string deriveName() const override;

    Json::Value toJSON() const override;

    ComponentCapability capability() const override {
      return ComponentCapability::Output;
    }

    ComponentType type() const override {
      return ComponentType::Meter;
    }

    std::unique_ptr<ComponentBase> clone() const override {
        return std::make_unique<AlfalfaMeter>(*this);
    }

    std::string meterName() const;

    private:
    std::string m_meter_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
