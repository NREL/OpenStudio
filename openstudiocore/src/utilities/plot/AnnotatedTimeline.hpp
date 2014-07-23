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

#ifndef UTILITIES_PLOT_ANNOTATEDTIMELINE_HPP
#define UTILITIES_PLOT_ANNOTATEDTIMELINE_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Macro.hpp"
#include "../data/TimeSeries.hpp"
#include "../core/Path.hpp"

namespace openstudio{

  class UTILITIES_API AnnotatedTimeline{
    private:

      // each value is a pair of name, time series
      typedef std::pair<std::string, TimeSeries> ValueType;

      // store a vector of named time series
      typedef std::vector<ValueType> StorageType;

    public:

      REGISTER_LOGGER("utilities.plot.AnnotatedTimeline");
      COMMON_PTR_TYPEDEFS(AnnotatedTimeline)

      /// constructor 
      AnnotatedTimeline() 
        : m_divId("chart_div"), m_pixelWidth(640), m_pixelHeight(480)
      {}

      /// create 
      static Ptr create() {return Ptr(new AnnotatedTimeline());}

      /// virtual destructor
      virtual ~AnnotatedTimeline() {}

      /// add a time series
      void addTimeSeries(const std::string& name, TimeSeries timeSeries);

      // set chart div id
      void divId(const std::string& id) {m_divId = id;}

      // change pixel sizes
      void pixelWidth(unsigned width) {m_pixelWidth = width;}
      void pixelHeight(unsigned height) {m_pixelHeight = height;}

      /// get the java script portion
      std::string script() const;

      /// get the html body portion
      std::string body() const;

      /// write to file
      void save(const path& path) const;

    private:

      // write methods
      void write(std::ostream& os) const;
      void writeColumns(std::ostream& os) const;
      void writeData(std::ostream& os) const;

      std::string m_divId;
      unsigned m_pixelWidth;
      unsigned m_pixelHeight;

      StorageType m_storage;
  };

} // openstudio

#endif // UTILITIES_PLOT_ANNOTATEDTIMELINE_HPP
