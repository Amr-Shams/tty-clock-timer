/*
 *      TTY-CLOCK headers file.
 *      Copyright © 2009-2013 tty-clock contributors
 *      Copyright © 2008-2009 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the  nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TTYCLOCK_H_INCLUDED
#define TTYCLOCK_H_INCLUDED

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

/* Macro */
#define NORMFRAMEW 35
#define SECFRAMEW  54
#define DATEWINH   3
#define AMSIGN     " [AM]"
#define PMSIGN     " [PM]"

/* Global ttyclock struct */
typedef struct
{
     /* while() boolean */
     bool running;
     bool pause; 

     /* terminal variables */
     SCREEN *ttyscr;
     char *tty;
     int bg;

     /* Running option */
     struct
     {
          bool second;
          bool screensaver;
          bool twelve;
          bool center;
          bool rebound;
          bool date;
          bool utc;
          bool box;
          bool noquit;
          char format[100];
          int color;
          bool bold;
          long delay;
          bool blink;
          long nsdelay;
     } option;

     /* Clock geometry */
     struct
     {
          int x, y, w, h;
          /* For rebound use (see clock_rebound())*/
          int a, b;
     } geo;

     /* Date content ([2] = number by number) */
     struct
     {
          unsigned int hour[2];
          unsigned int minute[2];
          unsigned int second[2];
          char datestr[256];
          char old_datestr[256];
     } date;


     struct { 
     unsigned int total_sessions; 
     unsigned int current_streak; 
     unsigned int best_streak; 
     /* Total time across all sessions */  
     time_t total_time;  
     /* Current session time */ 
     time_t session_time; 
     /* longest session of all the time */ 
     time_t best_session; 
/* Toggle the session */ 
     bool show_stats; 
     } stats; 

     /* time.h utils */
     struct tm *tm;
     time_t lt;

     /* init time */ 
     time_t init_lt;

     /* Clock member */
     char *meridiem;
     WINDOW *framewin;
     WINDOW *datewin;
     WINDOW *statswin; 

} ttyclock_t;

/* Prototypes */
void init(void);
void signal_handler(int signal);
void draw_number(int n, int x, int y);
void draw_clock(void);
void clock_move(int x, int y, int w, int h);
void set_second(void);
void set_center(bool b);
void set_box(bool b);
void key_event(void);
void cleanup(void); 
void clock_rebound(void);
int format_hours(int);
void reset_timer(void); 

/* Stats functions */ 
void init_stats(void);
void update_stats(void);
void draw_stats(void);
void save_stats(void);
void load_stats(void);
void toggle_stats(void);
void reset_stats(void);
/* Global variable */
ttyclock_t ttyclock;

/* Number matrix */
extern const bool number[][15]; 

#endif /* TTYCLOCK_H_INCLUDED */

// vim: expandtab tabstop=5 softtabstop=5 shiftwidth=5
