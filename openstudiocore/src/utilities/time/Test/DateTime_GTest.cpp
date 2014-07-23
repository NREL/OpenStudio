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

#include "../DateTime.hpp"
#include "../../core/Exception.hpp"

#include <string>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST(DateTime, Normalization)
{
  Date baseDate(MonthOfYear::Jan,1,2008);

  DateTime testDateTime(baseDate, Time(0,0,0,0));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2008));

  testDateTime = DateTime(baseDate, Time(0,0,0,1));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2008));

  testDateTime = DateTime(baseDate, Time(0,0,0,-1));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,31,2007));

  testDateTime = DateTime(baseDate, Time(0,23,59,59));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2008));

  testDateTime = DateTime(baseDate, Time(0,-23,-59,-59));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,31,2007));

  testDateTime = DateTime(baseDate, Time(1,0,0,0));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,2,2008));

  testDateTime = DateTime(baseDate, Time(-1,0,0,0));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,31,2007));

  testDateTime = DateTime(baseDate, Time(1,23,59,59));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,2,2008));

  testDateTime = DateTime(baseDate, Time(-1,-23,-59,-59));
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,30,2007));
};

TEST(DateTime, StringConstructor)
{
  DateTime dateTime(Date(MonthOfYear::Jan,1,2008), Time(0,0,0,0));

  std::stringstream ss;
  ss << dateTime;
  DateTime dateTime2(ss.str());

  EXPECT_TRUE(dateTime == dateTime2) << ss.str() << ", " << dateTime << " != " << dateTime2;

  dateTime = DateTime(Date(MonthOfYear::Jan,1,2008), Time(0,4,0,0));

  ss.str("");
  ss << dateTime;
  dateTime2 = DateTime(ss.str());

  EXPECT_TRUE(dateTime == dateTime2) << ss.str() << ", " << dateTime << " != " << dateTime2;

  dateTime = DateTime(Date(MonthOfYear::Jan,1,2008), Time(4,4,0,0));

  ss.str("");
  ss << dateTime;
  dateTime2 = DateTime(ss.str());

  EXPECT_TRUE(dateTime == dateTime2) << ss.str() << ", " << dateTime << " != " << dateTime2;

  dateTime = DateTime(Date(MonthOfYear::Jan,10,2008), Time(4,4,0,0));

  ss.str("");
  ss << dateTime;
  dateTime2 = DateTime(ss.str());

  EXPECT_TRUE(dateTime == dateTime2) << ss.str() << ", " << dateTime << " != " << dateTime2;

  dateTime = DateTime(Date(MonthOfYear::Jan,10), Time(4,4,0,0));

  ss.str("");
  ss << dateTime;
  dateTime2 = DateTime(ss.str());

  EXPECT_TRUE(dateTime == dateTime2) << ss.str() << ", " << dateTime << " != " << dateTime2;
}

TEST(DateTime, TMConstructor)
{
  time_t t = time(0);
  tm* lt = localtime(&t);
  DateTime zd(*lt);

  EXPECT_EQ(zd.date().dayOfMonth(), static_cast<unsigned>(lt->tm_mday));
  EXPECT_EQ(openstudio::month(zd.date().monthOfYear()) - 1, static_cast<unsigned>(lt->tm_mon));
  EXPECT_EQ(zd.date().year(), lt->tm_year+1900);

  EXPECT_EQ(zd.time().hours(), lt->tm_hour);
  EXPECT_EQ(zd.time().minutes(), lt->tm_min);
  EXPECT_EQ(zd.time().seconds(), lt->tm_sec);
}

