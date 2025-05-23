# Copyright (C) 2008-2012 Christian Anthon <anthon@kiku.dk>
# Copyright (C) 2012-2016 the AUTHORS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

#
# $Id: $
# 

gnubgiconsdir = $(datadir)
gnubgpixmapsdir = $(pkgdatadir)/pixmaps

ICONS_16_SCALABLE = icons/hicolor/16x16/apps/scalable/gnubg.svg
ICONS_16 = icons/hicolor/16x16/apps/gnubg.png
ICONS_22_SCALABLE = icons/hicolor/22x22/apps/scalable/gnubg.svg
ICONS_22 = icons/hicolor/22x22/apps/gnubg.png
ICONS_24_SCALABLE = icons/hicolor/24x24/apps/scalable/gnubg.svg
ICONS_24 = icons/hicolor/24x24/apps/gnubg.png
ICONS_32_SCALABLE = icons/hicolor/32x32/apps/scalable/gnubg.svg
ICONS_32 = icons/hicolor/32x32/apps/gnubg.png
ICONS_48_SCALABLE = icons/hicolor/48x48/apps/scalable/gnubg.svg
ICONS_48 = icons/hicolor/48x48/apps/gnubg.png

STOCK_MENU_IMAGES = \
		    16x16/actions/cancel_16.png \
		    16x16/actions/double_16.png \
		    16x16/actions/go_next_16.png \
		    16x16/actions/go_next_cmarked_16.png \
		    16x16/actions/go_next_game_16.png \
		    16x16/actions/go_next_marked_16.png \
		    16x16/actions/go_prev_16.png \
		    16x16/actions/go_prev_cmarked_16.png \
		    16x16/actions/go_prev_game_16.png \
		    16x16/actions/go_prev_marked_16.png \
		    16x16/actions/hint_16.png \
		    16x16/actions/ok_16.png \
		    16x16/actions/resign_16.png \
		    16x16/actions/runit_16.png

STOCK_BUTTON_IMAGES = \
		      24x24/actions/anti_clockwise_24.png \
		      24x24/actions/cancel_24.png \
		      24x24/actions/clockwise_24.png \
		      24x24/actions/double_24.png \
		      24x24/actions/go_next_24.png \
		      24x24/actions/go_next_cmarked_24.png \
		      24x24/actions/go_next_game_24.png \
		      24x24/actions/go_next_marked_24.png \
		      24x24/actions/go_prev_24.png \
		      24x24/actions/go_prev_cmarked_24.png \
		      24x24/actions/go_prev_game_24.png \
		      24x24/actions/go_prev_marked_24.png \
		      24x24/actions/hint_24.png \
		      24x24/actions/new0_24.png \
		      24x24/actions/new11_24.png \
		      24x24/actions/new1_24.png \
		      24x24/actions/new13_24.png \
		      24x24/actions/new15_24.png \
		      24x24/actions/new17_24.png \
		      24x24/actions/new3_24.png \
		      24x24/actions/new5_24.png \
		      24x24/actions/new7_24.png \
		      24x24/actions/new9_24.png \
		      24x24/actions/ok_24.png \
		      24x24/actions/resign_24.png \
		      24x24/actions/resignsb_24.png \
		      24x24/actions/resignsg_24.png \
		      24x24/actions/resignsn_24.png \
		      24x24/actions/runit_24.png

STOCK_IMAGES = \
	       $(STOCK_MENU_IMAGES) \
	       $(STOCK_BUTTON_IMAGES)

EXTRA_DIST = \
	     gnubg.ico \
	     $(STOCK_IMAGES) \
	     $(ICONS_16_SCALABLE) \
	     $(ICONS_22_SCALABLE) \
	     $(ICONS_24_SCALABLE) \
	     $(ICONS_32_SCALABLE) \
	     $(ICONS_48_SCALABLE)


if USE_GTK

CLEANFILES =
PIXMAPS = gnubg-big.png

stock-icons.list: $(STOCK_IMAGES) Makefile.am
	( rm -f $@; \
          for image in $(STOCK_IMAGES); do \
            echo $$image | \
              sed -e 's|.*/||' -e 's|-|_|g' -e 's|\.png$$||' >> $@; \
	    echo "  $(srcdir)/$$image" >> $@; \
	  done )

if FOUND_GDK_PIXBUF_CSOURCE
gnubg-stock-pixbufs.h: stock-icons.list
	$(GDK_PIXBUF_CSOURCE) --raw --build-list `cat $<` > $(@F)
CLEANFILES+=stock-icons.list gnubg-stock-pixbufs.h
else
gnubg-stock-pixbufs.h: stock-icons.list
	@echo ' ****************************'
	@echo ' ** unable to rebuild pixmaps'
	@echo ' ****************************'
endif

stock-icons.xml: $(STOCK_IMAGES) Makefile.am
	@echo '<?xml version="1.0" encoding="UTF-8"?>' > $@
	@echo '<gresources>' >> $@
	@echo ' <gresource prefix="/org/gnubg">' >> $@
	@for image in $(STOCK_IMAGES); do echo "  <file>$$image</file>" >> $@; done
	@echo ' </gresource>' >> $@
	@echo '</gresources>' >> $@

gnubg-stock-resources.c gnubg-stock-resources.h: stock-icons.xml
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate --c-name=gnubg_stock --manual-register $<

nobase_dist_gnubgicons_DATA = \
			      $(ICONS_16) \
			      $(ICONS_22) \
			      $(ICONS_24) \
			      $(ICONS_32) \
			      $(ICONS_48)

dist_gnubgpixmaps_DATA = $(PIXMAPS)

noinst_DATA = gnubg-stock-pixbufs.h

if FOUND_GLIB_COMPILE_RESOURCES
noinst_LTLIBRARIES = libresources.la
nodist_libresources_la_SOURCES = gnubg-stock-resources.c gnubg-stock-resources.h
AM_CPPFLAGS = @GLIB_CFLAGS@
BUILT_SOURCES = gnubg-stock-resources.h
CLEANFILES += gnubg-stock-resources.c gnubg-stock-resources.h stock-icons.xml
endif

endif
