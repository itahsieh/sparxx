OBJS:=main.o
DEPENDS:=Makefile
WARNINGS:=-Wall -Werror

spartran: $(OBJS)
	g++ -o $@ $^

%.o: %.cxx $(DEPENDS)
	g++ $(WARNINGS) -o $@ -c $<
