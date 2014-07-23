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

#ifndef UTILITIES_PLOT_PIECHART_HPP
#define UTILITIES_PLOT_PIECHART_HPP

#include "../UtilitiesAPI.hpp"

#include "Chart.hpp"
#include "../data/Vector.hpp"
#include "../core/String.hpp"
#include "../core/Path.hpp"
#include "../core/Macro.hpp"

namespace openstudio{

  /** Class which represents a pie chart.  Data for the chart are stored in a Vector, 
  *   with each element corresponding to a slice in the chart and each slice is labeled.
  *   The size of the pie is normalized to the sum of the data, slices may be hidden to
  *   represent "missing" sections.
  *  
  */
  class UTILITIES_API PieChart{
  public:
    COMMON_PTR_TYPEDEFS(PieChart)

    /// constructor takes title, data, and labels
    /// note dimension of labels must match dimensions of data
    PieChart(const std::string& title, const Vector& data, 
             const std::vector<std::string>& labels);

    /// configuration options

    /// explode a given slice by index, multiple slices may be exploded at any time
    void explodeSlice(unsigned index);

    /// hide a given slice by index, multiple slices may be hidden at any time
    /// the label for the slice may also be hidden in the legend
    void hideSlice(unsigned index, bool hideLabel = false);

    /// set the colors of each slice, dimensions must match
    void setColors(const std::vector<Color>& colors);

    /// set the font and size of the title, use font size = 0 to hide
    void setTitleFont(unsigned fontSize, const Font& font);

    /// set the font and size of the labels, use font size = 0 to hide
    void setLabelFont(unsigned fontSize, const Font& font);

    /// set the font and size of the legend, use font size = 0 to hide
    void setLegendFont(unsigned fontSize, const Font& font);

    /// set the legend location
    void setLegendLocation(LegendLocation loc);

    /// display the chart in QT
    // some functions to display the chart

    /// save the chart to file
    void save(const openstudio::path& path) const;

  };

} // openstudio

#endif //UTILITIES_PLOT_PIECHART_HPP
