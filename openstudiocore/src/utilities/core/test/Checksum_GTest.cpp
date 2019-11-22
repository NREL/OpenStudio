/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
  auto itStart = checksums.begin();
  ++itStart;
  for (auto it = checksums.begin(), itEnd = checksums.end();
       itStart != itEnd; ++it, ++itStart) {
    EXPECT_TRUE(std::find(itStart,itEnd,*it) == itEnd);
  }
}
