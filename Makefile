CFLAGS=-Wall -g -std=gnu99 # -fsanitize=address
CC=clang

mdp: mdp.c mdp.h
	${CC} ${CFLAGS} -c mdp.c

start: mdp start.c
	${CC} ${CFLAGS} -o start start.c mdp.o

utilities: mdp utilities.c utilities.h
	${CC} ${CFLAGS} -c utilities.c

# NB: value and policy depend on utilities, but the dependency is removed so 
# the calc_eu and calc_meu stubs are not inadvertently compiled

value: mdp value_iteration.c
	${CC} ${CFLAGS} -o value_iteration value_iteration.c mdp.o utilities.o

policy: mdp policy_iteration.c policy_evaluation.c
	${CC} ${CFLAGS} -c policy_evaluation.c 
	${CC} ${CFLAGS} -o policy_iteration policy_iteration.c  \
	mdp.o utilities.o policy_evaluation.o

transition: mdp
	${CC} ${CFLAGS} -o transition transition.c mdp.o 

tidy: 
	rm -f *~

clean: tidy
	rm -f mdp.o policy_evaluation.o # NB: Does NOT delete utilities.o
	rm -f start value_iteration policy_iteration
