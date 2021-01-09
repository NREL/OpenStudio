
#include "EPJSONTranslator.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include <json/json.h>
#include <fmt/format.h>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio::EPJSON {
std::string toJSONFieldName(const std::string& fieldName) {
  return openstudio::replace(
    openstudio::replace(
      openstudio::replace(
        openstudio::replace(openstudio::replace(openstudio::replace(boost::to_lower_copy(fieldName), " ", "_"), "-", "_"), "**", "_"), "*", "_"),
      ".", "_"),
    "%", "");
}

auto getGroupName(const Json::Value &schema, const std::string &type_description, const std::string& group_name) -> std::pair<std::string, bool> {
  const auto &properties = schema["properties"];
  const auto &property = properties[type_description];
  const auto &patternProperties = property["patternProperties"];

  const auto &objectProperties = patternProperties[patternProperties.getMemberNames()[0]]["properties"];

  for (const auto &propertyName : objectProperties.getMemberNames()) {
    const auto &objectProperty = objectProperties[propertyName];
    const auto &type = objectProperty["type"];
    if (type.isString()) {
      if (type.asString() == "array") {
        return {propertyName, true};
      }
    }
  }
  auto first_object_name = toJSONFieldName(group_name);

  if (first_object_name.find("load_range") == 0) {
    return {"range", false};
  }
  if (first_object_name.find("thermal_comfort_model_") == 0) {
    return {"thermal_comfort_model", false};
  }

  if (first_object_name.find("control_scheme_") == 0) {
    return {"control_scheme", false};
  }
  const auto first_space = first_object_name.find('_');
  if (first_space != std::string::npos) {
    first_object_name.erase(first_space);
  }
  return {first_object_name, false};
}


Json::Value loadJSONSchema() {
  Json::Value root;

  std::ifstream ifs;
  const auto schemaPath = openstudio::getEnergyPlusDirectory() / openstudio::toPath("Energy+.schema.epJSON");
  ifs.open(openstudio::toString(schemaPath));
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  [[maybe_unused]] bool success = parseFromStream(builder, ifs, &root, &errs);
  // todo handle errors here

  return root;
}

