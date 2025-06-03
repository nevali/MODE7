ACLOCAL_AMFLAGS = -I BuildTools/M4

SUBDIRS = Sources .

EXTRA_DIST = README.md LICENSE

nodist_noinst_SCRIPTS = TeletextDemo TeletextServer MODE7

CLEANFILES = $(noinst_nodist_SCRIPTS)

TeletextDemo: BuildTools/Scripts/TeletextDemo.sh
	rm -f $@ && cp $< $@ && chmod +x $@
TeletextServer: BuildTools/Scripts/TeletextServer.sh
	rm -f $@ && cp $< $@ && chmod +x $@
MODE7: BuildTools/Scripts/MODE7.sh
	rm -f $@ && cp $< $@ && chmod +x $@
