/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
