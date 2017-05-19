CXX = gcc
SRCS = concurrency3.c linkedlist.c
HEADERS = linkedlist.h
OBJS = concurrency3.o linkedlist.o
 
concurrency3: ${OBJS} ${HEADERS}
		    ${CXX} ${OBJS} -o concurrency3 -lpthread
		    	     
${OBJS}: ${SRCS}
		    ${CXX} -c ${@:.o=.c}
		    	     
clean:
		    rm -f *.o concurrency3

