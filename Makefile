# $FreeBSD$

PROG=	netdumpd

ifeq ($(TARGET), without_capsicum)
# CFLAGS+=
LDADD+=	-lnv -lutil
SRCS=	netdumpd.c	\
	nocap_handler.c

else
CFLAGS+= -DWITH_CASPER -DWITH_CAPSICUM
LDADD+=	-lcasper -lnv -lutil
SRCS=	netdumpd.c	\
	cap_dns.c	\
	cap_handler.c	\
	cap_herald.c

endif

MAN=	netdumpd.8
BINDIR=	/usr/sbin

WARNS?=	6

# rc.d script.
SUBDIR+=	etc/rc.d

.include <bsd.prog.mk>
