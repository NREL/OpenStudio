/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include <resources.hxx>

#include "CoreFixture.hpp"
#include "../UnzipFile.hpp"
#include "../ZipFile.hpp"

#if (defined(_WIN32) || defined(_WIN64))
std::ostream& operator<<(std::ostream& t_o, const openstudio::path& t_path) {
  return t_o << openstudio::toString(t_path);
}
#endif

TEST_F(CoreFixture, Unzip_NonExistTest) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/blarg.zip");

  ASSERT_ANY_THROW(openstudio::UnzipFile f(p));
}

TEST_F(CoreFixture, Unzip_DirListTest) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");

  openstudio::UnzipFile uf(p);

  std::vector<openstudio::path> list = uf.listFiles();

  ASSERT_EQ(list.size(), 6u);

  EXPECT_EQ(list[0], openstudio::toPath("file1.txt"));
  EXPECT_EQ(list[1], openstudio::toPath("file2.txt"));
}

TEST_F(CoreFixture, Unzip_ExtractFileTest) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");

  openstudio::UnzipFile uf(p);

  openstudio::path outpath = openstudio::tempDir() / openstudio::toPath("ExtractFileTest");
  openstudio::filesystem::remove_all(outpath);

  openstudio::path outfile1 = outpath / openstudio::toPath("file2.txt");
  openstudio::path outfile2 = outpath / openstudio::toPath("testdir1/testdir2/file3.txt");
  openstudio::path outfile3 = outpath / openstudio::toPath("testdir1/testdir2/testpat.db");

  EXPECT_EQ(outfile1, uf.extractFile(openstudio::toPath("file2.txt"), outpath));

  ASSERT_TRUE(openstudio::filesystem::exists(outfile1));

  std::ifstream ifs(openstudio::toSystemFilename(outfile1));

  std::string line;
  std::getline(ifs, line);

  EXPECT_EQ("18 bytes of data.", line);

  EXPECT_EQ(outfile2, uf.extractFile(openstudio::toPath("testdir1/testdir2/file3.txt"), outpath));
  EXPECT_EQ(outfile3, uf.extractFile(openstudio::toPath("testdir1/testdir2/testpat.db"), outpath));

  ASSERT_TRUE(openstudio::filesystem::exists(outfile2));
  ASSERT_TRUE(openstudio::filesystem::exists(outfile3));
  EXPECT_EQ(0u, openstudio::filesystem::file_size(outfile2));
  EXPECT_EQ(112640u, openstudio::filesystem::file_size(outfile3));
}

TEST_F(CoreFixture, Unzip_ExtractAllFilesTest) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");

  openstudio::UnzipFile uf(p);

  openstudio::path outpath = openstudio::tempDir() / openstudio::toPath("ExtractAllFilesTest");
  openstudio::filesystem::remove_all(outpath);

  std::vector<openstudio::path> createdFiles = uf.extractAllFiles(outpath);

  ASSERT_EQ(4u, createdFiles.size());

  for (auto itr = createdFiles.begin(); itr != createdFiles.end(); ++itr) {
    EXPECT_TRUE(openstudio::filesystem::exists(*itr));
  }
}

TEST_F(CoreFixture, Zip_CreateFile) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");
  openstudio::path outpath = openstudio::tempDir() / openstudio::toPath("CreateFileTest");
  openstudio::path outzip = outpath / openstudio::toPath("new.zip");

  openstudio::filesystem::remove_all(outpath);

  {
    openstudio::filesystem::create_directories(outzip.parent_path());
    openstudio::ZipFile zf(outzip, false);
    zf.addFile(p, openstudio::toPath("added.zip"));
  }

  openstudio::UnzipFile uf(outzip);
  std::vector<openstudio::path> createdFiles = uf.extractAllFiles(outpath);

  ASSERT_EQ(1u, createdFiles.size());
  ASSERT_TRUE(openstudio::filesystem::exists(createdFiles[0]));
  EXPECT_EQ(openstudio::filesystem::file_size(p), openstudio::filesystem::file_size(createdFiles[0]));
}

TEST_F(CoreFixture, Zip_AppendFile) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");
  openstudio::path outpath = openstudio::tempDir() / openstudio::toPath("AppendFileTest");
  openstudio::path outzip = outpath / openstudio::toPath("new.zip");

  openstudio::filesystem::remove_all(outpath);

  {
    openstudio::filesystem::create_directories(outzip.parent_path());
    openstudio::ZipFile zf(outzip, false);
    zf.addFile(p, openstudio::toPath("added.zip"));
  }

  {
    openstudio::ZipFile zf(outzip, true);
    zf.addFile(p, openstudio::toPath("in/some/subdir/added2.zip"));
  }

  openstudio::UnzipFile uf(outzip);
  std::vector<openstudio::path> createdFiles = uf.extractAllFiles(outpath);

  ASSERT_EQ(2u, createdFiles.size());
  ASSERT_TRUE(openstudio::filesystem::exists(createdFiles[0]));
  ASSERT_TRUE(openstudio::filesystem::exists(createdFiles[1]));
  EXPECT_EQ(openstudio::filesystem::file_size(p), openstudio::filesystem::file_size(createdFiles[0]));
  EXPECT_EQ(openstudio::filesystem::file_size(p), openstudio::filesystem::file_size(createdFiles[1]));

  EXPECT_EQ(outpath / openstudio::toPath("in/some/subdir/added2.zip"), createdFiles[1]);
}
