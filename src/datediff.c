/* datediff - display differences in time in human readable way

   Copyright (C) 2009 by Alan Falloon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>
#include <argp.h>
#include <stdlib.h>
#include <stddef.h>
#include "getdate.h"
#include "timespec.h"

#define PROGRAM_NAME "datediff"

const char * argp_program_version =
  PROGRAM_NAME " (" PACKAGE_NAME ") " VERSION
  " Copyright (c) 2009 Alan Falloon\n"
  "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
  "This is free software: you are free to change and redistribute it.\n"
  "There is NO WARRANTY, to the extent permitted by law.";
const char * argp_program_bug_address = PACKAGE_BUGREPORT;

static const struct argp_option options[] = {
  {0,0,0,0,0,0}
};
static error_t arg_parser(int KEY, char *ARG, struct argp_state *STATE);
static const struct argp argp = {
  .options     = options,
  .parser      = arg_parser,
  .args_doc    = "START_DATE END_DATE",
  .doc         =
  "Print the difference between two dates in human terms (like \"1 month 2 weeks\").\v"
  "When called called with two dates, it prints thier difference.\n\nYou can"
  " use any date format recognised by the date(1) command.",
  .children    = NULL,
  .help_filter = NULL,
  .argp_domain = 0,
};

struct state {
  /* These must be in this order so that we can treat them as an array
     of 2 struct timespec values in arg_parser.*/
  struct timespec start_date, end_date;
};

enum time_units {
  year  ,
  month ,
  day   ,
  hour  ,
  minute,
  second,
  num_units
};

struct time_unit_name {
  const char* singular;
  const char* plural;
};

typedef int time_diff_t[num_units];

static void diff_time(const struct timespec *start,
                      const struct timespec *end,
                      time_diff_t result);

int main(int argc, char *argv[])
{
  struct state s;
  int i, first;
  struct time_unit_name time_unit_names[num_units] = {
    {"year"  , "years"  },
    {"month" , "months" },
    {"day"   , "days"   },
    {"hour"  , "hours"  },
    {"minute", "minutes"},
    {"second", "seconds"}
  };
  time_diff_t time_diff;

  argp_parse(&argp,argc,argv,0,&i,&s);

  diff_time(&s.start_date, &s.end_date, time_diff);

  for (first = 1, i = 0; i < num_units; ++i) {
    if (0 == time_diff[i]) continue;
    if (first) first=0;
    else       printf(" ");
    switch (time_diff[i]) {
    case 1:
    case -1:
      printf("%d %s", time_diff[i], time_unit_names[i].singular);
      break;
    default:
      printf("%d %s", time_diff[i], time_unit_names[i].plural);
      break;
    }
  }

  if (!first) printf("\n");
  return 0;
}

static const size_t tm_offset[num_units] = {
  offsetof(struct tm, tm_year),
  offsetof(struct tm, tm_mon ),
  offsetof(struct tm, tm_mday),
  offsetof(struct tm, tm_hour),
  offsetof(struct tm, tm_min ),
  offsetof(struct tm, tm_sec )
};
static int tm_unit(struct tm *t, size_t off)
{
  return *(int*)(((char*)t) + off);
}
static void diff_time(const struct timespec *start,
                      const struct timespec *end,
                      time_diff_t res)
{
  int i;
  struct tm start_tm, end_tm;
  gmtime_r(&start->tv_sec, &start_tm);
  gmtime_r(&end  ->tv_sec, &end_tm  );
  for( i = 0; i < num_units; ++i ) {
    const int start = tm_unit(&start_tm, tm_offset[i]);
    const int end   = tm_unit(&end_tm  , tm_offset[i]);
    res[i] = end-start;
  }
}

static error_t arg_parser(int key, char *arg, struct argp_state *argp_state)
{
  int r, i;
  char **argv;
  struct state *state = argp_state->input;
  struct timespec * const times = &state->start_date;
  struct timespec now;
  switch (key)
  {
  case ARGP_KEY_ARGS:
    if (argp_state->argc - argp_state->next != 2) {
      argp_error(argp_state, "Expecting exactly two dates, but got %d",
                 argp_state->argc - argp_state->next);
      return EINVAL;
    }
    argv = argp_state->argv + argp_state->next;
    argp_state->next += 2;
    gettime(&now);
    for (i = 0; i < 2; ++i) {
      r = get_date(&times[i], argv[i], &now);
      if (!r) {
        argp_error(argp_state, "Unrecognized date format \"%s\"", argv[i]);
        return EINVAL;
      }
    }
    if ( state->end_date.tv_sec  <  state->start_date.tv_sec ||
        (state->end_date.tv_sec  == state->start_date.tv_sec &&
         state->end_date.tv_nsec <  state->start_date.tv_nsec)) {
      argp_error(argp_state, "End date is before start date");
      return EINVAL;
    }
    return 0;
  default:
    return ARGP_ERR_UNKNOWN;
  }
}
