/**
 * @file     NAN.cpp
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     06/03/2013 04:06:49 PM
 *
 */


#include <cmath>
#include <cstdio>
#include <limits>

int main()
{
  double nan = std::numeric_limits<double>::signaling_NaN();

  if (isnan(nan))
    printf("isnan\n");

  if (std::numeric_limits<double>::has_signaling_NaN)
    printf("has NAN\n");

}
