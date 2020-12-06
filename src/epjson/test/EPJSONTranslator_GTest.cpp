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

  for (const auto &obj : idf->objects())
  {
    const auto &group = obj.iddObject().group();
    const auto &type = obj.iddObject().type().valueName();
    const auto &type_description = obj.iddObject().type().valueDescription();

    const auto &name = obj.name();
    const auto &defaultedName = obj.nameString(true);

    auto &json_group = result[type_description];

    const auto &usable_name = [&](){
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

    auto &json_object = json_group[usable_name];

    fmt::print(" Group: '{}' Type: '{}' Description: '{}' Name: '{}' DefaultedName: '{}'\n", group, type, obj.iddObject().type().valueDescription(), name?*name:"UNDEFINED", defaultedName);

    for (unsigned int idx = 0; idx < obj.numFields(); ++idx) {
      const auto &iddField = obj.iddObject().getField(idx);

      const auto fieldName = openstudio::replace(boost::to_lower_copy(iddField->name()), " ", "_");
      fmt::print("   Inspecting Field: {}", fieldName);

      if (iddField->isNameField()) {
        // skip name, we already got that
        continue;
      }
      switch (iddField->properties().type.value()) {
        case openstudio::IddFieldType::RealType: {
          const auto fieldReal = obj.getDouble(idx);
          if (fieldReal) {
            fmt::print("   Field: {} value (real): {}\n", iddField->name(), *fieldReal);
            json_object[fieldName] = *fieldReal;
          }
          break;
        }

        case openstudio::IddFieldType::IntegerType: {
          const auto fieldInt = obj.getInt(idx);
          if (fieldInt) {
            fmt::print("   Field: {} value (int): {}\n", iddField->name(), *fieldInt);
            json_object[fieldName] = *fieldInt;
          }
          break;
        }

        case openstudio::IddFieldType::AlphaType:
        case openstudio::IddFieldType::ExternalListType:
        case openstudio::IddFieldType::HandleType:
        case openstudio::IddFieldType::UnknownType:
        case openstudio::IddFieldType::ChoiceType:
        case openstudio::IddFieldType::ObjectListType:
        case openstudio::IddFieldType::NodeType:
        case openstudio::IddFieldType::URLType: {

          const auto fieldString = obj.getString(idx);

          if (fieldString && !fieldString.get().empty()) {
            fmt::print("   Field: {} value (string-like): {}\n", iddField->name(), *fieldString);
            json_object[fieldName] = *fieldString;
          }
          break;
        }
      }
    }
  }

  std::ofstream ofs("/home/jason/test_output.json", std::ofstream::trunc);
  ofs << result.toStyledString() << '\n';
}
