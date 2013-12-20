#!/usr/bin/env python

"""
Reads over OrscLinks.cc and checks that the link bitfield table is valid. It
makes sure that all named bits are represented the appropriate number of times.
It does not check the arrangement of the bits.

Author: D. Ausitn Belknap, UW-Madison
"""

import re


ORSCLINKFILE = "../src/OrscLinks.cc"


def expected_counts():
    """
    Generate the different RCT bit names and the expected number of times it
    should occur in the link tables
    """
    out = {}

    out["0"] = 18
    out["ElecBC0"] = 2
    out["ElecBC0"] = 2

    # For each RC
    for i in xrange(7):
        # For each region
        for j in xrange(2):
            out["RCEtId[%i][%i]" % (i, j)] = 1
            # For each bit
            for k in xrange(10):
                out["RC[%i][%i][%i]" % (i, j, k)] = 1

    # For each EG index
    for i in xrange(4):
        out["IEReg[%i]" % i] = 1
        out["NEReg[%i]" % i] = 1

        # For each card bit
        for j in xrange(3):
            out["IECard[%i][%i]" % (i, j)] = 1
            out["NECard[%i][%i]" % (i, j)] = 1

        # For each bit
        for j in xrange(6):
            out["IE[%i][%i]" % (i, j)] = 1
            out["NE[%i][%i]" % (i, j)] = 1

    return out


def parse_file(infile):
    """
    Use Regex to grab the link tables from OrscLinks.cc as a string, and
    tokenize the output.
    """
    S = infile.read()

    pattern = "uint8_t L[12] \[16\]\[8\] = \{(.*?)\};"

    x = ",".join(re.findall(pattern, S, flags=re.DOTALL))

    for i in ["\n", "{", "}", " "]:
        x = x.replace(i, "")

    out = x.split(",")

    return out
    


def generate_counts(x):
    """Compute the number of times each named bit occurs"""
    out = {}
    for i in x:
        out.setdefault(i, 0)
        out[i] += 1

    return out


def check_file():
    """
    Open OrscLinks.cc, parse the link tables from file, compute the number of
    times each named bit occurs in the table and compare with the expected.
    """
    with open(ORSCLINKFILE, 'r') as infile:
        x = parse_file(infile)

    expect = expected_counts()
    actual = generate_counts(x)

    error = False

    for i in expect:
        try:
            if expect[i] != actual[i]:
                print "Mismatch:", i, actual[i]
                error = True
            else:
                pass
        except KeyError:
            print "Missing:", i
            error = True

    if not error:
        print "No Errors Found"


def main(argv=None):
    check_file()
    return 0


if __name__ == "__main__":
    status = main()
    exit(status)
