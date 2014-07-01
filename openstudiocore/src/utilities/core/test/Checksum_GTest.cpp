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

#include <gtest/gtest.h>

#include "../Checksum.hpp"
#include "../UUID.hpp"
#include "../Containers.hpp"

#include <resources.hxx>

using openstudio::path;
using openstudio::toPath;
using openstudio::checksum;
using openstudio::createUUID;
using openstudio::StringVector;
using openstudio::toString;
using std::string;
using std::istream;
using std::ostream;
using std::stringstream;
using std::cout;
using std::endl;

TEST(Checksum, Strings)
{
  EXPECT_EQ("00000000", checksum(string("")));

  EXPECT_EQ("1AD514BA", checksum(string("Hi there")));

  EXPECT_EQ("D5682D26", checksum(string("HI there")));

  EXPECT_EQ("597EA479", checksum(string("Hithere")));

  EXPECT_EQ("17B88D3A", checksum(string("Hi there\r\nGoodbye")));
  EXPECT_EQ("17B88D3A", checksum(string("Hi there\nGoodbye")));

  EXPECT_EQ("F4CC67AC", checksum(string("Hi there\r\nGoodbye\r\n")));
  EXPECT_EQ("F4CC67AC", checksum(string("Hi there\nGoodbye\n")));
}

TEST(Checksum, Streams)
{
  stringstream ss;
  EXPECT_EQ(istream::goodbit, ss.rdstate());
  EXPECT_EQ("00000000", checksum(ss));

  // checksum tried to read from empty stream and both eof and fail bits are set
  EXPECT_EQ(istream::failbit | istream::eofbit, ss.rdstate());

  // need to clear the eofbit that was set when checksum read all the way through
  ss.clear();
  EXPECT_EQ(istream::goodbit, ss.rdstate());

  // write some more to the stringstream
  ss << "Hi there";
  EXPECT_EQ(istream::goodbit, ss.rdstate());
  EXPECT_EQ("1AD514BA", checksum(ss));

  // checksum read all the way through and eofbit is set
  EXPECT_EQ(istream::failbit | istream::eofbit, ss.rdstate());
  EXPECT_EQ("00000000", checksum(ss));
  EXPECT_EQ(istream::failbit | istream::eofbit, ss.rdstate());

  // need to clear the eofbit that was set when checksum read all the way through
  ss.clear();
  EXPECT_EQ(istream::goodbit, ss.rdstate());
  EXPECT_EQ("00000000", checksum(ss));
}

TEST(Checksum, Paths)
{
  // read a file, contents are "Hi there"
  path p = resourcesPath() / toPath("utilities/Checksum/Checksum.txt");
  EXPECT_EQ("1AD514BA", checksum(p));

  // read a file, contents are "Hi there\r\nGoodbye" on Windows and "Hi there\nGoodbye" otherwise
  p = resourcesPath() / toPath("utilities/Checksum/Checksum2.txt");
  EXPECT_EQ("17B88D3A", checksum(p));

  // checksum of a directory is "00000000"
  p = resourcesPath() / toPath("utilities/Checksum/");
  EXPECT_EQ("00000000", checksum(p));

  // if can't find file return "00000000"
  p = resourcesPath() / toPath("utilities/Checksum/NotAFile.txt");
  EXPECT_EQ("00000000", checksum(p));
}

TEST(Checksum, UUIDs) {
  StringVector checksums;
  for (unsigned i = 0, n = 1000; i < n; ++i) {
    checksums.push_back(checksum(toString(createUUID())));
  }
  StringVector::iterator itStart = checksums.begin();
  ++itStart;
  for (StringVector::iterator it = checksums.begin(), itEnd = checksums.end(); 
       itStart != itEnd; ++it, ++itStart) {
    EXPECT_TRUE(std::find(itStart,itEnd,*it) == itEnd);
  }
}
