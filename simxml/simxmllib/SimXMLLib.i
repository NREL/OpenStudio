/**********************************************************************
 *  Copyright (c) 2008-2011, Alliance for Sustainable Energy.
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
#ifndef SIMXMLLIB_I
#define SIMXMLLIB_I

%{
  #include <XmlDoc.hpp>
  #include <XmlElement.hpp>
  #include <SimXmlDoc.hpp>
  #include <SimXmlElement.hpp>
  #include <simcore.hxx>
  #include <simres.hxx>
  #include <simgeom.hxx>
  #include <simbldg.hxx>
  #include <simmep.hxx>
  #include <simmodel.hxx>

  #include <QDomDocument>
  #include <QDomElement>
%}

%include <stl.i>

%template(DoubleVector) std::vector<double>;
%template(IntVector) std::vector<int>;
%template(StringVector) std::vector<std::string>;

%ignore QString;

%ignore std::vector<simxml::SimXmlElement>::vector(size_type);
%ignore std::vector<simxml::SimXmlElement>::resize(size_type);
%template(SimXmlElementVector) std::vector<simxml::SimXmlElement>;

%include <XmlDoc.hpp>
%include <XmlElement.hpp>
%include <SimXmlDoc.hpp>
%include <SimXmlElement.hpp>

%include <simcore.ixx>
%include <simres.ixx>
%include <simgeom.ixx>
%include <simbldg.ixx>
%include <simmep.ixx>
%include <simmodel.ixx>

%include <simcore.hxx>
%include <simres.hxx>
%include <simgeom.hxx>
%include <simbldg.hxx>
%include <simmep.hxx>
%include <simmodel.hxx>

#endif //SIMXMLLIB_I