TEST(DateTime, normalization2)
{
  Date baseDate(MonthOfYear::Jan,1);

  DateTime testDateTime(baseDate);
  testDateTime += Time(0,0,0,0);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2009));

  testDateTime = DateTime(baseDate);
  testDateTime -= Time(0,0,0,0);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2009));

  testDateTime = DateTime(baseDate);
  testDateTime += Time(0,0,0,1);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2009));

  testDateTime = DateTime(baseDate);
  testDateTime -= Time(0,0,0,1);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,31,2008));

  testDateTime = DateTime(baseDate);
  testDateTime += Time(0,23,59,59);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,1,2009));

  testDateTime = DateTime(baseDate);
  testDateTime -= Time(0,23,59,59);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,31,2008));

  testDateTime = DateTime(baseDate);
  testDateTime += Time(1,0,0,0);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,2,2009));

  testDateTime = DateTime(baseDate);
  testDateTime -= Time(1,0,0,0);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,31,2008));

  testDateTime = DateTime(baseDate);
  testDateTime += Time(1,23,59,59);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Jan,2,2009));

  testDateTime = DateTime(baseDate);
  testDateTime -= Time(1,23,59,59);
  EXPECT_TRUE(testDateTime.date() == Date(MonthOfYear::Dec,30,2008));
};

TEST(DateTime,EpochConversions) {
  DateTime dateTime = DateTime::now();
  std::time_t asEpoch = dateTime.toEpoch();
  DateTime copy = DateTime::fromEpoch(asEpoch);
  EXPECT_EQ(dateTime,copy);
}

TEST(DateTime,ISO8601Conversions) {
  DateTime dateTime = DateTime::now();
  std::string asIso = dateTime.toISO8601();
  OptionalDateTime copy = DateTime::fromISO8601(asIso);
  ASSERT_TRUE(copy);
  EXPECT_EQ(dateTime,copy.get());
}


