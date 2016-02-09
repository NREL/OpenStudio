/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "OSMeasure.hpp"

namespace openstudio {
namespace measure {

OSMeasure::~OSMeasure()
{}

std::string OSMeasure::name() const{
  return "";
}

std::string OSMeasure::description() const{
  return "";
}

std::string OSMeasure::taxonomy() const{
  return "";
}

std::string OSMeasure::modeler_description() const{
  return "";
}

bool OSMeasure::registerWithSketchUp() const {
  return false;
}

bool OSMeasure::registerWithApplication() const {
  return false;
}

} // measure
} // openstudio
