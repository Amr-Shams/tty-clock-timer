# TTY-Clock and TTY-Timer MakeFile
# Under BSD License
# See ttyclock.c for the license detail.

# Source files
COMMON_SRC = common.c
CLOCK_SRC = ttyclock.c
TIMER_SRC = ttytimer.c

# Compiler and tools
CC ?= gcc
PKG_CONFIG ?= pkg-config

# Binary names
CLOCK_BIN = tty-clock
TIMER_BIN = tty-timer

# Installation paths
PREFIX ?= /usr/local
INSTALLPATH ?= ${DESTDIR}${PREFIX}/bin
MANPATH ?= ${DESTDIR}${PREFIX}/share/man/man1

# Auto-detect ncurses configuration
ifeq ($(shell sh -c 'which ncurses6-config>/dev/null 2>/dev/null && echo y'), y)
	CFLAGS += -Wall -g $$(ncurses6-config --cflags)
	LDFLAGS += $$(ncurses6-config --libs)
else ifeq ($(shell sh -c 'which ncursesw6-config>/dev/null 2>/dev/null && echo y'), y)
	CFLAGS += -Wall -g $$(ncursesw6-config --cflags)
	LDFLAGS += $$(ncursesw6-config --libs)
else ifeq ($(shell sh -c 'which ncurses5-config>/dev/null 2>/dev/null && echo y'), y)
	CFLAGS += -Wall -g $$(ncurses5-config --cflags)
	LDFLAGS += $$(ncurses5-config --libs)
else ifeq ($(shell sh -c 'which ncursesw5-config>/dev/null 2>/dev/null && echo y'), y)
	CFLAGS += -Wall -g $$(ncursesw5-config --cflags)
	LDFLAGS += $$(ncursesw5-config --libs)
else
	CFLAGS += -Wall -g $$($(PKG_CONFIG) --cflags ncurses)
	LDFLAGS += $$($(PKG_CONFIG) --libs ncurses)
endif

# Build targets
.PHONY: all clean install uninstall install-clock install-timer uninstall-clock uninstall-timer

# Default target builds both programs
all: $(CLOCK_BIN) $(TIMER_BIN)

# Build tty-clock
$(CLOCK_BIN): $(COMMON_SRC) $(CLOCK_SRC) ttyclock.h
	@echo "building tty-clock"
	$(CC) $(CFLAGS) $(COMMON_SRC) $(CLOCK_SRC) -o $(CLOCK_BIN) $(LDFLAGS)

# Build tty-timer  
$(TIMER_BIN): $(COMMON_SRC) $(TIMER_SRC) ttyclock.h
	@echo "building tty-timer"
	$(CC) $(CFLAGS) $(COMMON_SRC) $(TIMER_SRC) -o $(TIMER_BIN) $(LDFLAGS)

# Legacy target for backward compatibility (renamed to avoid circular dependency)
clock: $(CLOCK_BIN)
timer: $(TIMER_BIN)

# Install both programs
install: $(CLOCK_BIN) $(TIMER_BIN)
	@echo "installing both programs to ${INSTALLPATH}"
	@mkdir -p ${INSTALLPATH}
	@cp $(CLOCK_BIN) ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/$(CLOCK_BIN)
	@cp $(TIMER_BIN) ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/$(TIMER_BIN)
	@echo "installing manpages to ${MANPATH}"
	@mkdir -p ${MANPATH}
	@if [ -f $(CLOCK_BIN).1 ]; then \
		cp $(CLOCK_BIN).1 ${MANPATH}; \
		chmod 0644 ${MANPATH}/$(CLOCK_BIN).1; \
	fi
	@if [ -f $(TIMER_BIN).1 ]; then \
		cp $(TIMER_BIN).1 ${MANPATH}; \
		chmod 0644 ${MANPATH}/$(TIMER_BIN).1; \
	fi
	@echo "installed both programs"

# Install only tty-clock
install-clock: $(CLOCK_BIN)
	@echo "installing tty-clock to ${INSTALLPATH}"
	@mkdir -p ${INSTALLPATH}
	@cp $(CLOCK_BIN) ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/$(CLOCK_BIN)
	@if [ -f $(CLOCK_BIN).1 ]; then \
		mkdir -p ${MANPATH}; \
		cp $(CLOCK_BIN).1 ${MANPATH}; \
		chmod 0644 ${MANPATH}/$(CLOCK_BIN).1; \
	fi
	@echo "tty-clock installed"

# Install only tty-timer
install-timer: $(TIMER_BIN)
	@echo "installing tty-timer to ${INSTALLPATH}"
	@mkdir -p ${INSTALLPATH}
	@cp $(TIMER_BIN) ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/$(TIMER_BIN)
	@if [ -f $(TIMER_BIN).1 ]; then \
		mkdir -p ${MANPATH}; \
		cp $(TIMER_BIN).1 ${MANPATH}; \
		chmod 0644 ${MANPATH}/$(TIMER_BIN).1; \
	fi
	@echo "tty-timer installed"

# Uninstall both programs
uninstall:
	@echo "uninstalling both programs"
	@rm -f ${INSTALLPATH}/$(CLOCK_BIN)
	@rm -f ${INSTALLPATH}/$(TIMER_BIN)
	@rm -f ${MANPATH}/$(CLOCK_BIN).1
	@rm -f ${MANPATH}/$(TIMER_BIN).1
	@echo "both programs uninstalled"

# Uninstall only tty-clock
uninstall-clock:
	@echo "uninstalling tty-clock"
	@rm -f ${INSTALLPATH}/$(CLOCK_BIN)
	@rm -f ${MANPATH}/$(CLOCK_BIN).1
	@echo "tty-clock uninstalled"

# Uninstall only tty-timer
uninstall-timer:
	@echo "uninstalling tty-timer"
	@rm -f ${INSTALLPATH}/$(TIMER_BIN)
	@rm -f ${MANPATH}/$(TIMER_BIN).1
	@echo "tty-timer uninstalled"

# Clean build artifacts
clean:
	@echo "cleaning build files"
	@rm -f $(CLOCK_BIN) $(TIMER_BIN)
	@echo "cleaned"

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build both tty-clock and tty-timer (default)"
	@echo "  tty-clock     - Build only tty-clock"
	@echo "  tty-timer     - Build only tty-timer"
	@echo "  install       - Install both programs"
	@echo "  install-clock - Install only tty-clock"
	@echo "  install-timer - Install only tty-timer"
	@echo "  uninstall     - Uninstall both programs"
	@echo "  uninstall-clock - Uninstall only tty-clock"
	@echo "  uninstall-timer - Uninstall only tty-timer"
	@echo "  clean         - Remove build artifacts"
	@echo "  help          - Show this help message"