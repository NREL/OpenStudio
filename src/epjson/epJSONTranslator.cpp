/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "epJSONTranslator.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <json/json.h>
#include <fmt/format.h>
#include <vector>
#include <string_view>

namespace openstudio::epJSON {

template <typename... KeyType>
const Json::Value& safeLookupValue(const Json::Value& base, const KeyType&... keys) {
  const Json::Value* obj = &base;
  const Json::Value* last_found = &base;
  const Json::Value* last_null = nullptr;

  const auto lookupKey = [&obj, &last_found, &last_null](const auto& key) -> void {
    if (!obj->isNull() && obj->isObject()) {
      const Json::Value& next_obj = (*obj)[key];
      if (!next_obj.isNull()) {
        last_found = &next_obj;
      } else {
        last_null = &next_obj;
      }
      obj = &next_obj;
    }
  };

  (lookupKey(keys), ...);

  if (last_null != nullptr) {
    return *last_null;
  }

  OS_ASSERT(last_found);
  return *last_found;
}

enum class JSONValueType
{
  Number,
  Integer,
  String,
  Array,
  Object,
  NumberOrString
};

const std::string& toJSONFieldName(std::map<std::string, std::string>& fieldNames, const std::string& fieldNameInput) {

  if (const auto& cached_value = fieldNames.find(fieldNameInput); cached_value != fieldNames.end()) {
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

  const auto cache_value = [&](const auto& input, const auto& value) -> const auto& { return fieldNames.emplace(input, value).first->second; };

  using namespace std::literals::string_view_literals;

  static constexpr std::array<std::pair<std::string_view, std::string_view>, 13> manualFixPairs{{
    {"poissons_ratio"sv, "poisson_s_ratio"sv},
    {"youngs_modulus"sv, "young_s_modulus"sv},
    {"g_function_lnt_ts_value"sv, "g_function_ln_t_ts_value"sv},
    // E+ 22.2.0 introduced fields in coils like `2017 Rated Evaporator Fan Power per volume Flow Rate`
    // We CANNOT leave a field starting with `<number> xxxx` in the IDD, because an enum (or a variable) in C++ cannot start with a number
    // so a manual fix here is needed
    {"rated_evaporator_fan_power_per_volume_flow_rate_2017", "2017_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"rated_evaporator_fan_power_per_volume_flow_rate_2023", "2023_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_1_rated_evaporator_fan_power_per_volume_flow_rate_2017", "2017_speed_1_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_2_rated_evaporator_fan_power_per_volume_flow_rate_2017", "2017_speed_2_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_3_rated_evaporator_fan_power_per_volume_flow_rate_2017", "2017_speed_3_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_4_rated_evaporator_fan_power_per_volume_flow_rate_2017", "2017_speed_4_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_1_rated_evaporator_fan_power_per_volume_flow_rate_2023", "2023_speed_1_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_2_rated_evaporator_fan_power_per_volume_flow_rate_2023", "2023_speed_2_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_3_rated_evaporator_fan_power_per_volume_flow_rate_2023", "2023_speed_3_rated_evaporator_fan_power_per_volume_flow_rate"},
    {"speed_4_rated_evaporator_fan_power_per_volume_flow_rate_2023", "2023_speed_4_rated_evaporator_fan_power_per_volume_flow_rate"},
  }};

  if (auto it = std::find_if(manualFixPairs.cbegin(), manualFixPairs.cend(), [&fieldName](const auto& p) { return p.first == fieldName; });
      it != manualFixPairs.cend()) {
    return cache_value(fieldNameInput, it->second);
  }

  return cache_value(fieldNameInput, fieldName);
}

/** Locate the properties for a given object:
 *  schema root > properties > [type_description] (ObjectName) > patternProperties > "^.*\\S.*$" > properties */
const Json::Value& getSchemaObjectProperties(const Json::Value& schema, const std::string& type_description) {
  const auto& patternProperties = safeLookupValue(schema, "properties", type_description, "patternProperties");

  if (!patternProperties.isObject()) {
    LOG_FREE(LogLevel::Error, "epJSONTranslator", "Unable to find epJSON schema object for patternProperties for " << type_description);
    return patternProperties;
  }

  return safeLookupValue(patternProperties, patternProperties.getMemberNames()[0], "properties");
}

const Json::Value& getSchemaFieldNames(const Json::Value& schema, const std::string& type_description) {
  return safeLookupValue(schema, "properties", type_description, "legacy_idd", "fields");
}

JSONValueType schemaPropertyTypeDecode(const Json::Value& type) {
  if (type.isNull() || !type.isString()) {
    // LOG_FREE(LogLevel::Warn, "epJSONTranslator", "Unknown value passed to schemaPropertyTypeDecode, returning generic 'NumberOrString' Option");
    return JSONValueType::NumberOrString;
  }

  if (type.asString() == "string") {
    return JSONValueType::String;
  } else if (type.asString() == "number") {
    return JSONValueType::Number;
  } else if (type.asString() == "integer") {
    return JSONValueType::Integer;
  } else if (type.asString() == "object") {
    return JSONValueType::Object;
  } else if (type.asString() == "array") {
    return JSONValueType::Array;
  }

  return JSONValueType::NumberOrString;
}

/** Locate the properties for a given field of an object.
 * - group_name can be empty, or it's the name of the extensible group.
 * - field_name is the name of the field
 * - field_property_name is something like 'type', 'anyOf', or 'enum'
 *
 * if group_name is empty:
 *  getSchemaObjectProperties > [field_name] > [field_property_name]
 * else:
 *  getSchemaObjectProperties > [group_name] > items > properties > [field_name] > [field_property_name]
 */
const Json::Value& getSchemaObjectFieldProperty(const Json::Value& schema, const std::string& type_description, const std::string& group_name,
                                                const std::string& field_name, const std::string& field_property_name) {

  const auto& objectProperties = getSchemaObjectProperties(schema, type_description);

  if (group_name.empty()) {
    return safeLookupValue(objectProperties, field_name, field_property_name);
  } else {
    return safeLookupValue(objectProperties, group_name, "items", "properties", field_name, field_property_name);
  }
}

/** Find the 'type' property of a field and convert that to enum.*/
JSONValueType getSchemaObjectFieldPropertyType(const Json::Value& schema, const std::string& type_description, const std::string& group_name,
                                               const std::string& field_name) {
  JSONValueType type = schemaPropertyTypeDecode(getSchemaObjectFieldProperty(schema, type_description, group_name, field_name, "type"));
  if (type == JSONValueType::NumberOrString) {
    LOG_FREE(LogLevel::Warn, "epJSONTranslator",
             "Unknown value passed to schemaPropertyTypeDecode, returning generic 'NumberOrString' Option. "
               << "Occurred for type_description= " << type_description << ", group_name=" << group_name << ", field_name=" << field_name);
  }
  return type;
}

/** epJSON (unlike IDF) is case sensitive, so this routine find the correct 'enum' choice casing
 * It applies to fieldType = 'ChoiceType' or 'RealType' (since RealType can also be `anyOf` with values like 'Autosize' 'Autocalculate'))
 * eg: if given value='autosize', will convert it to 'Autosize' so that EnergyPlus' InputParser does recognize it */
std::string fixupEnumerationValue(const Json::Value& schema, const std::string& value, const std::string& type_description,
                                  const std::string& group_name, const std::string& field_name, const openstudio::IddFieldType fieldType) {

  if (fieldType == openstudio::IddFieldType::ChoiceType) {
    const auto& fieldProperty = getSchemaObjectFieldProperty(schema, type_description, group_name, field_name, "enum");
    const auto lower = boost::to_lower_copy(value);

    if (fieldProperty.isNull()) {
      LOG_FREE(LogLevel::Error, "epJSONTranslator", "Unable to find enum value for " << value << " in " << group_name << "::" << field_name)
      return value;
    }

    for (const auto& enumOption : fieldProperty) {
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

    const auto& fieldProperty = getSchemaObjectFieldProperty(schema, type_description, group_name, field_name, "anyOf");

    if (fieldProperty.isArray()) {
      const auto lower = boost::to_lower_copy(value);

      for (const auto& possibleValues : fieldProperty) {
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

/* Find name of an extensible group and cache it */
auto getGroupName(std::map<std::pair<std::string, std::string>, std::pair<std::string, bool>>& group_names,
                  std::map<std::string, std::string>& /*field_names*/, const Json::Value& schema, const std::string& type_description,
                  const std::string& group_name) -> const auto& {

  const auto key = std::pair{type_description, group_name};

  if (const auto cached = group_names.find(key); cached != group_names.end()) {
    return cached->second;
  }

  const auto cache_result = [&](const auto& name, const bool is_array) -> const auto& {
    return group_names.emplace(key, std::pair{name, is_array}).first->second;
  };

  const auto& objectProperties = getSchemaObjectProperties(schema, type_description);

  for (const auto& propertyName : objectProperties.getMemberNames()) {
    const auto& type = safeLookupValue(objectProperties, propertyName, "type");
    if (type.isString()) {
      if (type.asString() == "array") {
        return cache_result(propertyName, true);
      }
    }
  }

  // group name is irrelevant if it's not an array group
  return cache_result("", false);
}

openstudio::path defaultSchemaPath(openstudio::IddFileType filetype) {
  openstudio::path schemaPath;
  if (filetype == openstudio::IddFileType::EnergyPlus) {
    schemaPath = openstudio::getEnergyPlusDirectory() / openstudio::toPath("Energy+.schema.epJSON");
  } else {
    LOG_FREE(LogLevel::Error, "epJSONTranslator", "At the moment, only IddFileType::EnergyPlus is supported");
    // Later: use OpenStudio.epJSON
  }
  return schemaPath;
}

Json::Value loadJSON(const openstudio::path& path) {
  Json::Value root;

  std::ifstream ifs;
  ifs.open(openstudio::toString(path));
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  [[maybe_unused]] bool success = Json::parseFromStream(builder, ifs, &root, &errs);
  // todo handle errors here

  return root;
}

std::string getFieldName(const bool is_array, const IddObject& iddObject, const Json::Value& schema, const std::string& type_description,
                         const std::size_t group_number, const std::size_t field_number, std::string_view field_name) {
  if (is_array) {
    return std::string{field_name};
  }

  // Legacy IDD field names
  const auto& fieldNames = getSchemaFieldNames(schema, type_description);

  // use the index of the field inside of the IddObject to look up what its name should be
  // inside of the epJSON schema
  //
  // This is (partially) necessary because OpenStudio treats all groups as extensible.
  const auto& lookedUpFieldName =
    fieldNames[static_cast<int>((group_number - 1) * iddObject.extensibleGroup().size() + field_number + iddObject.nonextensibleFields().size())];

  LOG_FREE(LogLevel::Error, "epJSONTranslator", "Unable to look up field name for input field" << field_name)
  OS_ASSERT(lookedUpFieldName.isString());
  return lookedUpFieldName.asString();
}

Json::Value toJSON(const openstudio::IdfFile& idf, const openstudio::path& schemaPath) {

  openstudio::path schemaToLoad = schemaPath;
  if (schemaToLoad.empty()) {
    schemaToLoad = defaultSchemaPath(idf.iddFileType());
    if (schemaToLoad.empty()) {
      return Json::Value::null;
    }
  }

  std::map<std::string, int> type_counts;

  Json::Value result;

  std::map<std::pair<std::string, std::string>, std::pair<std::string, bool>> group_names;
  std::map<std::string, std::string> field_names;

  Json::Value schema = loadJSON(schemaToLoad);
  if (schema.isNull()) {
    LOG_FREE(LogLevel::Error, "epJSONTranslator", "Schema is invalid at path=" << schemaToLoad);
    return Json::Value::null;
  }

  result["Version"]["Version 1"]["version_identifier"] = fmt::format("{}.{}", idf.version().major(), idf.version().minor());

  for (const auto& obj : idf.objects()) {
    if (obj.iddObject().type().value() == openstudio::IddObjectType::CommentOnly) {
      // we aren't translating comments it seems
      continue;
    }

    const auto& type_description = obj.iddObject().type().valueDescription();

    const auto& name = obj.name();
    const auto& defaultedName = obj.nameString(true);

    auto& json_group = result[type_description];

    const bool is_fluid_properties_name = type_description.find("FluidProperties:Name") != std::string::npos;
    const bool is_lcc_use_price_escalation = type_description.find("LifeCycleCost:UsePriceEscalation") != std::string::npos;

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

    if (name) {
      if (is_fluid_properties_name) {
        json_object["fluid_name"] = *name;
      } else if (is_lcc_use_price_escalation) {
        json_object["lcc_price_escalation_name"] = *name;
      }
    }

    const auto visitField = [&schema, &type_description](auto&& visitor, const openstudio::IddField& iddField, const std::string& group_name,
                                                         const auto& fieldName, const auto& field, const auto idx) -> bool {
      const auto jsonFieldType = getSchemaObjectFieldPropertyType(schema, type_description, group_name, fieldName);

      switch (jsonFieldType) {
        case JSONValueType::String: {
          const auto fieldString = field.getString(idx);
          if (fieldString && !fieldString->empty()) {
            visitor(fixupEnumerationValue(schema, *fieldString, type_description, group_name, fieldName, iddField.properties().type));
            return true;
          }
        }
        case JSONValueType::Integer: {
          const auto fieldInt = field.getInt(idx);
          if (fieldInt) {
            visitor(*fieldInt);
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
      const auto& [group_name, is_array_group] =
        openstudio::epJSON::getGroupName(group_names, field_names, schema, type_description, obj.iddObject().extensibleGroup()[0].name());

      auto& containing_json = [&json_object, &group_name = group_name, is_array_group = is_array_group]() -> auto& {
        if (is_array_group) {
          auto& array_obj = json_object[group_name];
          return array_obj.append(Json::Value{Json::objectValue});
        } else {
          return json_object;
        }
      }();

      for (unsigned int idx = 0; idx < g.numFields(); ++idx) {
        const auto& iddField = obj.iddObject().extensibleGroup()[idx];

        const auto fieldName = getFieldName(is_array_group, obj.iddObject(), schema, type_description, cur_group_number, idx,
                                            toJSONFieldName(field_names, iddField.name()));

        [[maybe_unused]] const auto fieldAdded = visitField([&containing_json, &fieldName](const auto& value) { containing_json[fieldName] = value; },
                                                            iddField, group_name, fieldName, g, idx);
      }
    }

    for (unsigned int idx = 0; idx < obj.numFields(); ++idx) {
      const auto& iddField = obj.iddObject().getField(idx);

      const auto& fieldName = toJSONFieldName(field_names, iddField->name());

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

Json::Value toJSON(const openstudio::Workspace& workspace, const openstudio::path& schemaPath) {
  return toJSON(workspace.toIdfFile(), schemaPath);
}

std::string toJSONString(const openstudio::IdfFile& inputFile, const openstudio::path& schemaPath) {
  return toJSON(inputFile, schemaPath).toStyledString();
}

std::string toJSONString(const openstudio::Workspace& workspace, const openstudio::path& schemaPath) {
  return toJSON(workspace, schemaPath).toStyledString();
}

}  // namespace openstudio::epJSON
