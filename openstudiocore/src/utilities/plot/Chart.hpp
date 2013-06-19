/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_PLOT_CHART_HPP
#define UTILITIES_PLOT_CHART_HPP

namespace openstudio{

  /// type used for color
  class Color; // get this from somewhere in QT?

  /// type used for font
  class Font; // get this from somewhere in QT?
   
  /// options for legend location
  /// defaultPos = upper right hand corner
  /// upperHorizontal = single row underneath top of box
  /// lowerHorizontal = single row above bottom of box
  enum LegendLocation{DefaultPos, UpperHorizontal, LowerHorizontal};

} // openstudio

#endif //UTILITIES_PLOT_CHART_HPP
