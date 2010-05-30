/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Parse Test                                                    *
 * Author: Arash Partow (2002-2010)                              *
 * URL: http://www.partow.net/programming/strtk/index.html       *
 *                                                               *
 * Copyright notice:                                             *
 * Free use of the String Toolkit Library is permitted under the *
 * guidelines and in accordance with the most current version of *
 * the Common Public License.                                    *
 * http://www.opensource.org/licenses/cpl1.0.php                 *
 *                                                               *
 *****************************************************************
*/


/*
   Description: The Parse Test performs a simple parsing operation
                upon a string containing delimited values into 12
                relevant types from a struct named data_block. The
                test process is timed and once completed statistics
                about run-time and parsing rates is printed.
*/


#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "strtk.hpp"

#ifdef WIN32
 #include <windows.h>
#else
 #include <sys/time.h>
 #include <sys/types.h>
#endif

#ifdef WIN32

class timer
{
public:
   timer()      { QueryPerformanceFrequency(&clock_frequency); }
   void start() { QueryPerformanceCounter(&start_time);        }
   void stop()  { QueryPerformanceCounter(&stop_time);         }
   double time(){ return (1.0 *(stop_time.QuadPart - start_time.QuadPart)) / (1.0 * clock_frequency.QuadPart); }

private:
   LARGE_INTEGER start_time;
   LARGE_INTEGER stop_time;
   LARGE_INTEGER clock_frequency;
};

#else

class timer
{
public:
   void start() { gettimeofday(&start_time, 0); }
   void stop()  { gettimeofday(&stop_time,  0); }
   double time()
   {
      double diff = (stop_time.tv_sec - start_time.tv_sec) * 1000000.0;
      if (stop_time.tv_usec > start_time.tv_usec)
         diff += (1.0 * (stop_time.tv_usec - start_time.tv_usec));
      else if (stop_time.tv_usec < start_time.tv_usec)
         diff -= (1.0 * (start_time.tv_usec - stop_time.tv_usec));
      return (diff / 1000000.0);
   }
 private:
  struct timeval start_time;
  struct timeval stop_time;
  struct timeval clock_frequency;
};

#endif

std::size_t title_length()
{
   static const std::string s[] =
                            {
                               "[data into struct] ",
                               "[integer into " + strtk::type_name(std::vector<int>()) + "] ",
                               "[double into " + strtk::type_name(std::vector<double>()) + "] ",
                               "[tokenizer(md) raw speed test] ",
                               "[tokenizer(sd) raw speed test] ",
                               "[split(md) raw speed test] ",
                               "[split(sd) raw speed test] "
                            };
   std::size_t length = 0;
   length = std::max<std::size_t>(length,s[0].size());
   length = std::max<std::size_t>(length,s[1].size());
   length = std::max<std::size_t>(length,s[2].size());
   length = std::max<std::size_t>(length,s[3].size());
   length = std::max<std::size_t>(length,s[4].size());
   length = std::max<std::size_t>(length,s[5].size());
   length = std::max<std::size_t>(length,s[6].size());
   return length;
}

struct data_block
{
   std::string    d1;
   char           d2;
   int            d3;
   unsigned int   d4;
   double         d5;
   float          d6;
   short          d7;
   unsigned short d8;
   bool           d9;
   unsigned char  d10;
   long           d11;
   unsigned long  d12;
};


void parse_test01()
{
   std::cout << strtk::text::left_align(title_length(),' ',"[data into struct]");
   data_block i1;
   i1.d1  = "The1 quick2 brown3 fox4 jumps5 over6 the7 lazy8 dog9";
   i1.d2  = 'x';
   i1.d3  = -1234;
   i1.d4  = 78901;
   i1.d5  = 4567.8901;
   i1.d6  = 123.456f;
   i1.d7  = -16000;
   i1.d8  = 15000;
   i1.d9  = true;
   i1.d10 = 0xEE;
   i1.d11 = -737373;
   i1.d12 = 333777l;

   data_block i2;
   i2.d1  = "The9 quick8 brown7 fox6 jumps5 over4 the3 lazy2 dog1";
   i2.d2  = 'A';
   i2.d3  = -4321;
   i2.d4  = 11111;
   i2.d5  = 98765.12345;
   i2.d6  = 123.456f;
   i2.d7  = -11111;
   i2.d8  = 13333;
   i2.d9  = true;
   i2.d10 = 0xA5;
   i2.d11 = -737373;
   i2.d12 = 333777l;

   std::string str1 = "";
   std::string str2 = "";

   strtk::construct(str1,
                    "|",
                     i1.d1, i1.d2, i1.d3,
                     i1.d4, i1.d5, i1.d6,
                     i1.d7, i1.d8, i1.d9,
                    i1.d10,i1.d11,i1.d12);

   strtk::construct(str2,
                    "|",
                     i2.d1, i2.d2, i2.d3,
                     i2.d4, i2.d5, i2.d6,
                     i2.d7, i2.d8, i2.d9,
                    i2.d10,i2.d11,i2.d12);

   const std::size_t dblist_size = 30;

   std::vector<data_block> db_list(dblist_size);
   std::vector<std::string> s_list;

   std::size_t total_data_length = 0;
   for (std::size_t i = 0; i < (dblist_size >> 1); ++i)
   {
      s_list.push_back(str1);
      s_list.push_back(str2);
      total_data_length += (str1.size() + str2.size());
   }

   const std::size_t rounds = 1000000;
   timer t;
   t.start();
   for (std::size_t i = 0; i < rounds; ++i)
   {
      for (std::size_t j = 0; j < s_list.size(); ++j)
      {
         strtk::parse(s_list[j],
                      "|",
                       db_list[j].d1,  db_list[j].d2,  db_list[j].d3,
                       db_list[j].d4,  db_list[j].d5,  db_list[j].d6,
                       db_list[j].d7,  db_list[j].d8,  db_list[j].d9,
                       db_list[j].d10, db_list[j].d11, db_list[j].d12);
      }
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4fprs/s %5.2fMB/s\n",
          static_cast<unsigned long long>(s_list.size() * 12 * rounds),
          t.time(),
          (s_list.size() * rounds) / t.time(),
          (total_data_length * rounds) / (1048576.0 * t.time()));
}

