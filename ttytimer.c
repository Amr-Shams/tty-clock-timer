
#include "ttyclock.h"
void update_timer(void)
{
     int ihour;
     char tmpstr[128];

     ttyclock.lt = time(NULL);
     ttyclock.tm = localtime(&(ttyclock.lt));
     if (ttyclock.option.utc)
     {
          ttyclock.tm = gmtime(&(ttyclock.lt));
     }

     // Calculate elapsed time since initialization
     time_t diff = difftime(time(NULL), ttyclock.init_lt);
     int hours = (int)(diff / 3600);
     int minutes = (int)((diff % 3600) / 60);
     int seconds = (int)(diff % 60);

     ihour = format_hours(hours);
     /* Set hour using elapsed time */
     ttyclock.date.hour[0] = ihour / 10;
     ttyclock.date.hour[1] = ihour % 10;

     /* Set minutes using elapsed time */
     ttyclock.date.minute[0] = minutes / 10;
     ttyclock.date.minute[1] = minutes % 10;

     /* Set date string */
     strcpy(ttyclock.date.old_datestr, ttyclock.date.datestr);
     strftime(tmpstr,
              sizeof(tmpstr),
              ttyclock.option.format,
              ttyclock.tm);
     sprintf(ttyclock.date.datestr, "%s%s", tmpstr, ttyclock.meridiem);

     /* Set seconds using elapsed time */
     ttyclock.date.second[0] = seconds / 10;
     ttyclock.date.second[1] = seconds % 10;

     return;
}
void

