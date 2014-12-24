OBJS:=main.o
DEPENDS:=Makefile
WARNINGS:=-W -Wall -Werror
CFLAGS:=-std=c++0x

spartran: $(OBJS)
	g++ $(WARNINGS) -o $@ $^

%.o: %.cxx $(DEPENDS)
	g++ $(CFLAGS) $(WARNINGS) -o $@ -c $<
