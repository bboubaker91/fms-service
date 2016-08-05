pkg_packages := dbus-1 dbus-glib-1

PKG_CFLAGS  := $(shell pkg-config --cflags $(pkg_packages))
PKG_LDFLAGS := $(shell pkg-config --libs $(pkg_packages))
# Add additional flags:
# -g : add debugging symbols
# -Wall : enable most gcc warnings
# -DG_DISABLE_DEPRECATED : disable GLib functions marked as deprecated
ADD_CFLAGS := -g -Wall -DG_DISABLE_DEPRECATED
# -DNO_DAEMON : do not daemonize the server (on a separate line so can
#               be disabled just by commenting the line)
ADD_CFLAGS += -DNO_DAEMON

# Combine flags
CFLAGS  := $(PKG_CFLAGS) $(ADD_CFLAGS) $(CFLAGS)
LDFLAGS := $(PKG_LDFLAGS) $(LDFLAGS)
targets= fms-server
fms-server: fms-server.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ... Listing cut for brevity ...

# The server and client depend on the respective implementation source
# files, but also on the common interface as well as the generated
# stub interfaces.
fms-server.o: fms-server.c common-defs.h fms-server-stub.h
	$(CC) $(CFLAGS) -DPROGNAME=\"$(basename $@)\" -c $< -o $@

clean:
	$(RM) $(targets) $(cleanfiles) *.o