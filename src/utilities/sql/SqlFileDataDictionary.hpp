/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_SQL_SQLFILEDATADICTIONARY_HPP
#define UTILITIES_SQL_SQLFILEDATADICTIONARY_HPP

#include "../data/TimeSeries.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>

namespace openstudio {
namespace detail {

  /// stores metadata from meter and variable data dictionaries
  struct UTILITIES_API DataDictionaryItem
  {
    int recordIndex;
    int envPeriodIndex;
    std::string name;
    std::string keyValue;
    std::string envPeriod;
    std::string reportingFrequency;
    std::string units;
    std::string table;
    openstudio::TimeSeries timeSeries;

    DataDictionaryItem(int recordIndex_, int envPeriodIndex_, const std::string& name_, const std::string& keyValue_, const std::string& envPeriod_,
                       const std::string& reportingFrequency_, const std::string& units_, const std::string& table_)
      : recordIndex(recordIndex_),
        envPeriodIndex(envPeriodIndex_),
        name(name_),
        keyValue(keyValue_),
        envPeriod(envPeriod_),
        reportingFrequency(reportingFrequency_),
        units(units_),
        table(table_) {}
  };

  /// tags for accessing DataDictionaryItems
  struct id
  {
  };
  struct name
  {
  };
  struct keyValue
  {
  };
  struct reportingFrequency
  {
  };
  struct envPeriod
  {
  };
  struct envPeriodSet
  {
  };
  struct envPeriodReportingFrequencyNameKeyValue
  {
  };

  /// Define a multi_index_container of DataDictionaryItems with following indices:
  /// a unique index sorted by dataDictionary::recordIndex and dataDictionary::envPeriodIndex
  /// a non-unique index sorted by dataDictionary::name,
  using DataDictionaryTable = boost::multi_index_container<
    DataDictionaryItem,
    boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<id>,
        boost::multi_index::composite_key<DataDictionaryItem, boost::multi_index::member<DataDictionaryItem, int, &DataDictionaryItem::recordIndex>,
                                          boost::multi_index::member<DataDictionaryItem, int, &DataDictionaryItem::envPeriodIndex>>>,
      boost::multi_index::ordered_non_unique<boost::multi_index::tag<name>,
                                             ::boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::name>>,
      boost::multi_index::ordered_non_unique<boost::multi_index::tag<keyValue>,
                                             ::boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::keyValue>>,
      boost::multi_index::ordered_non_unique<boost::multi_index::tag<reportingFrequency>,
                                             ::boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::reportingFrequency>>,
      boost::multi_index::ordered_non_unique<boost::multi_index::tag<envPeriod>,
                                             ::boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::envPeriod>>,
      boost::multi_index::ordered_non_unique<
        boost::multi_index::tag<envPeriodReportingFrequencyNameKeyValue>,
        boost::multi_index::composite_key<DataDictionaryItem,
                                          boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::envPeriod>,
                                          boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::reportingFrequency>,
                                          boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::name>,
                                          boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::keyValue>>>>>;

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_SQL_SQLFILEDATADICTIONARY_HPP
