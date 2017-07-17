#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import unittest

import itertools
from subprocess import Popen, PIPE, call, STDOUT
from tempfile import mkdtemp
from shutil import rmtree

import signal
class Alarm(Exception):
    pass

class ReformatterTest(unittest.TestCase):
    """Subclass and override inputs/expectedOutputs (and possibly other
stuff) to create new pretransfer tests."""

    inputs = [""]
    expectedOutputs = [""]
    expectedRetCodeFail = False

    def alarmHandler(self, signum, frame):
        raise Alarm

    def withTimeout(self, seconds, cmd, *args, **kwds):
        signal.signal(signal.SIGALRM, self.alarmHandler)
        signal.alarm(seconds)
        ret = cmd(*args, **kwds)
        signal.alarm(0)         # reset the alarm
        return ret



    def runTest(self):
        try:

            for inp, exp in zip(self.inputs, self.expectedOutputs):
                self.proc = Popen(['.././rehtml'], stdin = PIPE, stdout=PIPE, )
                self.assertEqual(self.proc.communicate(input=inp.encode('utf-8'))[0].strip().decode('utf-8')+"[][\n]", exp+"[][\n]")
                #strip and decode to remove '\n' at the end of output of communicate()
                #communicat() does the closing too.

            retCode = self.proc.poll()
            if self.expectedRetCodeFail:
                self.assertNotEqual(retCode, 0)
            else:
                self.assertEqual(retCode, 0)

        finally:
            pass

class InlineBlankReformatterTest(ReformatterTest):
    inputs =            ["[{<i>}]foo [{<b><i>}]bar[][]",
                        "[<p>]foo [{<b>}]bar fie [{<i><b>}]baz[][{<b>}] fum[] fiz[][</p>]",
                         ]
    expectedOutputs =   ["<i>foo </i><b><i>bar</i></b>",
                        "<p>foo <b>bar fie </b><i><b>baz</b></i><b> fum</b> fiz</p>",  
                         ]
