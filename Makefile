CFLAGS = -g
objs = libExtrai.o libHelp.o libInsert-a.o libInsert.o libMove.o libList.o libRemove.o libVina.o

all: vina++

vina++: $(objs) source/vina++.c
	gcc $(CFLAGS) -o vina++ source/vina++.c $(objs)

libExtrai.o: source/libExtrai.c source/libExtrai.h source/libVina.h source/libInsert.h source/libList.h
	gcc $(CFLAGS) -c source/libExtrai.c

libHelp.o: source/libHelp.c source/libHelp.h
	gcc $(CFLAGS) -c source/libHelp.c

libInsert-a.o: source/libInsert-a.c source/libInsert-a.h source/libVina.h source/libInsert.h source/libList.h
	gcc $(CFLAGS) -c source/libInsert-a.c

libInsert.o: source/libInsert.c source/libInsert.h source/libVina.h source/libList.h
	gcc $(CFLAGS) -c source/libInsert.c

libMove.o: source/libMove.c source/libMove.h source/libVina.h source/libList.h source/libInsert.h
	gcc $(CFLAGS) -c source/libMove.c

libList.o: source/libList.c source/libList.h
	gcc $(CFLAGS) -c source/libList.c

libRemove.o: source/libRemove.c source/libRemove.h source/libVina.h source/libList.h source/libInsert.h
	gcc $(CFLAGS) -c source/libRemove.c

libVina.o: source/libVina.c source/libVina.h source/libList.h
	gcc $(CFLAGS) -c source/libVina.c
	
clean:
	rm -f vina++ $(objs) *~

purge: clean
