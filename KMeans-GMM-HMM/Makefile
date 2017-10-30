
# headers:
ver    = debug

HDRS    = infra.h \
    IViterbi.h \
    htkfile.h \
    dataset.h \
    util.h \
    distance.h \
    feature.h \
    KMeans.h \
	GMM.h \
	CHMM.h 

# sources:
SRCS    = IViterbi.cc \
    htkfile.cc \
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

all: train_keyword_model test_keyword_model i_viterbi_std

train_keyword_model: train_keyword_model.o $(OBJS)
	$(CC) train_keyword_model.o $(OBJS) $(LFLAGS) -o train_keyword_model

test_keyword_model: test_keyword_model.o $(OBJS)
	$(CC) test_keyword_model.o $(OBJS) $(LFLAGS) -o test_keyword_model

i_viterbi_std: i_viterbi_std.o $(OBJS)
	$(CC) i_viterbi_std.o $(OBJS) $(LFLAGS) -o i_viterbi_std

%.o: %.cc $(HDRS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o train_keyword_model test_keyword_model i_viterbi_std

