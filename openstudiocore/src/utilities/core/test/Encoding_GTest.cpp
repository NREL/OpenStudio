/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#include <gtest/gtest.h>

#include "../Encoding.hpp"


using namespace openstudio;


TEST(Encoding, Compression)
{
  EXPECT_TRUE("Hello" == toStdString(toQByteArray("Hello")));
  EXPECT_TRUE("Hello" == toStdString(qUncompress(qCompress(toQByteArray("Hello"),9))));
  EXPECT_TRUE("Hello" == uncompress(compress("Hello", 9)));

  EXPECT_TRUE("Hello" == base64decode(base64encode("Hello")));
  EXPECT_TRUE("Hello" == uncompress(base64decode(base64encode(compress("Hello",9)))));

  EXPECT_TRUE("Hello" == uncompressFromBase64(compressToBase64("Hello", 9)));

}


