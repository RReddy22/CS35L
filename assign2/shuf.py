#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
import argparse

class randline:
    def __init__(self, lines, replace):
        self.replace = replace
        self.lines = lines
        
    def chooseline(self):
        lines = self.lines
        line = random.choice(lines)
        if self.replace == False:
            self.lines.remove(line)
        return line
        
def main():
    version_msg = "%(prog)s 2.0"
    usage_msg = """%(prog)s [OPTION]... FILE

Output randomly selected lines from FILE."""

    parser = argparse.ArgumentParser(usage=usage_msg)
    parser.add_argument('--version', action='version', version=version_msg)
    parser.add_argument('input_file',
                      nargs='?',  type=argparse.FileType('r'), default=sys.stdin,
                      help='specifies the input file name')
    parser.add_argument("-n", "--head-count",
                      action="store", dest="numlines", default=None,
                      help="output NUMLINES lines (default 1)")
    parser.add_argument("-r", "--repeat", action='store_true', dest="replace",
                        help="Repeat output values, that is, select with replacement")
    parser.add_argument("-e", "--echo", nargs='*', dest="string", help="Treat each command-line operand as an input line", default=None)
    parser.add_argument("-i", "--input-range", dest="integer", default=None, help="Act as if input came from a file containing the range of unsigned decimal integers lo…hi, one per line")
    args = parser.parse_args()

    input_file = args.input_file

    try:
        match [args.string, args.integer]:
            case [None, None]:
                strings = args.input_file.readlines()
            case [_,None]:
                strings = list(map(lambda s: s + "\n", args.string))
            case [None, _]:
                lo, hi = args.integer.split("-")
                strings = list(map(lambda n: str(n) + "\n", range(int(lo),int(hi)+1)))
            case _:
                parser.error("input range in echo both specified")
                
        generator = randline(strings, args.replace)

        if args.numlines is not None:
            
            try:
                numlines = int(args.numlines)
            except:
                parser.error("invalid NUMLINES: {0}".
                     format(args.numlines))

            if numlines < 0:
                parser.error("negative count: {0}".
                     format(numlines))

            for index in range(numlines):
                sys.stdout.write(generator.chooseline())
        else:
            while len(generator.lines) > 0:
                sys.stdout.write(generator.chooseline())
                
    except IOError as err:
        parser.error("I/O error({0}): {1}".
                     format(err.errno, err.strerror))

if __name__ == "__main__":
    main()
