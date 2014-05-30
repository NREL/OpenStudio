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

#ifndef UTILITIES_PLOT_BARCHART_HPP
#define UTILITIES_PLOT_BARCHART_HPP

#include "../UtilitiesAPI.hpp"

#include "../data/Vector.hpp"
#include "../data/Matrix.hpp"
#include "Chart.hpp"
#include "../core/Path.hpp"

namespace openstudio{

  /** class BarChart
  *
  *   Data for the chart are stored in a Matrix, with each column representing a bar and each row
  *   representing the stacked components of that bar.  xLabels correspond to labels
  *   for each bar and yLabels correspond to labels for each stacked component.  
  *  
  *   If number of rows = 1, then legend displays the xLabels, there are no x tick labels, 
  *   and each column gets a unique color. This is a non-stacked bar chart.
  *
  *   If number of rows > 1, then legend displays the yLabels, xLabels are displayed next 
  *   to tick mark for each bar, and each row gets a unique color.  This is a stacked bar chart.
  *
  *   Example usage:
  *
  *     Matrix data(2,2);
  *     data(0,0) = 10; // baseline electricity
  *     data(0,1) = 15; // baseline gas
  *     data(1,0) = 8; // proposed electricity
  *     data(1,1) = 11; // proposed gas
  *
  *     vector<std::string> xLabels(2); // which building
  *     xLabels[0] = "Baseline"; // baseline building
  *     xLabels[1] = "Proposed"; // better building
  *
  *     vector<std::string> yLabels(2); // fuel type
  *     yLabels[0] = "Electricity"; // baseline building
  *     yLabels[1] = "Gas"; // better building
  *
  *     BarChart barChart(data);
  *     barChart.setTitle("Baseline vs. Proposed Building Energy Use (GJ)");
  *     barChart.setXLabels(xLabels);
  *     barChart.setYLabels(yLabels);
  *
  */
  class UTILITIES_API BarChart{
  public:

    /// constructor takes data as a Vector
    /// note: equivalent to 1xN Matrix
    BarChart(const Vector& data);

    /// constructor takes data as a Matrix
    BarChart(const Matrix& data);

    /// copy constructor
    BarChart(const BarChart& other);

    /// get data as a Matrix
    const Matrix& data() const;

    /// set the title
    void setTitle(const std::string& title);

    /// set the x labels, does nothing if size of xLabels does not equal number of columns
    void setXLabels(const std::vector<std::string>& xLabels);

    /// set the y labels, does nothing if size of yLabels does not equal number of rows
    void setYLabels(const std::vector<std::string>& yLabels);

    /// set the colors of the bar chart
    /// if rows = 1, then colors is size of number of columns
    /// if rows > 1, then colors is size of number of rows
    void setColors(const std::vector<Color>& colors);

    /// set the colors of a given column
    /// colors is size of number of rows
    void setColors(unsigned columnIndex, const std::vector<Color>& colors);

    /// set the font and size of the title, use font size = 0 to hide
    void setTitleFont(unsigned fontSize, const Font& font);

    /// set the font and size of the x labels, use font size = 0 to hide
    void setXLabelFont(unsigned fontSize, const Font& font);

    /// set the font and size of the y labels, use font size = 0 to hide
    void setYLabelFont(unsigned fontSize, const Font& font);

    /// set the font and size of the legend, use font size = 0 to hide
    void setLegendFont(unsigned fontSize, const Font& font);

    /// set the legend location
    void setLegendLocation(LegendLocation loc);

    /// display the chart in QT
    // some functions to display the chart

    /// save the chart to file
    void save(const openstudio::path& path) const;

  };

  /** Class which represents a collection of bar charts, each bar chart is a named group.  
  *
  *   Example usage:
  *
  *     BarChart barChart1;
  *     BarChart barChart2;
  *     ....
  *     GroupedBarChart groupedChart();
  *     groupedChart.setTitle("Baseline vs. Proposed Building Energy Use (GJ)");
  *     groupedChart.addBarChart("Baseline", barChart1);
  *     groupedChart.addBarChart("Proposed", barChart2);
  *
  */
  class UTILITIES_API GroupedBarChart{

    /// constructor creates an empty chart
    GroupedBarChart();

    /// copy constructor
    GroupedBarChart(const GroupedBarChart& other);

    /// set the title
    void setTitle(const std::string& title);

    /// add a bar chart with a group name
    void addBarChart(const std::string& group, const BarChart& chart);

    /// set the font and size of the title, use font size = 0 to hide
    /// note that titles of the individual charts are suppressed
    void setTitleFont(unsigned fontSize, const Font& font);

    /// set the font and size of the x labels, use font size = 0 to hide
    void setGroupLabelFont(unsigned fontSize, const Font& font);

    /// set the spacing between groups relative to the group label font size
    void setGroupSpacing(unsigned spacing);

    /// specify which chart is used to determine the legend, defaults to first chart
    void setLegendChartIndex(unsigned index);

    /// set the legend location
    void setLegendLocation(LegendLocation loc);

    /// display the chart in QT
    // some functions to display the chart

    /// save the chart to file
    void save(const openstudio::path& path) const;

  };

} // openstudio

#endif //UTILITIES_PLOT_BARCHART_HPP
