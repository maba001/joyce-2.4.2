
noinst_DATA = joyce.lyx joyce.pdf joyce.txt \
              edops.lyx edops.pdf edops.txt \
	      anne.lyx  anne.pdf anne.txt \
	      hardware.lyx hardware.pdf hardware.txt \
	      pcwkey.ps

EXTRA_DIST = $(noinst_DATA)

%.txt:	%.lyx
	lyx -E text $@ $< 

%.tex:	%.lyx
	lyx -E latex $@ $< 

%.pdf:	%.lyx
	lyx -E pdf $@ $< 
