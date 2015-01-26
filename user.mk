HEADERS = \
	src/defs.h \
	src/posix/defs.h \
	src/types/defs.h \
	src/io/defs.h \
	\
	src/posix/file.h \
	src/posix/poll.h \
	src/posix/thread.h \
	src/posix/time.h \
	src/posix/trace.h \
	\
	src/altc.h \
	src/dtoa.h \
	src/math.h \
	src/mem.h \
	src/res.h \
	src/string.h \
	src/try.h \
	\
	src/types/avltree.h \
	src/types/func.h \
	src/types/list.h \
	\
	src/io/chunk.h \
	src/io/output.h \
	src/io/print.h \
	src/io/string.h \

#ifeq ($(bmake_HOST),windows)
#else
#EXTRA = src/thread/posix/defs.h
#endif

all: altc.h

altc.h: $(HEADERS) user.mk
	rm -f $@
	printf '#ifndef LIBRT_H\n#define LIBRT_H\n' >> $@
	for inc in $(HEADERS) ; do sed -e'1,2d' -e'$$d' $$inc >> $@ ; done
	printf '#endif\n' >> $@

install: altc_h_install

altc_h_install: altc.h
	install --mode 0644 -D altc.h "$(PREFIX)/include/altc.h"

clean: altc_h_clean

altc_h_clean:
	rm -f altc.h

.PHONY: altc_h_clean
