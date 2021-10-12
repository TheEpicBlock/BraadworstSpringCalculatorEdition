# ----------------------------
# Makefile Options
# ----------------------------

NAME = BWScalc
ICON = icon.png
DESCRIPTION = "De legendarische Braadworst spring, nu in je rekenmachine"
COMPRESSED = NO
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

scaleimages:
	$(Q)$(call MKDIR,$(GFXDIR)/scaled)
	$(Q)$(call MKDIR,$(GFXDIR)/backgrounded)
	$(Q)$(call MKDIR,$(GFXDIR)/generated)
	./scaleimages.py $(GFXDIR)

gfx: scaleimages
