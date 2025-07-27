
#include "ttyclock.h"
time_t pause_time;
void 
init_stats(void) { 
  memset(&ttyclock.stats, 0, sizeof(ttyclock.stats)); 
  ttyclock.stats.show_stats = false; 
  /* Load stats from the file */ 
  load_stats();
  /* Create the stats window on the top corner  */ 
  int stats_w=30; 
  int stats_h=12; 
  int stats_x=1; 
  int stats_y=COLS-stats_w-1;
  if(stats_y<0) stats_y=0; 
  if(stats_x + stats_h > LINES) stats_x=LINES-stats_h-1;
    ttyclock.statswin = newwin(stats_h, stats_w,
                                stats_x, stats_y);

  if(ttyclock.option.box && ttyclock.stats.show_stats)
    box(ttyclock.statswin, 0, 0); 
}
void 
update_stats(void) {
  if(!ttyclock.pause){ 
  ttyclock.stats.session_time = difftime(time(NULL), ttyclock.init_lt); 
  if(ttyclock.stats.session_time > ttyclock.stats.best_session) {
    ttyclock.stats.best_session= ttyclock.stats.session_time;
    }
  }
}
void 
// TODO: update it to the advanced version like the clock 
draw_stats(void) { 
  if(!ttyclock.stats.show_stats){ return ;}
  
    werase(ttyclock.statswin);
    
    if (ttyclock.option.box) {
        box(ttyclock.statswin, 0, 0);
    }
    
    if (ttyclock.option.bold) {
        wattron(ttyclock.statswin, A_BOLD);
    }
    
    wbkgdset(ttyclock.statswin, COLOR_PAIR(2));
    
    // Display stats
    mvwprintw(ttyclock.statswin, 1, 2, "TIMER STATISTICS");
    mvwprintw(ttyclock.statswin, 3, 2, "Sessions: %u", ttyclock.stats.total_sessions);
    mvwprintw(ttyclock.statswin, 4, 2, "Current Streak: %u", ttyclock.stats.current_streak);
    mvwprintw(ttyclock.statswin, 5, 2, "Best Streak: %u", ttyclock.stats.best_streak);
    
    // Format times
    int hours, minutes, seconds;
    
    // Current session
    hours = (int)(ttyclock.stats.session_time / 3600);
    minutes = (int)((ttyclock.stats.session_time % 3600) / 60);
    seconds = (int)(ttyclock.stats.session_time % 60);
    mvwprintw(ttyclock.statswin, 6, 2, "Session: %02d:%02d:%02d", hours, minutes, seconds);
    
    // Best session
    hours = (int)(ttyclock.stats.best_session / 3600);
    minutes = (int)((ttyclock.stats.best_session % 3600) / 60);
    seconds = (int)(ttyclock.stats.best_session % 60);
    mvwprintw(ttyclock.statswin, 7, 2, "Best: %02d:%02d:%02d", hours, minutes, seconds);
    
    // Total time
    hours = (int)(ttyclock.stats.total_time / 3600);
    minutes = (int)((ttyclock.stats.total_time % 3600) / 60);
    mvwprintw(ttyclock.statswin, 8, 2, "Total: %02d:%02d", hours, minutes);
    
    // Status
    mvwprintw(ttyclock.statswin, 10, 2, "Status: %s", 
              ttyclock.pause ? "PAUSED" : "RUNNING");
    
    wrefresh(ttyclock.statswin);
}
void toggle_stats(void)
{
    ttyclock.stats.show_stats = !ttyclock.stats.show_stats;
    
    if (ttyclock.stats.show_stats) {
        // Show stats window
        if (ttyclock.option.box) {
            box(ttyclock.statswin, 0, 0);
        }
        wrefresh(ttyclock.statswin);
    } else {
        // Hide stats window
        werase(ttyclock.statswin);
        wrefresh(ttyclock.statswin);
    }
}
void 
save_stats(void) { 
  FILE *file = fopen(".ttytimer_stats", "w");
  if(!file)return ; 
  fprintf(file, "%u\n%u\n%u\n%ld\n%ld\n%ld\n", 
                ttyclock.stats.total_sessions,
                ttyclock.stats.current_streak,
                ttyclock.stats.best_streak,
                ttyclock.stats.total_time,
                ttyclock.stats.session_time,
                ttyclock.stats.best_session);
        fclose(file);
}
void load_stats(void)
{
    FILE *file = fopen(".ttytimer_stats", "r");
    if (file) {
        fscanf(file, "%u\n%u\n%u\n%ld\n%ld\n%ld\n",
               &ttyclock.stats.total_sessions,
               &ttyclock.stats.current_streak,
               &ttyclock.stats.best_streak,
               &ttyclock.stats.total_time,
               &ttyclock.stats.session_time,
               &ttyclock.stats.best_session);
        fclose(file);
    }
}
void  
reset_stats(void) { 
  ttyclock.stats.total_sessions++; 
  ttyclock.stats.total_time += ttyclock.stats.session_time; 
  ttyclock.stats.current_streak++; 
  if (ttyclock.stats.current_streak > ttyclock.stats.best_streak) { 
    ttyclock.stats.best_streak = ttyclock.stats.current_streak; 
  } 
  save_stats(); 
  reset_timer(); 
  ttyclock.stats.session_time = 0; 
}
void
update_timer(void)
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
toggle_pause(void)
{
     time_t now = time(NULL);

     if (!ttyclock.pause)
     {
          pause_time = now;
     }
     else
     {
          time_t paused_duration = now - pause_time;
          ttyclock.init_lt += paused_duration;
     }

     ttyclock.pause = !ttyclock.pause;
}
void
key_event(void)
{
     int i, c;

     struct timespec length = {ttyclock.option.delay, ttyclock.option.nsdelay};

     fd_set rfds;
     FD_ZERO(&rfds);
     FD_SET(STDIN_FILENO, &rfds);

     if (ttyclock.option.screensaver)
     {
          c = wgetch(stdscr);
          if (c != ERR && ttyclock.option.noquit == false)
          {
               ttyclock.running = false;
          }
          else
          {
               nanosleep(&length, NULL);
               for (i = 0; i < 8; ++i)
                    if (c == (i + '0'))
                    {
                         ttyclock.option.color = i;
                         init_pair(1, ttyclock.bg, i);
                         init_pair(2, i, ttyclock.bg);
                    }
          }
          return;
     }

     switch (c = wgetch(stdscr))
     {
     case KEY_RESIZE:
          endwin();
          init();
          break;

     case KEY_UP:
     case 'k':
     case 'K':
          if (ttyclock.geo.x >= 1 && !ttyclock.option.center)
               clock_move(ttyclock.geo.x - 1, ttyclock.geo.y, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case KEY_DOWN:
     case 'j':
     case 'J':
          if (ttyclock.geo.x <= (LINES - ttyclock.geo.h - DATEWINH) && !ttyclock.option.center)
               clock_move(ttyclock.geo.x + 1, ttyclock.geo.y, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case KEY_LEFT:
     case 'h':
     case 'H':
          if (ttyclock.geo.y >= 1 && !ttyclock.option.center)
               clock_move(ttyclock.geo.x, ttyclock.geo.y - 1, ttyclock.geo.w, ttyclock.geo.h);
          break;

     case KEY_RIGHT:
     case 'l':
     case 'L':
          if (ttyclock.geo.y <= (COLS - ttyclock.geo.w - 1) && !ttyclock.option.center)
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
    case 'w': 
    case 'W': 
          toggle_stats(); 
          break; 
    case 'z': 
    case 'Z': 
          reset_stats(); 
          break; 

     case '0':
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
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

int
main(int argc, char **argv)
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
     init_stats();
     attron(A_BLINK);
     while (ttyclock.running)
     {
          clock_rebound();
          if (ttyclock.pause == true)
          {
               key_event();
               continue;
          }
          update_timer();
          update_stats(); 
          draw_clock();
          draw_stats(); 
          key_event();
     }
     save_stats();

     endwin();
     printf("Timer stopped.\n");

     return 0;
}
