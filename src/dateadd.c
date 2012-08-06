/* Copyright (C) 2009 by Alan Falloon
   
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
#include "parse-datetime.h"
#include "timespec.h"
#include "fprintftime.h"

#define PROGRAM_NAME "dateadd"

const char * argp_program_version =
  PROGRAM_NAME " (" PACKAGE_NAME ") " VERSION
  " Copyright (c) 2009 Alan Falloon\n"
  "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
  "This is free software: you are free to change and redistribute it.\n"
  "There is NO WARRANTY, to the extent permitted by law.";
const char * argp_program_bug_address = PACKAGE_BUGREPORT;

static const struct argp_option options[] = {
  {.name="format", .key='f', .arg="FORMAT", .flags=0,
   .doc=
   "The output format of the resulting date and time. Format sequences are"
   " identical to the date(1) command."},
  {0}
};
static error_t arg_parser(int KEY, char *ARG, struct argp_state *STATE);
static const struct argp argp = {
  .options     = options,
  .parser      = arg_parser,
  .args_doc    = "DATE",
  .doc         =  
  "Print the date after a sequence of relative date changes.\v"
  "When called with a series of date strings, usually relative dates"
  " (ex. \"+1 month\") this program applies each relative date in order"
  " and prints the final date.\n\nAbsolute dates, of course, reset the date"
  " and are usually only used as the first argument. The default is the"
  " current date and time, if no absolute dates are specified.\n\nYou can"
  " use any date format recognised by the date(1) command.",
  .children    = NULL,
  .help_filter = NULL,
  .argp_domain = 0,
};

struct state {
  struct timespec date;
  char*           fmt;
};

int main(int argc, char *argv[])
{
  struct state s;
  int index;
  struct tm *tm;
  gettime(&s.date);
  s.fmt = "%a, %d %b %Y %H:%M:%S %z";
  argp_parse(&argp,argc,argv,0,&index,&s);
  tm = localtime(&s.date.tv_sec);
  fprintftime(stdout,s.fmt,tm,0,s.date.tv_nsec);
  fprintf(stdout,"\n");
  return 0;
}

static error_t arg_parser(int key, char *arg, struct argp_state *argp_state)
{
  int r;
  struct timespec new_date;
  struct state *state = argp_state->input;
  switch (key)
  {
  case 'f':
    state->fmt = arg;
    return 0;
  case ARGP_KEY_ARG:
    r = parse_datetime(&new_date, arg, &state->date);
    if (!r) {
      argp_error(argp_state, "Unrecognized date format \"%s\"", arg);
      return EINVAL;
    }
    state->date = new_date;
    return 0;
  default:
    return ARGP_ERR_UNKNOWN;
  }
}