void parse_test02()
{
   static const std::string data = "-1140639 -1352246 -1364216  -139556 -1508745 -1613887 -1785865  -189043 -2587409  -277311 "
                                   "+5949527  6178253 +6183204  6716479 +6853831  6881008  +698539   712081 +7641583  8362997 "
                                   "-2803774 -2990674 -3101408 -3193780 -3284812 -3288516  -360094 -4283200 -4857366  -487077 "
                                   "+4475723  4481602 +4739021  4971375 +4983229   549060 +5652473  5718381 +5807982  5905582 "
                                   "-5098362 -5481616 -5685238 -6024753 -6327104 -6435002 -6616233 -6790734  -682977 -7103068 "
                                   "+1725389  1739907 +1790328  2283111 +2633376  3081627  +327784  3329177 +3413213   359663 "
                                   "-7162254 -7166378 -7257340 -7563199 -7762135 -7823310 -7929750 -8195674 -8283146 -8502597 "
                                   "-8641026 -8995586 -9290949 -9408126 -9446630 -9744076 -9937408  1081438 +1266350  1571976 "
                                   " +369796  3767047 +3779257  3818495 +3978233  3985261 +4170549  4261764 +4283197  4444979 "
                                   "+8376780   840695 +8444607  9219961 +9482187  9517369 +9521169  9523796 +9634928  9785825";

   std::vector<int> vec_int;
   vec_int.reserve(100);

   std::cout << strtk::text::left_align(title_length(),' ',"[integer into " + strtk::type_name(vec_int) + "]");

   timer t;
   t.start();
   static const std::size_t rounds = 1000000;
   for (std::size_t i = 0; i < rounds; ++i)
   {
      vec_int.clear();
      strtk::parse(data," ",vec_int);
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4fprs/s %5.2fMB/s\n",
          static_cast<unsigned long long>(vec_int.size() * rounds),
          t.time(),
          (vec_int.size() * rounds) / t.time(),
          (data.size() * rounds) / (1048576.0 * t.time()));
}

void parse_test03()
{
   static const std::string data = "-1140639.1140639 -1352246.1352246 -1364216.1352246 -1395560.1395560 -1508745.1508745 "
                                   "+5949527.5949527  6178253.6178253 +6183204.6178253  6716479.6716479 +6853831.6853831 "
                                   "-2803774.2803774 -2990674.2990674 -3101408.2990674 -3193780.3193780 -3284812.3284812 "
                                   "+4475723.4475723  4481602.4481602 +4739021.4481602  4971375.4971375 +4983229.4983229 "
                                   "-5098362.5098362 -5481616.5481616 -5685238.5481616 -6024753.6024753 -6327104.6327104 "
                                   "+1725389.1725389  1739907.1739907 +1790328.1739907  2283111.2283111 +2633376.2633376 "
                                   "-7162254.7162254 -7166378.7166378 -7257340.7166378 -7563199.7563199 -7762135.7762135 "
                                   "-8641026.8641026 -8995586.8995586 -9290949.8995586 -9408126.9408126 -9446630.9446630 "
                                   " +369796.+369796  3767047.3767047 +3779257.3767047  3818495.3818495 +3978233.3978233 "
                                   "+8376780.8376780  0840695.8406950 +8444607.8406950  9219961.9219961 +9482187.9482187";

   std::vector<double> vec_double;
   vec_double.reserve(50);

   std::cout << strtk::text::left_align(title_length(),' ',"[double into " + strtk::type_name(vec_double) + "]");

   timer t;
   t.start();
   static const std::size_t rounds = 1000000;
   for (std::size_t i = 0; i < rounds; ++i)
   {
      vec_double.clear();
      strtk::parse(data," ",vec_double);
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4fprs/s %5.2fMB/s\n",
          static_cast<unsigned long long>(vec_double.size() * rounds),
          t.time(),
          (vec_double.size() * rounds) / t.time(),
          (data.size() * rounds) / (1048576.0 * t.time()));
}

