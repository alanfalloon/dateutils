ACLOCAL_AMFLAGS = -I m4
SUBDIRS = lib src
EXTRA_DIST = AUTHORS COPYING m4/gnulib-cache.m4

# For git-version-gen
BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	$(AM_V_GEN)echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	$(AM_V_GEN)echo $(VERSION) > $(distdir)/.tarball-version

# For gitlog-to-changelog (stolen from coreutils/Makefile.am)
.PHONY: gen-ChangeLog
dist-hook: gen-ChangeLog
gen-ChangeLog:
	$(AM_V_GEN)if (cd $(top_srcdir) &&					\
	               git rev-parse --git-dir) 2>&1 >/dev/null; then		\
	  (cd $(top_srcdir) &&							\
	   build-aux/gitlog-to-changelog) > $(distdir)/cl-t;			\
	  rm -f $(distdir)/ChangeLog;						\
	  mv $(distdir)/cl-t $(distdir)/ChangeLog;				\
	fi
