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
#include "BCLFixture.hpp"

#include "../BCLComponent.hpp"
#include "../BCLMeasure.hpp"
#include "../LocalBCL.hpp"
#include "../RemoteBCL.hpp"
#include "../../data/Attribute.hpp"
#include "../../idd/IddFile.hpp"
#include "../../idf/Workspace.hpp"
#include "../../time/DateTime.hpp"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>

#include <time.h>

using namespace openstudio;

TEST_F(BCLFixture, LocalBCLTest)
{
  EXPECT_TRUE(LocalBCL::instance().setProdAuthKey(defaultProdAuthKey));
  EXPECT_EQ(defaultProdAuthKey, LocalBCL::instance().prodAuthKey());

  EXPECT_TRUE(LocalBCL::instance().setDevAuthKey(defaultDevAuthKey));
  EXPECT_EQ(defaultDevAuthKey, LocalBCL::instance().devAuthKey());
}

TEST_F(BCLFixture, RemoteBCLTest)
{
  RemoteBCL remoteBCL;

  // set temporary production auth key
  remoteBCL.setProdAuthKey(prodAuthKey);
  EXPECT_EQ(prodAuthKey, remoteBCL.prodAuthKey());
  EXPECT_EQ(remoteBCL.prodAuthKey(), remoteBCL.authKey());

  // set temporary development auth key
  remoteBCL.setDevAuthKey(devAuthKey);
  EXPECT_EQ(devAuthKey, remoteBCL.devAuthKey());

  // check that defaults to production url
  EXPECT_EQ(remoteBCL.remoteProductionUrl(), remoteBCL.remoteUrl());

  // get all roofs (not children), via empty first arg and non-null second string
  std::vector<BCLSearchResult> responses = remoteBCL.searchComponentLibrary("","Exterior Roof");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // test total result and page functions
  EXPECT_GT(remoteBCL.resultsPerQuery(),0);
  EXPECT_LT(remoteBCL.resultsPerQuery(),101);
  EXPECT_GT(remoteBCL.lastTotalResults(),0);
  EXPECT_GT(remoteBCL.numResultPages(),0);
  EXPECT_GT(remoteBCL.lastTotalResults(),remoteBCL.numResultPages());

  // get all roofs (not children), via wildcard first arg and non-null second string
  responses = remoteBCL.searchComponentLibrary("*","Exterior Roof");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all nonresidential components, via non-null first arg and wildcard second string
  responses = remoteBCL.searchComponentLibrary("ashrae","*");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all nonresidential roofs (not children), via non-null first arg and non-null second string
  responses = remoteBCL.searchComponentLibrary("ashrae","Exterior Roof");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all things wall, via non-null first string and and default arg second string
  responses = remoteBCL.searchComponentLibrary("wall","");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all constructions and children, via null first arg and tid
  responses = remoteBCL.searchComponentLibrary("",127);
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all constructions and children, via wildcard first arg and tid
  responses = remoteBCL.searchComponentLibrary("*",127);
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all constructions matching "office", via non-null first arg and tid
  responses = remoteBCL.searchComponentLibrary("office",127);
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // verify that different pages of results are returning different uids
  responses = remoteBCL.searchComponentLibrary("",127,0);
  unsigned numResponsesSearch1 = responses.size();
  ASSERT_GT(static_cast<int>(numResponsesSearch1),0);
  std::vector<BCLSearchResult> responses2 = remoteBCL.searchComponentLibrary("",127,1);
  unsigned numResponsesSearch2 = responses2.size();
  ASSERT_GT(static_cast<int>(numResponsesSearch2),0);
  EXPECT_NE(responses[0].uid(), responses2[0].uid());

  // get all denver weather files
  responses = remoteBCL.searchComponentLibrary("Denver","Weather File");
  ASSERT_GT(static_cast<int>(responses.size()),0);

  /// Download an individual component by uid and extract
  /// returns true if a download is started
  bool success = remoteBCL.downloadComponent(responses[1].uid());
  ASSERT_TRUE(success);

  /// Returns the last downloaded component if there is one
  boost::optional<BCLComponent> completed = remoteBCL.waitForComponentDownload();
  ASSERT_TRUE(completed);

// Remove comment block to test development server
/*

  remoteBCL.useRemoteDevelopmentUrl();

  // verify dev url and that change was applied
  EXPECT_EQ(remoteBCL.remoteDevelopmentUrl(), remoteBCL.remoteUrl());
  EXPECT_EQ(remoteBCL.devAuthKey(), remoteBCL.authKey());

  // get all roofs (not children), via empty first arg and non-null second string
  responses = remoteBCL.searchComponentLibrary("","Exterior Roof");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // test total result and page functions
  EXPECT_GT(remoteBCL.resultsPerQuery(),0);
  EXPECT_LT(remoteBCL.resultsPerQuery(),101);
  EXPECT_GT(remoteBCL.lastTotalResults(),0);
  EXPECT_GT(remoteBCL.numResultPages(),0);
  EXPECT_GT(remoteBCL.lastTotalResults(),remoteBCL.numResultPages());

  // get all roofs (not children), via wildcard first arg and non-null second string
  responses = remoteBCL.searchComponentLibrary("*","Exterior Roof");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all nonresidential components, via non-null first arg and wildcard second string
  responses = remoteBCL.searchComponentLibrary("ashrae","*");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all nonresidential roofs (not children), via non-null first arg and non-null second string
  responses = remoteBCL.searchComponentLibrary("ashrae","Exterior Roof");
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all things wall, via non-null first string and and default arg second string
  responses = remoteBCL.searchComponentLibrary("wall","");
  ASSERT_GT(static_cast<int>(responses.size()),0);

  // get all constructions and children, via null first arg and tid
  responses = remoteBCL.searchComponentLibrary("",127);
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all constructions and children, via wildcard first arg and tid
  responses = remoteBCL.searchComponentLibrary("*",127);
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // get all constructions matching "office", via non-null first arg and tid
  responses = remoteBCL.searchComponentLibrary("office",127);
  EXPECT_GT(static_cast<int>(responses.size()),0);

  // verify that different pages of results are returning different uids
  responses = remoteBCL.searchComponentLibrary("",127,0);
  numResponsesSearch1 = responses.size();
  EXPECT_GT(static_cast<int>(numResponsesSearch1),0);
  responses2 = remoteBCL.searchComponentLibrary("",127,1);
  numResponsesSearch2 = responses2.size();
  EXPECT_GT(static_cast<int>(numResponsesSearch2),0);
  EXPECT_NE(responses[0].uid(), responses2[0].uid());

  // get all denver weather files
  responses = remoteBCL.searchComponentLibrary("Denver","Weather File");
  ASSERT_GT(static_cast<int>(responses.size()),0);

  /// Download an individual component by uid and extract
  /// returns true if a download is started
  success = remoteBCL.downloadComponent(responses[1].uid());
  ASSERT_TRUE(success);

  /// Returns the last downloaded component if there is one
  completed = remoteBCL.waitForComponentDownload();
  ASSERT_TRUE(completed);
*/
}

