#!/usr/bin/env python

"""
Reads over OrscLinks.cc and checks that the link bitfield table is valid. It
makes sure that all named bits are represented the appropriate number of times.
It does not check the arrangement of the bits.

Author: D. Austin Belknap, UW-Madison
"""

import re


ORSCLINKFILE = "../src/OrscLinks.cc"


def expected_counts():
    """
    Generate the different RCT bit names and the expected number of times it
    should occur in the link tables
    """
    out = {}

    out["0"] = 50

    # For each RC
    for i in xrange(7):
        # For each region
        for j in xrange(2):
            out["RCTau[%i][%i]" % (i, j)] = 1
            out["RCOf[%i][%i]" % (i, j)] = 1
            out["RCHad[%i][%i]" % (i, j)] = 1
            # For each bit
            for k in xrange(10):
                out["RCEt[%i][%i][%i]" % (i, j, k)] = 1

    # For each HF
    for i in xrange(8):
        out["HFFg[%i]" % (i)] = 1
        # for each bit
        for j in xrange(8):
            out["HFEt[%i][%i]" % (i, j)] = 1

    # For each EG index
    for i in xrange(4):
        # For each card + region bit
        for j in xrange(4):
            out["IEPos[%i][%i]" % (i, j)] = 1
            out["NEPos[%i][%i]" % (i, j)] = 1

        # For each bit
        for j in xrange(6):
            out["IEEt[%i][%i]" % (i, j)] = 1
            out["NEEt[%i][%i]" % (i, j)] = 1

    return out


def parse_file(infile):
    """
    Use Regex to grab the link tables from OrscLinks.cc as a string, and
    tokenize the output.
    """
    S = infile.read()

    pattern = "uint8_t L[12] \[24\]\[8\] = \{(.*?)\};"

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
