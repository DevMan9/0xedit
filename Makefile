CC = clang
OBJS = editor.o main.o tops.o doublylinkedlist.o paintbuffer.o input.o render.o
PROG = 0xedit

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $@


main.o: main.c
editor.o: editor.c editor.h
tops.o: tops.c tops.h
doublylinkedlist.o: doublylinkedlist.c doublylinkedlist.h
paintbuffer.o: paintbuffer.c paintbuffer.h
input.o: input.c input.h
render.o: render.c render.h


clean:
	rm -f $(OBJS) $(PROG)