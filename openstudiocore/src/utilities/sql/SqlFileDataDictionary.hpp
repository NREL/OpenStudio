/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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

    DataDictionaryItem(int recordIndex_,int envPeriodIndex_, std::string name_, std::string keyValue_, std::string envPeriod_, std::string reportingFrequency_, std::string units_, std::string table_):recordIndex(recordIndex_),envPeriodIndex(envPeriodIndex_), name(name_), keyValue(keyValue_), envPeriod(envPeriod_), reportingFrequency(reportingFrequency_), units(units_), table(table_) {}
  };

  /// tags for accessing DataDictionaryItems
  struct id{};
  struct name{};
  struct keyValue{};
  struct reportingFrequency{};
  struct envPeriod{};
  struct envPeriodSet{};
  struct envPeriodReportingFrequencyNameKeyValue{};

  /// Define a multi_index_container of DataDictionaryItems with following indices:
  /// a unique index sorted by dataDictionary::recordIndex and dataDictionary::envPeriodIndex
  /// a non-unique index sorted by dataDictionary::name,
  typedef boost::multi_index_container <
    DataDictionaryItem,
    boost::multi_index::indexed_by <
    boost::multi_index::ordered_unique < boost::multi_index::tag<id>, 
      boost::multi_index::composite_key <
        DataDictionaryItem, 
        boost::multi_index::member < DataDictionaryItem, int, &DataDictionaryItem::recordIndex>,
        boost::multi_index::member < DataDictionaryItem, int, &DataDictionaryItem::envPeriodIndex>
      >
    >,
    boost::multi_index::ordered_non_unique < boost::multi_index::tag<name>, BOOST_MULTI_INDEX_MEMBER(DataDictionaryItem, std::string,name)>,
    boost::multi_index::ordered_non_unique < boost::multi_index::tag<keyValue>, BOOST_MULTI_INDEX_MEMBER(DataDictionaryItem, std::string, keyValue)>,
    boost::multi_index::ordered_non_unique < boost::multi_index::tag<reportingFrequency>, BOOST_MULTI_INDEX_MEMBER(DataDictionaryItem, std::string, reportingFrequency)>,
    boost::multi_index::ordered_non_unique < boost::multi_index::tag<envPeriod>, BOOST_MULTI_INDEX_MEMBER(DataDictionaryItem, std::string, envPeriod)>,
    boost::multi_index::ordered_non_unique < boost::multi_index::tag<envPeriodReportingFrequencyNameKeyValue>, 
      boost::multi_index::composite_key <
      DataDictionaryItem, 
        boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::envPeriod>,
        boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::reportingFrequency>,
        boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::name>,
        boost::multi_index::member<DataDictionaryItem, std::string, &DataDictionaryItem::keyValue>
      > 
    > 
    > 
  > DataDictionaryTable;

} // detail
} // openstudio

#endif // UTILITIES_SQL_SQLFILEDATADICTIONARY_HPP
