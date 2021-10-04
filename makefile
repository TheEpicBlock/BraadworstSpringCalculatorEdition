# ----------------------------
# Makefile Options
# ----------------------------

NAME = BWScalc
ICON = icon.png
DESCRIPTION = "De legendarische Braadworst spring, nu in je rekenmachine"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -O3
CXXFLAGS = -Wall -Wextra -O3

# ----------------------------

include $(shell cedev-config --makefile)

scaleimages:
	$(Q)$(call MKDIR,$(GFXDIR)/scaled)
	$(Q)$(call MKDIR,$(GFXDIR)/backgrounded)
	$(Q)$(call MKDIR,$(GFXDIR)/generated)
	./scaleimages.py $(GFXDIR)

gfx: scaleimages
