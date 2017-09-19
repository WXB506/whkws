
# headers:
ver    =debug

HDRS    = infra.h \
    htkfile.h \
    dataset.h \
    util.h \
    distance.h \
    feature.h \
    KMeans.h \
	GMM.h \
	CHMM.h 

# sources:
SRCS    = htkfile.cc \
    dataset.cc \
    util.cc \
    distance.cc \
    feature.cc \
    KMeans.cc\
    GMM.cc \
    CHMM.cc 

CC      = g++

ifeq ($(ver), debug)
	CFLAGS = -g -std=c++0x -Iinfra2 -I.
	LFLAGS = -g -std=c++0x -pthread  -Linfra2 -linfra
else
	CFLAGS = -O3 -std=c++0x -Iinfra2 -I.
	LFLAGS = -O3 -std=c++0x -pthread -Linfra2 -linfra
endif

OBJS = $(SRCS:.cc=.o)


#==========================================================#

all: train_keyword_model

train_keyword_model: train_keyword_model.o $(OBJS)
	$(CC) train_keyword_model.o $(OBJS) $(LFLAGS) -o train_keyword_model

%.o: %.cc $(HDRS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o train_keyword_model

