CXX = gcc
SRCS = concurrency5.c 
OBJS = concurrency5.o
 
concurrency5: ${OBJS}
		    ${CXX} ${OBJS} -o concurrency5 -lpthread
		    	     
${OBJS}: ${SRCS}
		    ${CXX} -c ${@:.o=.c}
		    	     
clean:
		    rm -f *.o concurrency5
