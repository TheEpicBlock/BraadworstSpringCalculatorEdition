# ----------------------------
# Makefile Options
# ----------------------------

NAME = BraadworstSpring
ICON = icon.png
DESCRIPTION = "De legendarische Braadworst spring, nu in je rekenmachine"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
