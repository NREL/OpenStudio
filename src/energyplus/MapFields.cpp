/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "MapFields.hpp"

#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace energyplus {

  bool mapFields(const IdfObject& oldObject, IdfObject& newObject) {
    bool result = true;
    unsigned numFields = oldObject.numFields();
    for (unsigned i = 0; i < numFields; ++i) {
      if (auto s_ = oldObject.getString(i)) {
        result = result && newObject.setString(i, s_.get());
      }
    }
    return result;
  }

  bool mapFields(const IdfObject& oldObject, IdfObject& newObject, const std::vector<std::pair<unsigned, unsigned>>& fieldMap) {
    bool result = true;
    for (const auto& [oriIndex, targetIndex] : fieldMap) {
      if (auto s_ = oldObject.getString(oriIndex)) {
        result = result && newObject.setString(targetIndex, s_.get());
      }
    }
    return result;
  }

}  // namespace energyplus
}  // namespace openstudio
