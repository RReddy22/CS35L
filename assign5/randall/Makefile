# Make x86-64 random byte generators.

# Copyright 2015, 2020, 2021 Paul Eggert

# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# Optimization level.  Change this -O2 to -Og or -O0 or whatever.
OPTIMIZE =

# The C compiler and its options.
CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd
#-fanalyzer

# The archiver command, its options and filename extension.
TAR = tar
TARFLAGS = --gzip --transform 's,^,randall/,'
TAREXT = tgz

default: randall.o options.o output.o rand64-hw.o rand64-sw.o
	$(CC) $(CFLAGS) -o randall randall.o options.o output.o rand64-hw.o rand64-sw.o

randall: randall.o options.o output.o rand64-hw.o rand64-sw.o
	$(CC) $(CFLAGS) -o randall randall.o options.o output.o rand64-hw.o rand64-sw.o

randall.o: randall.c options.c output.c rand64-hw.c rand64-sw.c options.h output.h rand64-hw.h rand64-sw.h
	$(CC) $(CFLAGS) randall.c options.c output.c rand64-hw.c rand64-sw.c -c

options.o: options.c options.h
	$(CC) $(CFLAGS) options.c -c

output.o: output.c output.h
	$(CC) $(CFLAGS) output.c -c

rand64-hw.o: rand64-hw.c rand64-hw.h
	$(CC) $(CFLAGS) rand64-hw.c -c

rand64-sw.o: rand64-sw.c rand64-sw.h
	$(CC) $(CFLAGS) rand64-sw.c -c

assignment: randall-assignment.$(TAREXT)
assignment-files = COPYING Makefile randall.c options.c output.c rand64-hw.c rand64-sw.c options.h output.h rand64-hw.h rand64-sw.h

randall-assignment.$(TAREXT): $(assignment-files)
	$(TAR) $(TARFLAGS) -cf $@ $(assignment-files)

submission-tarball: randall-submission.$(TAREXT)
submission-files = $(assignment-files) \
	notes.txt Makefile # More files should be listed here, as needed.                                                    

randall-submission.$(TAREXT): $(submission-files)
	$(TAR) $(TARFLAGS) -cf $@ $(submission-files)

repository-tarball:
	$(TAR) -czf randall-git.tgz .git

.PHONY: default clean assignment submission-tarball repository-tarball

check: default
	./randall 2 | wc -c | grep -Fxq "2" \
	&& ./randall -o 8 16 | wc -c | grep -Fxq "16" \
	&& echo "foo" > bar.txt && ./randall 3 -i /bar.txt | grep -Fxq "foo" && rm bar.txt \
	&& ./randall -i rand48_r 16 | wc -c | grep -Fxq "16" \
	&& ./randall -o 8 -i rand48_r 16 | wc -c | grep -Fxq "16" \
	&& echo "success" || echo "failure" 

clean:
	rm -f *.o *.$(TAREXT) randall
