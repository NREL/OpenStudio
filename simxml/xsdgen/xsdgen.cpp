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

#include <XsdFile.hpp>
#include <SimXmlGenerator.hpp>

#include <iostream>

int main(int argc, char** argv)
{
  if (argc != 3){
    std::cout << "Usage: xsdgen /path/to/file.xsd /path/to/outDir/" << std::endl;
    return -1;
  }

  std::string xsdPath(argv[1]);

  std::string outDir(argv[2]);

  XsdFile xsdFile(xsdPath);

  SimXmlGenerator generator(xsdFile);

  return generator.generate(outDir);
}

