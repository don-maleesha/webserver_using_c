CC = gcc # This is the compiler to use
CFLAGS = -I. # These are the flags to pass to the compiler
DEPS = ./lib/file_helper.h ./lib/http_helper.h # These are the dependencies
OBJ = server.o # These are the object files
USERID = 123456789 # This is your user ID

# This is a rule to make the object file from the source file
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)  # Use $< for the source file

all: server # This is the rule to make the executable

# This is a rule to make the executable from the object files
server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# This is a rule to clean up the directory
clean:
	rm -rf *.o server *.tar.gz

dist: tarball # This is a rule to make the distribution tarball

# This is a rule to make the distribution tarball
tarball: clean
	tar -cvzf /tmp/$(USERID).tar.gz --exclude=./.vagrant . && mv /tmp/$(USERID).tar.gz .