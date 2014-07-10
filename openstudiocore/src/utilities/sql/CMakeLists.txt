set(sql_src
  sql/page.hpp
  sql/SqlFile.hpp
  sql/SqlFile.cpp
  sql/SqlFileEnums.hpp
  sql/SqlFileDataDictionary.hpp
  sql/SqlFile_Impl.hpp
  sql/SqlFile_Impl.cpp
  sql/SqlFileTimeSeriesQuery.hpp
  sql/SqlFileTimeSeriesQuery.cpp
)

set(sql_test_src
  sql/Test/IlluminanceMap_GTest.cpp
  sql/Test/SqlFileFixture.hpp
  sql/Test/SqlFileFixture.cpp
  sql/Test/SqlFile_GTest.cpp
  sql/Test/SqlFileTimeSeriesQuery_GTest.cpp
#  Copy Y:/5500/HPBldg/DannysFiles/eplusout.sql to build/resources/utilities folder before running SqlFileLargeFixture tests
#  sql/Test/SqlFileLargeFixture.hpp
#  sql/Test/SqlFileLargeFixture.cpp
#  sql/Test/SqlFileLarge_GTest.cpp
)

set(sql_swig_src
  sql/SqlFile.i
)