static const std::string md_base = "a+bc=def ghij-klmno?pqrstu&vwxyzAB@CDEFGHIJ~KLMNOPQRS#TUVWXYZ012|3456789abcd|efghijklmnopqrsdu!";
static const std::size_t md_replicate_count = 1000000;
static const std::string md_delimiters = "-+=~&*[]{}()<>|!?@^%$#\".,;:_ /\\\t\r\n";
static const std::size_t md_rounds = 10;

void raw_tokenizer_md_speed_test()
{
   std::cout << strtk::text::left_align(title_length(),' ',"[tokenizer(md) raw speed test]");

   std::string s;
   s.reserve(md_base.size() * md_replicate_count);
   strtk::replicate(md_replicate_count,md_base,s);
   s.resize(s.size() - 1);
   strtk::multiple_char_delimiter_predicate predicate(md_delimiters);
   typedef strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer_type;
   tokenizer_type tokenizer(s,predicate);
   std::size_t token_count = 0;
   timer t;
   t.start();
   for (std::size_t i = 0; i < md_rounds; ++i)
   {
      tokenizer.assign(s);
      tokenizer_type::iterator itr = tokenizer.begin();
      tokenizer_type::const_iterator end = tokenizer.end();
      while (end != itr)
      {
         ++token_count;
         ++itr;
      }
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4ftks/s %5.2fMB/s\n",
          static_cast<unsigned long long>(token_count),
          t.time(),
          token_count / t.time(),
          (s.size() * md_rounds) / (1048576.0 * t.time()));
}

void raw_split_md_speed_test()
{
   std::cout << strtk::text::left_align(title_length(),' ',"[split(md) raw speed test]");
   std::string s;
   s.reserve(md_base.size() * md_replicate_count);
   strtk::replicate(md_replicate_count,md_base,s);
   s.resize(s.size() - 1);
   strtk::multiple_char_delimiter_predicate predicate(md_delimiters);
   std::size_t token_count = 0;
   timer t;
   t.start();
   for (std::size_t i = 0; i < md_rounds; ++i)
   {
      strtk::split(predicate,
                   s,
                   strtk::counting_back_inserter<strtk::std_string::iterator_type>(token_count));
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4ftks/s %5.2fMB/s\n",
          static_cast<unsigned long long>(token_count),
          t.time(),
          token_count / t.time(),
          (s.size() * md_rounds) / (1048576.0 * t.time()));
}

static const std::string sd_base = "a|bc|def|ghij|klmno|pqrstu|vwxyzAB|CDEFGHIJ|KLMNOPQRS|TUVWXYZ012|3456789!@#$|^&*(){}[]<>?:;,~|";
static const std::size_t sd_replicate_count = 1000000;
static const std::size_t sd_rounds = 10;

void raw_tokenizer_sd_speed_test()
{
   std::cout << strtk::text::left_align(title_length(),' ',"[tokenizer(sd) raw speed test]");
   std::string s;
   s.reserve(sd_base.size() * sd_replicate_count);
   strtk::replicate(sd_replicate_count,sd_base,s);
   s.resize(s.size() - 1);
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   typedef strtk::std_string::tokenizer<>::type tokenizer_type;
   tokenizer_type tokenizer(s,predicate);
   std::size_t token_count = 0;
   timer t;
   t.start();
   for (std::size_t i = 0; i < sd_rounds; ++i)
   {
      tokenizer.assign(s);
      tokenizer_type::iterator itr = tokenizer.begin();
      tokenizer_type::const_iterator end = tokenizer.end();
      while (end != itr)
      {
         ++token_count;
         ++itr;
      }
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4ftks/s %5.2fMB/s\n",
          static_cast<unsigned long long>(token_count),
          t.time(),
          token_count / t.time(),
          (s.size() * sd_rounds) / (1048576.0 * t.time()));
}

void raw_split_sd_speed_test()
{
   std::cout << strtk::text::left_align(title_length(),' ',"[split(sd) raw speed test]");
   std::string s;
   s.reserve(sd_base.size() * sd_replicate_count);
   strtk::replicate(sd_replicate_count,sd_base,s);
   s.resize(s.size() - 1);
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   std::size_t token_count = 0;
   timer t;
   t.start();
   for (std::size_t i = 0; i < sd_rounds; ++i)
   {
      strtk::split(predicate,
                   s,
                   strtk::counting_back_inserter<strtk::std_string::iterator_type>(token_count));
   }
   t.stop();
   printf("Token Count:%10llu  Total time:%8.4f  Rate:%14.4ftks/s %5.2fMB/s\n",
          static_cast<unsigned long long>(token_count),
          t.time(),
          token_count / t.time(),
          (s.size() * sd_rounds) / (1048576.0 * t.time()));
}

int main()
{
   parse_test01();
   parse_test02();
   parse_test03();
   raw_tokenizer_sd_speed_test();
   raw_tokenizer_md_speed_test();
   raw_split_sd_speed_test();
   raw_split_md_speed_test();
   return 0;
}