Json::Value toJSON(const openstudio::IdfFile& idf) {
  std::map<std::string, int> type_counts;

  Json::Value result;

  fmt::print("VERSION: {}\n", idf.version().str());

  Json::Value schema = loadJSONSchema();

  result["Version"]["Version 1"]["version_identifier"] = fmt::format("{}.{}", idf.version().major(), idf.version().minor());

  for (const auto& obj : idf.objects()) {
    if (obj.iddObject().type().value() == openstudio::IddObjectType::CommentOnly) {
      // we aren't translating comments it seems
      continue;
    }

    const auto& group = obj.iddObject().group();
    const auto& type = obj.iddObject().type().valueName();
    const auto& type_description = obj.iddObject().type().valueDescription();

    const auto& name = obj.name();
    const auto& defaultedName = obj.nameString(true);

    auto& json_group = result[type_description];

    const auto& usable_json_object_name = [&]() {
      if (name) {
        return *name;
      } else {
        if (!defaultedName.empty()) {
          return defaultedName;
        } else {
          return fmt::format("{} {}", type_description, ++type_counts[type_description]);
        }
      }
    }();

    auto& json_object = json_group[usable_json_object_name];
    json_object = Json::Value(Json::objectValue);

    //    fmt::print(" Group: '{}' Type: '{}' Description: '{}' Name: '{}' DefaultedName: '{}' Format: '{}'\n", group, type,
    //               obj.iddObject().type().valueDescription(), name ? *name : "UNDEFINED", defaultedName, obj.iddObject().properties().format);

    const auto insertGroupNumber = [](const std::size_t number, const std::string_view group_name, const std::string_view field_name) -> std::string {
      const auto nameLocation = field_name.find(group_name);
      assert(nameLocation != std::string_view::npos);
      if (group_name == "layer" && number == 1) {
        return "outside_layer";
      }
      const auto updated_group_name = [&]() -> std::string_view {
        if (group_name == "block" && field_name.find("block_size") == 0) {
          return "block_size";
        } else {
          return group_name;
        }
      }();

      return fmt::format("{}_{}{}", field_name.substr(0, nameLocation + updated_group_name.length()), number,
                         field_name.substr(nameLocation + updated_group_name.length()));
    };

    const auto updateValue = [](const auto& value, const auto& fieldName, const auto fieldType) -> std::string {
      static constexpr std::string_view values_to_update[] = {"yes",      "no",     "autosize", "normal",   "fanger",        "continuous",
                                                              "discrete", "hourly", "detailed", "schedule", "autocalculate", "correlation"};

      switch (fieldType.value()) {
        case openstudio::IddFieldType::ChoiceType:

        case openstudio::IddFieldType::RealType: {

          const auto lower = boost::to_lower_copy(value);

          if (lower == "naturalgas") {
            return "NaturalGas";
          }
          if (lower == "wetbulb") {
            return "WetBulb";
          }

          if (lower == "autocalculate" && toJSONFieldName(fieldName).find("maximum_flow") == 0) {
            return "Autosize";
          }

          if (lower == "autosize" && toJSONFieldName(fieldName) == "plant_loop_volume") {
            return "Autocalculate";
          }

          if (std::find(begin(values_to_update), end(values_to_update), lower) != end(values_to_update)) {
            return openstudio::toUpperCamelCase(boost::to_lower_copy(value));
          }
        }

        default:
          return value;
      }
    };


    const auto visitField = [updateValue](auto&& visitor, const openstudio::IddField& iddField, const auto& field, const auto idx) {
      if (iddField.properties().type.value() == openstudio::IddFieldType::IntegerType) {
        const auto fieldInt = field.getInt(idx);
        if (fieldInt) {
          visitor(*fieldInt);
          return;
        }
      }

      {
        const auto fieldDouble = field.getDouble(idx);

        if (fieldDouble) {
          const auto fieldInt = field.getInt(idx);

          if (fieldInt && static_cast<double>(*fieldInt) == *fieldDouble) {
            if (iddField.name().find("Number") != std::string::npos) {
              visitor(*fieldInt);
              return;
            }
          }

          visitor(*fieldDouble);
          return;
        }
      }

      {
        const auto fieldString = field.getString(idx);
        if (fieldString && !fieldString->empty()) {
          visitor(updateValue(*fieldString, iddField.name(), iddField.properties().type));

          return;
        }
      }
    };

    std::size_t cur_group_number = 0;

    for (const auto& g : obj.extensibleGroups()) {
      ++cur_group_number;
      // get first field and try to make a group name out of it
//      const auto [group_name, is_array_group] = openstudio::EPJSON::getGroupName(type_description, obj.iddObject().extensibleGroup()[0].name());

      const auto [group_name, is_array_group] = openstudio::EPJSON::getGroupName(schema, type_description, obj.iddObject().extensibleGroup()[0].name());

      auto& containing_json = [&json_object, &group_name = group_name, is_array_group = is_array_group ]() -> auto& {
        if (is_array_group) {
          auto& array_obj = json_object[group_name];
          return array_obj.append(Json::Value{});
        } else {
          return json_object;
        }
      }
      ();

      for (unsigned int idx = 0; idx < g.numFields(); ++idx) {
        const auto& iddField = obj.iddObject().extensibleGroup()[idx];

        if (!is_array_group) {
          const auto fieldName = insertGroupNumber(cur_group_number, group_name, toJSONFieldName(iddField.name()));
          visitField([&containing_json, &fieldName](const auto& value) { containing_json[fieldName] = value; }, iddField, g, idx);
        } else {
          const auto fieldName = toJSONFieldName(iddField.name());
          visitField([&containing_json, &fieldName](const auto& value) { containing_json[fieldName] = value; }, iddField, g, idx);
        }
      }
    }

    for (unsigned int idx = 0; idx < obj.numFields(); ++idx) {
      const auto& iddField = obj.iddObject().getField(idx);

      const auto fieldName = toJSONFieldName(iddField->name());
      //      fmt::print("   Inspecting Field: {} ({}) '{}'i [{}]\n", iddField->name(), fieldName, iddField->fieldId(),
      //                 iddField->properties().type.valueName());

      if (iddField->isNameField()) {
        // skip name, we already got that
        continue;
      }

      if (obj.iddObject().isExtensibleField(idx)) {
        // skip extensible field, we already dealt with that
        continue;
      }

      visitField([&json_object, &fieldName](const auto& value) { json_object[fieldName] = value; }, iddField.get(), obj, idx);
    }
  }
  return result;
}
}  // namespace openstudio::EPJSON
