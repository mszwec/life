
CC = gcc

CFLAGS = -c -g -Wall  			 	\
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

CGTK = `pkg-config gtk+-2.0 --cflags`
LGTK = `pkg-config gtk+-2.0 --libs`

LFLAGS = -o

OBJECTS_LIFE = life.o engine.o
OBJECTS_REPLICATOR = life.o replicator.o

all: replicator life

replicator.o: engine.c
	@echo "**** replicator.o ... ****"
	$(CC) $(CFLAGS) -DREPLICATOR engine.c -o replicator.o
	@echo "**** replicator.o ended ****"


engine.o: engine.c
	@echo "**** engine.o ... ****"
	$(CC) $(CFLAGS) engine.c -o engine.o
	@echo "**** engine.o ended ****"

life.o: life.c
	@echo "**** life.o ... ****"
	$(CC) $(CFLAGS) life.c $(CGTK)
	@echo "**** life.o ended ****"

life: $(OBJECTS_LIFE)
	@echo "**** Linking... Life****"
	$(CC) $(OBJECTS_LIFE) $(LGTK) $(LFLAGS) life	
	@echo "**** Linking ended ****"

replicator: $(OBJECTS_REPLICATOR)
	@echo "**** Linking... Replicator ****"
	$(CC) $(OBJECTS_REPLICATOR) $(LGTK) $(LFLAGS) replicator	
	@echo "**** Linking ended ****"

clean: 
	@echo "**** Cleanig ... ****"
	rm -rf *.o life replicator 
	@echo "**** Cleaning ended ****"

