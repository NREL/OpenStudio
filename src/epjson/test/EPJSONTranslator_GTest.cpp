/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EPJSONFixture.hpp"
#include "../EPJSONTranslator.hpp"

#include "../../utilities/core/StringHelpers.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <fmt/format.h>
#include <json/json.h>

#include <resources.hxx>
#include <OpenStudio.hxx>

TEST_F(EPJSONFixture, TranslateIDFToEPJSON) {
  auto idf = openstudio::IdfFile::load("/home/jason/test.idf");
  ASSERT_TRUE(idf);

  Json::Value result;

  std::map<std::string, int> type_counts;

  for (const auto& obj : idf->objects()) {
    const auto& group = obj.iddObject().group();
    const auto& type = obj.iddObject().type().valueName();
    const auto& type_description = obj.iddObject().type().valueDescription();

    const auto& name = obj.name();
    const auto& defaultedName = obj.nameString(true);

    auto& json_group = result[type_description];
    const auto toJSONFieldName = [](const auto& fieldName) {
      return openstudio::replace(
        openstudio::replace(
          openstudio::replace(
            openstudio::replace(openstudio::replace(openstudio::replace(boost::to_lower_copy(fieldName), " ", "_"), "-", "_"), "**", "_"), "*", "_"),
          ".", "_"),
        "%", "");
    };

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

    fmt::print(" Group: '{}' Type: '{}' Description: '{}' Name: '{}' DefaultedName: '{}' Format: '{}'\n", group, type,
               obj.iddObject().type().valueDescription(), name ? *name : "UNDEFINED", defaultedName, obj.iddObject().properties().format);

    const auto insertGroupNumber = [](const std::size_t number, const std::string_view group_name, const std::string_view field_name) -> std::string {
      const auto nameLocation = field_name.find(group_name);
      assert(nameLocation != std::string_view::npos);
      if (group_name == "layer" && number == 1) {
        return "outside_layer";
      }
      return fmt::format("{}_{}{}", field_name.substr(0, nameLocation + group_name.length()), number,
                         field_name.substr(nameLocation + group_name.length()));
    };

    const auto getGroupName = [toJSONFieldName](const std::string_view& type_description, const auto& group) -> std::pair<std::string, bool> {
      auto first_object_name = toJSONFieldName(group[0].name());

      if (first_object_name == "field") {
        return {"data", true};
      }

      if (first_object_name.find("node_") != std::string::npos) {
        return {"node", true};
      }

      if (first_object_name.find("manager_") != std::string::npos) {
        return {"manager", true};
      }

      if (first_object_name.find("component_") == 0 && type_description.find("List") == std::string::npos) {
        return {"component", true};
      }
      if (first_object_name.find("variable_") != std::string::npos) {
        return {"variable_detail", true};
      }

      if (first_object_name.find("zone_equipment_") != std::string::npos) {
        return {"equipment", true};
      }

      if (first_object_name.find("branch_") != std::string::npos) {
        return {"branch", true};
      }
      if (first_object_name.find("vertex_") != std::string::npos && type_description.find("Fenestration") == std::string::npos) {
        return {"vertex", true};
      }

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
    };

    const auto updateValue = [](const auto& value, const auto fieldType) -> std::string {
      static constexpr std::string_view values_to_update[] = {"yes",      "no",     "autosize", "normal",  "fanger",  "continuous",
                                                              "discrete", "hourly", "detailed", "wetbulb", "schedule", "autocalculate", "correlation"};
      switch (fieldType.value()) {
        case openstudio::IddFieldType::ChoiceType:

        case openstudio::IddFieldType::RealType: {
          if (value == "AUTOCALCULATE") {
            return "Autosize";
          }

          const auto lower = boost::to_lower_copy(value);

          if (lower == "naturalgas") {
            return "NaturalGas";
          }

          if (std::find(begin(values_to_update), end(values_to_update), lower) != end(values_to_update)) {
            return openstudio::toUpperCamelCase(boost::to_lower_copy(value));
          }
        }

        default:
          return value;
      }
    };

    const auto pluralizeName = [](const auto& name) -> std::string {
      if (name == "data") {
        return "data";
      } else if (name == "vertex") {
        return "vertices";
      } else if (name == "branch") {
        return "branches";
      } else if (name == "equipment") {
        return "equipment";
      } else {
        return name + "s";
      }
    };

    const auto visitField = [updateValue](auto&& visitor, const auto& iddField, const auto& field, const auto idx) {
      {
        const auto fieldDouble = field.getDouble(idx);
        if (fieldDouble) {
          visitor(*fieldDouble);
          return;
        }
      }

      {
        const auto fieldInt = field.getInt(idx);
        if (fieldInt) {
          visitor(*fieldInt);
          return;
        }
      }

      {
        const auto fieldString = field.getString(idx);
        if (fieldString && !fieldString->empty()) {
          visitor(updateValue(*fieldString, iddField.properties().type));

          return;
        }
      }
    };

    std::size_t cur_group_number = 0;

    for (const auto& g : obj.extensibleGroups()) {
      ++cur_group_number;
      // get first field and try to make a group name out of it
      const auto [group_name, is_array_group] = getGroupName(type_description, obj.iddObject().extensibleGroup());

      fmt::print("Starting Group {}\n", group_name);

      auto& containing_json = [&json_object, pluralizeName, &group_name = group_name, is_array_group = is_array_group ]() -> auto& {
        if (is_array_group) {
          auto& array_obj = json_object[pluralizeName(group_name)];
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
      fmt::print("   Inspecting Field: {} ({}) '{}'i [{}]\n", iddField->name(), fieldName, iddField->fieldId(),
                 iddField->properties().type.valueName());

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

  std::ofstream ofs("/home/jason/test_output.json", std::ofstream::trunc);
  ofs << result.toStyledString() << '\n';
}
