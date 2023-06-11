# $FreeBSD$

PROG=	netdumpd
SRCS=	netdumpd.c	\
	cap_herald.c
# 	cap_dns.c	\
# 	cap_handler.c

MAN=	netdumpd.8
BINDIR=	/usr/sbin

LDADD+=	-lcasper -lnv -lutil
# LDADD+=	-lnv -lutil

# CFLAGS+= -DWITH_CASPER -DWITH_CAPSICUM
CFLAGS+= -DWITH_CASPER

WARNS?=	6

# rc.d script.
SUBDIR+=	etc/rc.d

.include <bsd.prog.mk>
