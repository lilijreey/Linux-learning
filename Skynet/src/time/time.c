#include <assert.h>

#include <glib.h>

#include "time.h"

time_t mk_integral_tm_day(struct tm tm_cur, int mday, int month)
{
	assert( (mday > 0) && (mday < 32) );

	tm_cur.tm_hour = 0;
	tm_cur.tm_min  = 0;
	tm_cur.tm_sec  = 0;

	GDate* gdate1 = g_date_new_dmy(tm_cur.tm_mday, tm_cur.tm_mon + 1, tm_cur.tm_year);
	GDate* gdate2 = g_date_new_dmy(mday, tm_cur.tm_mon + 1, tm_cur.tm_year);

	if (month > 0) {
		g_date_add_months(gdate2, month);
	} else if (month < 0) {
		g_date_subtract_months(gdate2, -month);
	}

	int days = g_date_days_between(gdate1, gdate2);

	g_date_free(gdate1);
	g_date_free(gdate2);
	return mktime(&tm_cur) + days * 86400;
}
