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

#include "AnnotatedTimeline.hpp"

#include <iostream>
#include <set>
#include <limits>

#include <boost/filesystem/fstream.hpp>
#include <math.h>

using namespace boost::filesystem;
using namespace std;

namespace openstudio{

  /// add a time series
  void AnnotatedTimeline::addTimeSeries(const std::string& name, TimeSeries timeSeries)
  {
    // if null time series
    if (timeSeries.values().empty()){
      LOG(Error, "Null time series")
      return;
    }

    // if we already have a time series
    if (!m_storage.empty()){

      // get the first time series
      TimeSeries firstTimeSeries = m_storage.front().second;
    }

    // add the time series
    m_storage.push_back(ValueType(name, timeSeries));
  }

  /// get the java script portion
  std::string AnnotatedTimeline::script() const
  {
    stringstream result;
    result << "    <script type='text/javascript' src='http://www.google.com/jsapi'></script>" << endl;
    result << "    <script type='text/javascript'>" << endl;
    result << "      google.load('visualization', '1', {'packages':['annotatedtimeline']});" << endl;
    result << "      google.setOnLoadCallback(drawChart);" << endl;
    result << "      function drawChart() { var data = new google.visualization.DataTable();" << endl;
    writeColumns(result);
    writeData(result);
    result << "      var chart = new google.visualization.AnnotatedTimeLine(document.getElementById('" << m_divId << "'));" << endl;
    result << "      chart.draw(data, {displayAnnotations: true, legendPosition: 'newRow', displayAnnotationsFilter: true}); }" << endl;
    result << "    </script>" << endl;
    return result.str();
  }

  /// get the html body portion
  std::string AnnotatedTimeline::body() const
  {
    stringstream result;
    result << "    <div id='" << m_divId << "' style='width: " << m_pixelWidth << "px; height: " << m_pixelHeight << "px;'></div>" << endl;
    return result.str();
  }

  /// write to file
  void AnnotatedTimeline::save(const path& path) const
  {
    boost::filesystem::ofstream os(path);
    if (os.good()){
      write(os);
    }
  }

  /// write to file
  void AnnotatedTimeline::write(std::ostream& os) const
  {
    if (!m_storage.empty()){
      os << "<html>" << endl;
      os << "  <head>" << endl;
      os << script();
      os << "  </head>" << endl;
      os << "  <body>" << endl;
      os << body();
      os << "  </body>" << endl;
      os << "</html>" << endl;
    }
  }

  void AnnotatedTimeline::writeColumns(std::ostream& os) const
  {
    os << "      data.addColumn('date', 'Date');" << endl;

    unsigned number = 1;
    for (const ValueType& value : m_storage){
      os << "      data.addColumn('number', '" << value.first << "');" << endl;
      os << "      data.addColumn('string', 'title" << number << "');" << endl;
      os << "      data.addColumn('string', 'text" << number << "');" << endl;
      ++number;
    }
  }

  void AnnotatedTimeline::writeData(std::ostream& os) const
  {
    // set of all dates and times at which values are reported, sorted in ascending order
    std::set<DateTime> dateTimes;

    // loop over each time series and insert date times
    for (const ValueType& value : m_storage){
/*      for (const DateTime& dateTime : value.second.dateTimes()){
        dateTimes.insert(dateTime);
      }
 */
    for (const double& fracDays : value.second.daysFromFirstReport()){
        dateTimes.insert(value.second.firstReportDateTime()+Time(fracDays));
      }
    }

    // create storage for all the points
    os << "      data.addRows(" << dateTimes.size() << ");" << endl;

    // loop over each time series and insert date times
    double outOfRangeValue = std::numeric_limits<double>::max();
    for (const ValueType& value : m_storage){
      TimeSeries timeSeries = value.second;
      timeSeries.setOutOfRangeValue(outOfRangeValue);
    }

    // write out each row
    unsigned row = 0;
    for (const DateTime& dateTime : dateTimes){

      if (!os.good()) {
        LOG_AND_THROW("ostream in AnnotatedTimeline::writeData no longer good after writing row " << row - 1 << ".");
      }

      // write the date
      os << "      data.setValue(" << row << ", 0, new Date(" << dateTime.date().year()
         << ", " << month(dateTime.date().monthOfYear())-1 << ", " << dateTime.date().dayOfMonth()
         << ", " << dateTime.time().hours() << ", " << dateTime.time().minutes() << ", " << dateTime.time().seconds()
         << "));" << endl;

      // loop over each time series and insert date times
      unsigned column = 1;
      for (const ValueType& value : m_storage){
        TimeSeries timeSeries = value.second;
        double v = timeSeries.value(dateTime);
        if (v != outOfRangeValue){
          os << "      data.setValue(" << row << ", " << column << ", " << value.second.value(dateTime) << ");" << endl;
        }
        column+=3;
      }

      ++row;
    }
  }

} // openstudio
