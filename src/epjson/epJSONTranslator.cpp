
#include "epJSONTranslator.hpp"
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

namespace openstudio::epJSON {

enum class JSONValueType {
  Number,
  String,
  Array,
  Object,
  NumberOrString
};

const std::string &toJSONFieldName(std::map<std::string, std::string> &fieldNames, const std::string& fieldNameInput) {

  if (const auto &cached_value = fieldNames.find(fieldNameInput); cached_value != fieldNames.end())
  {
    return cached_value->second;
  }

  auto fieldName = boost::to_lower_copy(fieldNameInput);
  boost::replace_all(fieldName, " ", "_");
  boost::replace_all(fieldName, ".", "_");
  boost::replace_all(fieldName, "%", "");
  boost::replace_all(fieldName, "*", "_");
  boost::replace_all(fieldName, "**", "_");
  boost::replace_all(fieldName, "-", "_");
  boost::replace_all(fieldName, "/", "_");
  boost::replace_all(fieldName, "_", "_");
  boost::replace_all(fieldName, "(", "");
  boost::replace_all(fieldName, ")", "_");
  boost::replace_all(fieldName, "__", "_");
  boost::replace_all(fieldName, ":", "_");

  const auto cache_value = [&](const auto &input, const auto &value) -> const auto & {
    return fieldNames.emplace(input, value).first->second;
  };

  if (fieldName == "poissons_ratio") {
    return cache_value(fieldNameInput, "poisson_s_ratio");
  }

  if (fieldName == "youngs_modulus") {
    return cache_value(fieldNameInput, "young_s_modulus");
  }
  return cache_value(fieldNameInput, fieldName);
}


const Json::Value& getSchemaObjectProperties(const Json::Value& schema, const std::string& type_description) {
  const auto& properties = schema["properties"];
  const auto& property = properties[type_description];
  const auto& patternProperties = property["patternProperties"];

  return patternProperties[patternProperties.getMemberNames()[0]]["properties"];
}

JSONValueType schemaPropertyTypeDecode(const Json::Value &type)
{
  if (!type.isNull()) {
    if (type.asString() == "string") {
      return JSONValueType::String;
    } else if (type.asString() == "number") {
      return JSONValueType::Number;
    } else if (type.asString() == "object") {
      return JSONValueType::Object;
    } else if (type.asString() == "array") {
      return JSONValueType::Array;
    }
  }

  return JSONValueType::NumberOrString;
}

JSONValueType getSchemaObjectPropertyType(const Json::Value& schema, const std::string& type_description, const std::string& property_name) {
  const auto& properties = getSchemaObjectProperties(schema, type_description);
  const auto & property = properties[property_name];
  const auto &type = property["type"];
  return schemaPropertyTypeDecode(type);
}

JSONValueType getSchemaObjectPropertyType(const Json::Value& schema, const std::string& type_description, const std::string &group_name, const std::string& field_name) {
  const auto& properties = getSchemaObjectProperties(schema, type_description);
  const auto& property = properties[group_name];
  const auto& type = property["items"]["properties"][field_name]["type"];
  return schemaPropertyTypeDecode(type);
}

std::string fixupEnumerationValue(const Json::Value& schema, const std::string& value, const std::string& type_description,
                                  const std::string& group_name, const std::string& fieldName, const openstudio::IddFieldType fieldType) {
  if (fieldType == openstudio::IddFieldType::ChoiceType) {
    const auto& objectProperties = getSchemaObjectProperties(schema, type_description);

    const auto& field = [&]() {
      const auto& possible_field = objectProperties[fieldName]["enum"];
      if (!possible_field.isNull()) {
        return possible_field;
      }

      // todo handle error cases here
      return objectProperties[group_name]["items"]["properties"][fieldName]["enum"];
    }();

    const auto lower = boost::to_lower_copy(value);

    for (const auto& enumOption : field) {
      if (enumOption.isString()) {
        const auto& enumStr = enumOption.asString();
        if (boost::to_lower_copy(enumStr) == lower) {
          return enumStr;
        }
      }
    }

    // value wasn't found, so return passed-in value
    return value;
  }

  if (fieldType == openstudio::IddFieldType::RealType) {
    const auto& objectProperties = getSchemaObjectProperties(schema, type_description);
    const auto& field = objectProperties[fieldName]["anyOf"];
    if (field.isArray()) {
      const auto lower = boost::to_lower_copy(value);

      for (const auto& possibleValues : field) {
        const auto& enumOptions = possibleValues["enum"];
        if (enumOptions.isArray()) {
          for (const auto& enumOption : enumOptions) {
            if (enumOption.isString()) {
              const auto& enumStr = enumOption.asString();
              const auto lowerEnumStr = boost::to_lower_copy(enumStr);

              if (lowerEnumStr == lower) {
                return enumStr;
              }

              if (lowerEnumStr.find("auto") == 0 && lower.find("auto") == 0) {
                // it's the "auto" option, return it
                return enumStr;
              }
            }
          }
        }
      }
    }

    // value wasn't found, so return passed-in value
    return value;
  }

  // Not a real or enumeration type, return value
  return value;
}

auto getGroupName(std::map<std::pair<std::string, std::string>, std::pair<std::string, bool>> &group_names, 
                  std::map<std::string, std::string> &field_names, 
                  const Json::Value& schema, const std::string& type_description, const std::string& group_name) -> const auto & {

  const auto key = std::pair{type_description, group_name};

  if (const auto cached = group_names.find(key); cached != group_names.end()){
    return cached->second;
  }

  const auto cache_result = [&](const auto &name, const bool is_array) -> const auto &{
    return group_names.emplace(key, std::pair{name, is_array}).first->second;
  };

  const auto& objectProperties = getSchemaObjectProperties(schema, type_description);

  for (const auto& propertyName : objectProperties.getMemberNames()) {
    const auto& objectProperty = objectProperties[propertyName];
    const auto& type = objectProperty["type"];
    if (type.isString()) {
      if (type.asString() == "array") {
        return cache_result(propertyName, true);
      }
    }
  }
  auto first_object_name = toJSONFieldName(field_names, group_name);

  if (first_object_name.find("load_range") == 0) {
    return cache_result("range", false);
  }
  if (first_object_name.find("thermal_comfort_model_") == 0) {
    return cache_result("thermal_comfort_model", false);
  }

  if (first_object_name.find("control_scheme_") == 0) {
    return cache_result("control_scheme", false);
  }
  const auto first_space = first_object_name.find('_');
  if (first_space != std::string::npos) {
    first_object_name.erase(first_space);
  }
  return cache_result(first_object_name, false);
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

  std::map<std::pair<std::string, std::string>, std::pair<std::string, bool>> group_names;
  std::map<std::string, std::string> field_names;

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

    const bool is_fluid_properties_name = type_description.find("FluidProperties:Name") != std::string::npos;

    const auto& usable_json_object_name = [&]() {
      if (name && !is_fluid_properties_name) {
        return *name;
      } else {
        if (!defaultedName.empty() && !is_fluid_properties_name) {
          return defaultedName;
        } else {
          return fmt::format("{} {}", type_description, ++type_counts[type_description]);
        }
      }
    }();

    auto& json_object = json_group[usable_json_object_name];
    json_object = Json::Value(Json::objectValue);

    if (name && is_fluid_properties_name) {
      json_object["fluid_name"] = *name;
    }

    const auto insertGroupNumber = [](const std::size_t number, const std::string& group_name, std::string_view field_name) -> std::string {
      if (group_name == "layer" && number == 1) {
        return "outside_layer";
      }

      if (group_name == "outside" && field_name.find("outside_layer") == 0 && number == 1) {
        return std::string{field_name};
      }

      const auto updated_group_name = [&]() -> std::string_view {
        if (group_name == "block" && field_name.find("block_size") == 0) {
          return "block_size";
        } else if (group_name == "outside" && field_name.find("gap_") != std::string::npos) {
          return "gap";
        } else if (group_name == "outside" && field_name.find("outside_layer") == 0) {
          return "layer";
        } else if (group_name == "property" && field_name.find("property_value") == 0) {
          return "property_value";
        } else if (group_name == "equipment" && field_name == "demand_calculation_node_name") {
          return "demand_calculation";
        } else if (group_name == "equipment" && field_name == "setpoint_node_name") {
          return "setpoint";
        } else if (group_name == "equipment" && field_name == "operation_type") {
          return "operation";
        } else if (group_name == "equipment" && field_name == "component_flow_rate") {
          return "component";
        } else {
          return group_name;
        }
      }();

      if (updated_group_name == "layer" && field_name.find("outside_layer") == 0) {
        field_name.remove_prefix(8);
      }
      const auto nameLocation = field_name.find(updated_group_name);
      assert(nameLocation != std::string_view::npos);

      return fmt::format("{}_{}{}", field_name.substr(0, nameLocation + updated_group_name.length()), number,
                         field_name.substr(nameLocation + updated_group_name.length()));
    };

    const auto visitField = [&schema, &type_description](auto&& visitor, const openstudio::IddField& iddField, const std::string& group_name,
                                                        const auto& fieldName, const auto& field, const auto idx) -> bool {

      const auto jsonFieldType = [&](){
        if (group_name.empty()) {
          return getSchemaObjectPropertyType(schema, type_description, fieldName);
        } else {
          return getSchemaObjectPropertyType(schema, type_description, group_name, fieldName);
        }
      }();

      switch(jsonFieldType) {
        case JSONValueType::String: {
          const auto fieldString = field.getString(idx);
          if (fieldString && !fieldString->empty()) {
            visitor(fixupEnumerationValue(schema, *fieldString, type_description, group_name, fieldName, iddField.properties().type));
            return true;
          }
        }
        case JSONValueType::Number:
        case JSONValueType::NumberOrString: {
            const auto fieldDouble = field.getDouble(idx);

            if (fieldDouble) {
              const auto fieldInt = field.getInt(idx);

              if (fieldInt && static_cast<double>(*fieldInt) == *fieldDouble) {
                if (iddField.name().find("Number") != std::string::npos) {
                  visitor(*fieldInt);
                  return true;
                }
              }

              visitor(*fieldDouble);
              return true;
            }
        }
        case JSONValueType::Array:
        case JSONValueType::Object:
          break;
      }


      {
        const auto fieldString = field.getString(idx);
        if (fieldString && !fieldString->empty()) {
          visitor(fixupEnumerationValue(schema, *fieldString, type_description, group_name, fieldName, iddField.properties().type));

          return true;
        }
      }

      return false;
    };

    std::size_t cur_group_number = 0;

    for (const auto& g : obj.extensibleGroups()) {
      ++cur_group_number;
      // get first field and try to make a group name out of it
      //      const auto [group_name, is_array_group] = openstudio::epJSON::getGroupName(type_description, obj.iddObject().extensibleGroup()[0].name());

      const auto &[group_name, is_array_group] =
        openstudio::epJSON::getGroupName(group_names, field_names, schema, type_description, obj.iddObject().extensibleGroup()[0].name());

      auto& containing_json = [&json_object, &group_name = group_name, is_array_group = is_array_group ]() -> auto& {
        if (is_array_group) {
          auto& array_obj = json_object[group_name];
          return array_obj.append(Json::Value{Json::objectValue});
        } else {
          return json_object;
        }
      }
      ();

      for (unsigned int idx = 0; idx < g.numFields(); ++idx) {
        const auto& iddField = obj.iddObject().extensibleGroup()[idx];

        if (!is_array_group) {
          const auto fieldName = insertGroupNumber(cur_group_number, group_name, toJSONFieldName(field_names, iddField.name()));
          [[maybe_unused]] const auto fieldAdded = visitField([&containing_json, &fieldName](const auto& value) { containing_json[fieldName] = value; }, iddField,
                                             group_name, fieldName, g, idx);
 //         if (!fieldAdded) {
 //           containing_json[fieldName] = Json::Value{};
 //         }
        } else {
          const auto fieldName = toJSONFieldName(field_names, iddField.name());

          [[maybe_unused]] const auto fieldAdded = visitField([&containing_json, &fieldName](const auto& value) { containing_json[fieldName] = value; }, iddField,
                                             group_name, fieldName, g, idx);
//          if (!fieldAdded) {
//            containing_json[fieldName] = Json::Value{};
//          }
        }
      }
    }

    for (unsigned int idx = 0; idx < obj.numFields(); ++idx) {
      const auto& iddField = obj.iddObject().getField(idx);

      const auto &fieldName = toJSONFieldName(field_names, iddField->name());

      if (iddField->isNameField()) {
        // skip name, we already got that
        continue;
      }

      if (obj.iddObject().isExtensibleField(idx)) {
        // skip extensible field, we already dealt with that
        continue;
      }

      visitField([&json_object, &fieldName](const auto& value) { json_object[fieldName] = value; }, iddField.get(), "", fieldName, obj, idx);
    }
  }
  return result;
}
}  // namespace openstudio::epJSON
