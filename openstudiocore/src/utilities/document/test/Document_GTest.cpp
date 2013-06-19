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

#include <gtest/gtest.h>

#include <utilities/document/test/DocumentFixture.hpp>

#include <utilities/document/Document.hpp>
#include <utilities/document/HeadersAndFooters.hpp>
#include <utilities/document/Section.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Containers.hpp>

#include <resources.hxx>

#include <boost/filesystem/fstream.hpp>

#include <iomanip>

using openstudio::Document;
using openstudio::OptionalDocument;
using openstudio::toPath;
using openstudio::StringVector;
using openstudio::DocumentFormat;

TEST_F(DocumentFixture, ConstructGetSet)
{
  Document myDoc;
  EXPECT_EQ("",myDoc.title());
  EXPECT_TRUE(myDoc.authors() == StringVector());
  EXPECT_TRUE(myDoc.format() == DocumentFormat::LaTeX);
  EXPECT_TRUE(myDoc.header() == std::string());
  EXPECT_TRUE(myDoc.footer() == std::string());

  myDoc.setTitle("My Test Document");
  myDoc.addAuthor("Mickey Mouse");
  myDoc.addAuthor("Buggs Bunny");
  myDoc.setFormat(DocumentFormat::XHTML,openstudio::basicXHTMLHeader(),openstudio::basicXHTMLFooter());
  EXPECT_EQ("My Test Document",myDoc.title());
  EXPECT_EQ(static_cast<unsigned>(2),myDoc.authors().size());
  EXPECT_TRUE(myDoc.format() == DocumentFormat::XHTML);
  EXPECT_EQ(openstudio::basicXHTMLHeader(),myDoc.header());
  EXPECT_EQ(openstudio::basicXHTMLFooter(),myDoc.footer());
}

TEST_F(DocumentFixture, Queries)
{
  EXPECT_EQ(1u,doc.numElements());
  EXPECT_EQ(1u,doc.numSections());
  EXPECT_EQ(2u,doc.sections()[0].numElements());
}

TEST_F(DocumentFixture, Serialization)
{
  openstudio::path p = resourcesPath()/toPath("utilities/scratch/excerpt.osd");
  ASSERT_TRUE(openstudio::makeParentFolder(p));
  doc.save(p,true);

  OptionalDocument oDoc = Document::load(p);
  ASSERT_TRUE(oDoc);
  Document cloneDoc = *oDoc;
  EXPECT_EQ(1u,doc.numElements());
  EXPECT_EQ(1u,doc.numSections());
  EXPECT_EQ(2u,doc.sections()[0].numElements());
}

TEST_F(DocumentFixture, Print_COUT)
{
  doc.setFormat(DocumentFormat::COUT);
  LOG(Info,doc);
}

TEST_F(DocumentFixture, Print_LaTeX)
{
  doc.setFormat(DocumentFormat::LaTeX,openstudio::basicLaTeXArticleHeader(),openstudio::basicLaTeXFooter());
  openstudio::path p = toPath("excerpt.tex");
  doc.save(p,true);
}

TEST_F(DocumentFixture, Print_XHTML)
{
  doc.setFormat(DocumentFormat::XHTML,openstudio::basicXHTMLHeader(),openstudio::basicXHTMLFooter());
  openstudio::path p = toPath("excerpt.html");
  doc.save(p,true);
}