reset_timer(void)
{
     ttyclock.init_lt = time(NULL);
}
void 
toggle_pause(void){
    ttyclock.pause = !ttyclock.pause;
}
void
key_event(void)
{
     int i, c;

     struct timespec length = { ttyclock.option.delay, ttyclock.option.nsdelay };
     
     fd_set rfds;
     FD_ZERO(&rfds);
     FD_SET(STDIN_FILENO, &rfds);

     if (ttyclock.option.screensaver)
     {
          c = wgetch(stdscr);
          if(c != ERR && ttyclock.option.noquit == false)
          {
               ttyclock.running = false;
          }
          else
          {
               nanosleep(&length, NULL);
               for(i = 0; i < 8; ++i)
                    if(c == (i + '0'))
                    {
                         ttyclock.option.color = i;
                         init_pair(1, ttyclock.bg, i);
                         init_pair(2, i, ttyclock.bg);
                    }
          }
          return;
     }


     switch(c = wgetch(stdscr))
     {
     case KEY_RESIZE:
          endwin();
          init();
          break;

     case KEY_UP:
     case 'k':
     case 'K':
          if(ttyclock.geo.x >= 1
             && !ttyclock.option.center)
               clock_move(ttyclock.geo.x - 1, ttyclock.geo.y, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case KEY_DOWN:
     case 'j':
     case 'J':
          if(ttyclock.geo.x <= (LINES - ttyclock.geo.h - DATEWINH)
             && !ttyclock.option.center)
               clock_move(ttyclock.geo.x + 1, ttyclock.geo.y, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case KEY_LEFT:
     case 'h':
     case 'H':
          if(ttyclock.geo.y >= 1
             && !ttyclock.option.center)
               clock_move(ttyclock.geo.x, ttyclock.geo.y - 1, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case KEY_RIGHT:
     case 'l':
     case 'L':
          if(ttyclock.geo.y <= (COLS - ttyclock.geo.w - 1)
             && !ttyclock.option.center)
               clock_move(ttyclock.geo.x, ttyclock.geo.y + 1, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case 'q':
     case 'Q':
          if (ttyclock.option.noquit == false)
               ttyclock.running = false;
          break;

     case 's':
     case 'S':
          set_second();
          break;
     case 'c':
     case 'C':
          set_center(!ttyclock.option.center);
          break;

     case 'b':
     case 'B':
          ttyclock.option.bold = !ttyclock.option.bold;
          break;

     case 'r':
     case 'R':
          reset_timer();
          break;

     case 'x':
     case 'X':
          set_box(!ttyclock.option.box);
          break;
    case 'p': 
    case 'P': 
          toggle_pause(); 
          break;
     case '0': case '1': case '2': case '3':
     case '4': case '5': case '6': case '7':
          i = c - '0';
          ttyclock.option.color = i;
          init_pair(1, ttyclock.bg, i);
          init_pair(2, i, ttyclock.bg);
          break;

     default:
          pselect(1, &rfds, NULL, NULL, &length, NULL);
     }

     return;
}


int main(int argc, char **argv)
{
     int c;

     /* Alloc ttyclock */
     memset(&ttyclock, 0, sizeof(ttyclock_t));

     ttyclock.option.date = true;

     /* Default date format */
     strncpy(ttyclock.option.format, "%F", sizeof(ttyclock.option.format));
     /* Default color */
     ttyclock.option.color = COLOR_GREEN; /* COLOR_GREEN = 2 */
     /* Default delay */
     ttyclock.option.delay = 1;   /* 1FPS */
     ttyclock.option.nsdelay = 0; /* -0FPS */
     ttyclock.option.blink = false;

     atexit(cleanup);

     while ((c = getopt(argc, argv, "iuvsScbtrhBxnDC:f:d:T:a:")) != -1)
     {
          switch (c)
          {
          case 'h':
          default:
               printf("usage : tty-timer [-iuvsScbtrahDBxn] [-C [0-7]] [-f format] [-d delay] [-a nsdelay] [-T tty] \n"
                      "    -s            Toggle seconds display                         \n"
                      "    -S            Screensaver mode                               \n"
                      "    -x            Show box                                       \n"
                      "    -c            Set the timer at the center of the terminal   \n"
                      "    -C [0-7]      Set the timer color                           \n"
                      "    -b            Use bold colors                                \n"
                      "    -u            Use UTC time                                   \n"
                      "    -T tty        Display the timer on the specified terminal   \n"
                      "    -r            Reset timer (R key during runtime)            \n"
                      "    -f format     Set the date format                            \n"
                      "    -n            Don't quit on keypress                         \n"
                      "    -v            Show tty-timer version                         \n"
                      "    -i            Show some info about tty-timer                 \n"
                      "    -h            Show this page                                 \n"
                      "    -D            Hide date                                      \n"
                      "    -B            Enable blinking colon                          \n"
                      "    -d delay      Set the delay between redraws. Default 1s.    \n"
                      "    -a nsdelay    Additional delay in nanoseconds. Default 0ns. \n"
                      "\n");
               exit(EXIT_SUCCESS);
               break;
          case 'i':
               puts("TTY-Timer (based on TTY-Clock) - A simple terminal timer");
               exit(EXIT_SUCCESS);
               break;
          case 'u':
               ttyclock.option.utc = true;
               break;
          case 'v':
               puts("TTY-Timer v1.0 - Terminal Timer");
               exit(EXIT_SUCCESS);
               break;
          case 's':
               ttyclock.option.second = !ttyclock.option.second; // Toggle seconds
               break;
          case 'S':
               ttyclock.option.screensaver = true;
               break;
          case 'c':
               ttyclock.option.center = true;
               break;
          case 'b':
               ttyclock.option.bold = true;
               break;
          case 'C':
               if (atoi(optarg) >= 0 && atoi(optarg) < 8)
                    ttyclock.option.color = atoi(optarg);
               break;
          case 'r':
               reset_timer();
               break;
          case 'f':
               strncpy(ttyclock.option.format, optarg, 100);
               break;
          case 'd':
               if (atol(optarg) >= 0 && atol(optarg) < 100)
                    ttyclock.option.delay = atol(optarg);
               break;
          case 'D':
               ttyclock.option.date = false;
               break;
          case 'B':
               ttyclock.option.blink = true;
               break;
          case 'a':
               if (atol(optarg) >= 0 && atol(optarg) < 1000000000)
                    ttyclock.option.nsdelay = atol(optarg);
               break;
          case 'x':
               ttyclock.option.box = true;
               break;
          case 'T':
          {
               struct stat sbuf;
               if (stat(optarg, &sbuf) == -1)
               {
                    fprintf(stderr, "tty-timer: error: couldn't stat '%s': %s.\n",
                            optarg, strerror(errno));
                    exit(EXIT_FAILURE);
               }
               else if (!S_ISCHR(sbuf.st_mode))
               {
                    fprintf(stderr, "tty-timer: error: '%s' doesn't appear to be a character device.\n",
                            optarg);
                    exit(EXIT_FAILURE);
               }
               else
               {
                    free(ttyclock.tty);
                    ttyclock.tty = strdup(optarg);
               }
          }
          break;
          case 'n':
               ttyclock.option.noquit = true;
               break;
          }
     }

     init();
     attron(A_BLINK);
     while (ttyclock.running)
     {
                 key_event();

       if(ttyclock.pause == true)
         continue;
          clock_rebound();
          update_timer();
          draw_clock();
     }

     endwin();
     printf("Timer stopped.\n");

     return 0;
}
