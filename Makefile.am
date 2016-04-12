# Przygotowane pod katem kompilacji z bibliotekami libskey i libmd (wegorz)

SUBDIRS = libmd libskey

INCLUDES = -O2 $(gtk_cflags) -Wall -Werror
LDFLAGS = $(gtk_libs)

#INSTALL_PROGRAM = cp

bin_PROGRAMS = gskey

gskey_SOURCES = \
	main.c gskey.c gskey.h

gskey_LDADD = -Llibmd -Llibskey -lskey -lmd 