TEST_F(BCLFixture, RemoteBCLTest2)
{
  time_t startTime;
  time(&startTime);

  RemoteBCL remoteBCL;

  // get all constructions, via empty first arg and tid
  std::vector<BCLSearchResult> responses = remoteBCL.searchComponentLibrary("",127);
  ASSERT_GT(responses.size(),0u);

  bool success = remoteBCL.downloadComponent(responses[0].uid());
  ASSERT_TRUE(success);

  boost::optional<BCLComponent> component = remoteBCL.waitForComponentDownload();
  ASSERT_TRUE(component);

  /// delete this component if we already have it
  boost::optional<BCLComponent> testComponent = LocalBCL::instance().getComponent(responses[0].uid(), responses[0].versionId());
  if(testComponent){
    bool test = LocalBCL::instance().removeComponent(*testComponent);
    EXPECT_TRUE(test);
  }
  testComponent = LocalBCL::instance().getComponent(responses[0].uid(), responses[0].versionId());
  EXPECT_FALSE(testComponent);

  /// Download an individual component by uid and extract
  /// returns true if a download is started
  success = remoteBCL.downloadComponent(responses[0].uid());
  ASSERT_TRUE(success);

  component = remoteBCL.waitForComponentDownload();
  ASSERT_TRUE(component);

  /// Returns the last downloaded component if there is one
  boost::optional<BCLComponent> lastDownload = remoteBCL.lastComponentDownload();
  ASSERT_TRUE(lastDownload);

  EXPECT_FALSE(lastDownload->files().empty());
  for (const std::string& file : lastDownload->files()) {
    openstudio::path path = toPath(lastDownload->directory() + "/files/" + file);
    EXPECT_TRUE(QDir().exists(toQString(path)));
    time_t time = QFileInfo(toQString(path)).lastModified().toTime_t();
    EXPECT_GT(time, startTime);
  }

  // Find in local library
  component = LocalBCL::instance().getComponent(responses[0].uid(), responses[0].versionId());
  ASSERT_TRUE(component);

  // check that actually was downloaded
  EXPECT_FALSE(component->files().empty());
  for (const std::string& file : component->files()) {
    openstudio::path path = toPath(component->directory() + "/files/" + file);
    EXPECT_TRUE(QDir().exists(toQString(path)));
    time_t time = QFileInfo(toQString(path)).lastModified().toTime_t();
    EXPECT_GT(time, startTime);
  }

  // read all attributes, look for "OpenStudio Type"
  std::string openstudioType;
  for (const Attribute& attribute : component->attributes()) {
    if (istringEqual("OpenStudio Type", attribute.name())){
      openstudioType = attribute.valueAsString();
      break;
    }
  }
  ASSERT_FALSE(openstudioType.empty());

  // load the component
  std::vector<std::string> oscFiles = component->files("osc");
  ASSERT_FALSE(oscFiles.empty());
  EXPECT_EQ(1u, oscFiles.size());
  openstudio::path oscPath = toPath(oscFiles[0]);
  EXPECT_TRUE(QDir().exists(toQString(oscPath)));
  // DLM: the real loading procedure would be to run this through the version translator first 
  boost::optional<Workspace> workspace = Workspace::load(oscPath, IddFile::catchallIddFile()); 
  // This will fail on Windows if the path is greater than MAX_PATH
  EXPECT_TRUE(workspace);

  // search for components by type
  std::vector<std::pair<std::string, std::string> > searchTerms;
  searchTerms.push_back(std::make_pair("OpenStudio Type", openstudioType));

  std::vector<BCLComponent> components = LocalBCL::instance().componentAttributeSearch(searchTerms);
  EXPECT_FALSE(components.empty());

  // check that search returns newly downloaded component
  bool found = false;
  for (const BCLComponent& testComponent : components) {
    if (component->uid() == testComponent.uid()){
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
  
}

TEST_F(BCLFixture, GetComponentByUID)
{
  std::string uid = "c2c40a00-5ea5-0130-aa1d-14109fdf0b37";
  std::string versionId = "0c316887-63ef-45a3-a132-3b0a1c566b77";

  /// delete this component if we already have it
  boost::optional<BCLComponent> testComponent = LocalBCL::instance().getComponent(uid, versionId);
  if(testComponent){
    bool test = LocalBCL::instance().removeComponent(*testComponent);
    EXPECT_TRUE(test);
  }
  testComponent = LocalBCL::instance().getComponent(uid, versionId);
  EXPECT_FALSE(testComponent);

  RemoteBCL remoteBCL;

  bool success = remoteBCL.downloadComponent(uid);
  ASSERT_TRUE(success);

  boost::optional<BCLComponent> component = remoteBCL.waitForComponentDownload();
  ASSERT_TRUE(component);
  EXPECT_EQ(uid, component->uid());

  /// Returns the last downloaded component if there is one
  boost::optional<BCLComponent> lastDownload = remoteBCL.lastComponentDownload();
  ASSERT_TRUE(lastDownload);
  EXPECT_EQ(uid, lastDownload->uid());

  testComponent = LocalBCL::instance().getComponent(uid, versionId);
  EXPECT_TRUE(testComponent);
  EXPECT_EQ(uid, testComponent->uid());
}

TEST_F(BCLFixture, RemoteBCLMetaSearchTest)
{
  RemoteBCL remoteBCL;

  typedef std::pair<std::string, uint> PairType;

  // get all constructions, via empty first arg and tid
  bool test = remoteBCL.metaSearchComponentLibrary("",127);
  ASSERT_TRUE(test);
  boost::optional<BCLMetaSearchResult> result = remoteBCL.waitForMetaSearch();
  ASSERT_TRUE(result);
  unsigned numResponsesSearch1 = result->numResults();
  EXPECT_GT(numResponsesSearch1, 0u);
  EXPECT_FALSE(result->facets().empty());
  for (const BCLFacet& facet : result->facets()) {
    EXPECT_FALSE(facet.items().empty());
    for (const PairType& item : facet.items()) {
      EXPECT_NE(item.first, "");
      EXPECT_GT(item.second, 0u);
    }
  }
  EXPECT_FALSE(result->taxonomyTerms().empty());


  // get all exterior wall constructions, via empty first arg and non-null second string
  test = remoteBCL.metaSearchComponentLibrary("","Exterior Wall");
  ASSERT_TRUE(test);
  result = remoteBCL.waitForMetaSearch();
  ASSERT_TRUE(result);
  unsigned numResponsesSearch2 = result->numResults();
  EXPECT_GT(numResponsesSearch2, 0u);
  EXPECT_GT(numResponsesSearch1, numResponsesSearch2);
  EXPECT_FALSE(result->facets().empty());
  for (const BCLFacet& facet : result->facets()) {
    EXPECT_FALSE(facet.items().empty());
    for (const PairType& item : facet.items()) {
      EXPECT_NE(item.first, "");
      EXPECT_GT(item.second, 0u);
    }
  }
  EXPECT_FALSE(result->taxonomyTerms().empty());

  // get all constructions, via non-null first and second strings
  test = remoteBCL.metaSearchComponentLibrary("office",127);
  ASSERT_TRUE(test);
  result = remoteBCL.waitForMetaSearch();
  ASSERT_TRUE(result);
  unsigned numResponsesSearch3 = result->numResults();
  EXPECT_GT(numResponsesSearch3, 0u);
  EXPECT_GT(numResponsesSearch1, numResponsesSearch3);
  EXPECT_FALSE(result->facets().empty());
  for (const BCLFacet& facet : result->facets()) {
    EXPECT_FALSE(facet.items().empty());
    for (const PairType& item : facet.items()) {
      EXPECT_NE(item.first, "");
      EXPECT_GT(item.second, 0u);
    }
  }
  EXPECT_FALSE(result->taxonomyTerms().empty());

  // get all things office, via non-null first string and and empty second string
  test = remoteBCL.metaSearchComponentLibrary("office","");
  ASSERT_TRUE(test);
  result = remoteBCL.waitForMetaSearch();
  ASSERT_TRUE(result);
  unsigned numResponsesSearch4 = result->numResults();
  EXPECT_GT(numResponsesSearch4, 0u);
  EXPECT_GE(numResponsesSearch4, numResponsesSearch3);
  EXPECT_FALSE(result->facets().empty());
  for (const BCLFacet& facet : result->facets()) {
    EXPECT_FALSE(facet.items().empty());
    for (const PairType& item : facet.items()) {
      EXPECT_NE(item.first, "");
      EXPECT_GT(item.second, 0u);
    }
  }
  EXPECT_FALSE(result->taxonomyTerms().empty());

  // there are no components in this category
  test = remoteBCL.metaSearchComponentLibrary("","Constructions");
  ASSERT_TRUE(test);
  result = remoteBCL.waitForMetaSearch();
  ASSERT_TRUE(result);
  unsigned numResponsesSearch5 = result->numResults();
  EXPECT_EQ(0u, numResponsesSearch5);
  EXPECT_TRUE(result->facets().empty());
  for (const BCLFacet& facet : result->facets()) {
    EXPECT_FALSE(facet.items().empty());
    for (const PairType& item : facet.items()) {
      EXPECT_NE(item.first, "");
      EXPECT_GT(item.second, 0u);
    }
  }
  EXPECT_TRUE(result->taxonomyTerms().empty());
}