/////////////////////////////////////////////////////////////
// Calendar for 2008.  Format is day of month, day of year //
/////////////////////////////////////////////////////////////
/*
January 2008
  Su      Mo      Tu      We      Th      Fr      Sa
                 1,  1   2,  2   3,  3   4,  4   5,  5
 6,  6   7,  7   8,  8   9,  9  10, 10  11, 11  12, 12
13, 13  14, 14  15, 15  16, 16  17, 17  18, 18  19, 19
20, 20  21, 21  22, 22  23, 23  24, 24  25, 25  26, 26
27, 27  28, 28  29, 29  30, 30  31, 31

February 2008
    Su      Mo      Tu      We      Th      Fr      Sa
                                         1, 32   2, 33
 3, 34   4, 35   5, 36   6, 37   7, 38   8, 39   9, 40
10, 41  11, 42  12, 43  13, 44  14, 45  15, 46  16, 47
17, 48  18, 49  19, 50  20, 51  21, 52  22, 53  23, 54
24, 55  25, 56  26, 57  27, 58  28, 59  29, 60

March 2008
    Su      Mo      Tu      We      Th      Fr      Sa
                                                 1, 61
 2, 62   3, 63   4, 64   5, 65   6, 66   7, 67   8, 68
 9, 69  10, 70  11, 71  12, 72  13, 73  14, 74  15, 75
16, 76  17, 77  18, 78  19, 79  20, 80  21, 81  22, 82
23, 83  24, 84  25, 85  26, 86  27, 87  28, 88  29, 89
30, 90  31, 91

April 2008
  Su      Mo      Tu      We      Th      Fr      Sa
                 1, 92   2, 93   3, 94   4, 95   5, 96
 6, 97   7, 98   8, 99   9,100  10,101  11,102  12,103
13,104  14,105  15,106  16,107  17,108  18,109  19,110
20,111  21,112  22,113  23,114  24,115  25,116  26,117
27,118  28,119  29,120  30,121

May 2008
    Su      Mo      Tu      We      Th      Fr      Sa
                                 1,122   2,123   3,124
 4,125   5,126   6,127   7,128   8,129   9,130  10,131
11,132  12,133  13,134  14,135  15,136  16,137  17,138
18,139  19,140  20,141  21,142  22,143  23,144  24,145
25,146  26,147  27,148  28,149  29,150  30,151  31,152

June 2008
    Su      Mo      Tu      We      Th      Fr      Sa
 1,153   2,154   3,155   4,156   5,157   6,158   7,159
 8,160   9,161  10,162  11,163  12,164  13,165  14,166
15,167  16,168  17,169  18,170  19,171  20,172  21,173
22,174  23,175  24,176  25,177  26,178  27,179  28,180
29,181  30,182

July 2008
  Su      Mo      Tu      We      Th      Fr      Sa
                 1,183   2,184   3,185   4,186   5,187
 6,188   7,189   8,190   9,191  10,192  11,193  12,194
13,195  14,196  15,197  16,198  17,199  18,200  19,201
20,202  21,203  22,204  23,205  24,206  25,207  26,208
27,209  28,210  29,211  30,212  31,213

August 2008
    Su      Mo      Tu      We      Th      Fr      Sa
                                         1,214   2,215
 3,216   4,217   5,218   6,219   7,220   8,221   9,222
10,223  11,224  12,225  13,226  14,227  15,228  16,229
17,230  18,231  19,232  20,233  21,234  22,235  23,236
24,237  25,238  26,239  27,240  28,241  29,242  30,243
31,244

September 2008
    Su      Mo      Tu      We      Th      Fr      Sa
         1,245   2,246   3,247   4,248   5,249   6,250
 7,251   8,252   9,253  10,254  11,255  12,256  13,257
14,258  15,259  16,260  17,261  18,262  19,263  20,264
21,265  22,266  23,267  24,268  25,269  26,270  27,271
28,272  29,273  30,274

October 2008
  Su      Mo      Tu      We      Th      Fr      Sa
                         1,275   2,276   3,277   4,278
 5,279   6,280   7,281   8,282   9,283  10,284  11,285
12,286  13,287  14,288  15,289  16,290  17,291  18,292
19,293  20,294  21,295  22,296  23,297  24,298  25,299
26,300  27,301  28,302  29,303  30,304  31,305

November 2008
    Su      Mo      Tu      We      Th      Fr      Sa
                                                 1,306
 2,307   3,308   4,309   5,310   6,311   7,312   8,313
 9,314  10,315  11,316  12,317  13,318  14,319  15,320
16,321  17,322  18,323  19,324  20,325  21,326  22,327
23,328  24,329  25,330  26,331  27,332  28,333  29,334
30,335

December 2008
    Su      Mo      Tu      We      Th      Fr      Sa
         1,336   2,337   3,338   4,339   5,340   6,341
 7,342   8,343   9,344  10,345  11,346  12,347  13,348
14,349  15,350  16,351  17,352  18,353  19,354  20,355
21,356  22,357  23,358  24,359  25,360  26,361  27,362
28,363  29,364  30,365  31,366
*/
/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
// Calendar for 2009.  Format is day of month, day of year //
/////////////////////////////////////////////////////////////
/*
January 2009
  Su      Mo      Tu      We      Th      Fr      Sa
                                 1,  1   2,  2   3,  3
 4,  4   5,  5   6,  6   7,  7   8,  8   9,  9  10, 10
11, 11  12, 12  13, 13  14, 14  15, 15  16, 16  17, 17
18, 18  19, 19  20, 20  21, 21  22, 22  23, 23  24, 24
25, 25  26, 26  27, 27  28, 28  29, 29  30, 30  31, 31

February 2009
    Su      Mo      Tu      We      Th      Fr      Sa
 1, 32   2, 33   3, 34   4, 35   5, 36   6, 37   7, 38
 8, 39   9, 40  10, 41  11, 42  12, 43  13, 44  14, 45
15, 46  16, 47  17, 48  18, 49  19, 50  20, 51  21, 52
22, 53  23, 54  24, 55  25, 56  26, 57  27, 58  28, 59

March 2009
    Su      Mo      Tu      We      Th      Fr      Sa
 1, 60   2, 61   3, 62   4, 63   5, 64   6, 65   7, 66
 8, 67   9, 68  10, 69  11, 70  12, 71  13, 72  14, 73
15, 74  16, 75  17, 76  18, 77  19, 78  20, 79  21, 80
22, 81  23, 82  24, 83  25, 84  26, 85  27, 86  28, 87
29, 88  30, 89  31, 90

April 2009
  Su      Mo      Tu      We      Th      Fr      Sa
                         1, 91   2, 92   3, 93   4, 94
 5, 95   6, 96   7, 97   8, 98   9, 99  10,100  11,101
12,102  13,103  14,104  15,105  16,106  17,107  18,108
19,109  20,110  21,111  22,112  23,113  24,114  25,115
26,116  27,117  28,118  29,119  30,120

May 2009
    Su      Mo      Tu      We      Th      Fr      Sa
                                         1,121   2,122
 3,123   4,124   5,125   6,126   7,127   8,128   9,129
10,130  11,131  12,132  13,133  14,134  15,135  16,136
17,137  18,138  19,139  20,140  21,141  22,142  23,143
24,144  25,145  26,146  27,147  28,148  29,149  30,150
31,151

June 2009
    Su      Mo      Tu      We      Th      Fr      Sa
         1,152   2,153   3,154   4,155   5,156   6,157
 7,158   8,159   9,160  10,161  11,162  12,163  13,164
14,165  15,166  16,167  17,168  18,169  19,170  20,171
21,172  22,173  23,174  24,175  25,176  26,177  27,178
28,179  29,180  30,181

July 2009
  Su      Mo      Tu      We      Th      Fr      Sa
                         1,182   2,183   3,184   4,185
 5,186   6,187   7,188   8,189   9,190  10,191  11,192
12,193  13,194  14,195  15,196  16,197  17,198  18,199
19,200  20,201  21,202  22,203  23,204  24,205  25,206
26,207  27,208  28,209  29,210  30,211  31,212

August 2009
    Su      Mo      Tu      We      Th      Fr      Sa
                                                 1,213
 2,214   3,215   4,216   5,217   6,218   7,219   8,220
 9,221  10,222  11,223  12,224  13,225  14,226  15,227
16,228  17,229  18,230  19,231  20,232  21,233  22,234
23,235  24,236  25,237  26,238  27,239  28,240  29,241
30,242  31,243

September 2009
    Su      Mo      Tu      We      Th      Fr      Sa
                 1,244   2,245   3,246   4,247   5,248
 6,249   7,250   8,251   9,252  10,253  11,254  12,255
13,256  14,257  15,258  16,259  17,260  18,261  19,262
20,263  21,264  22,265  23,266  24,267  25,268  26,269
27,270  28,271  29,272  30,273

October 2009
  Su      Mo      Tu      We      Th      Fr      Sa
                                 1,274   2,275   3,276
 4,277   5,278   6,279   7,280   8,281   9,282  10,283
11,284  12,285  13,286  14,287  15,288  16,289  17,290
18,291  19,292  20,293  21,294  22,295  23,296  24,297
25,298  26,299  27,300  28,301  29,302  30,303  31,304

November 2009
    Su      Mo      Tu      We      Th      Fr      Sa
 1,305   2,306   3,307   4,308   5,309   6,310   7,311
 8,312   9,313  10,314  11,315  12,316  13,317  14,318
15,319  16,320  17,321  18,322  19,323  20,324  21,325
22,326  23,327  24,328  25,329  26,330  27,331  28,332
29,333  30,334

December 2009
    Su      Mo      Tu      We      Th      Fr      Sa
                 1,335   2,336   3,337   4,338   5,339
 6,340   7,341   8,342   9,343  10,344  11,345  12,346
13,347  14,348  15,349  16,350  17,351  18,352  19,353
20,354  21,355  22,356  23,357  24,358  25,359  26,360
27,361  28,362  29,363  30,364  31,365
*/
/////////////////////////////////////////////////////////////
