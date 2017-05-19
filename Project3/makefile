CXX = gcc
SRCS = concurrency3.c lib.c
HEADERS = lib.h
OBJS = concurrency3.o lib.o
 
concurrency3: ${OBJS} ${HEADERS}
		    ${CXX} ${OBJS} -o concurrency3 -lpthread
		    	     
${OBJS}: ${SRCS}
		    ${CXX} -c ${@:.o=.c}
		    	     
clean:
		    rm -f *.o concurrency3

