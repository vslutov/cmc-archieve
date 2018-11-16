#!/usr/bin/python
# coding: utf

from __future__ import print_function

import readline
import sys
import tpg
import itertools

def make_op(s):
    return {
        '+': lambda x,y: x+y,
        '-': lambda x,y: x-y,
        '*': lambda x,y: x*y,
        '/': lambda x,y: x/y,
        '&': lambda x,y: x&y,
        '|': lambda x,y: x|y,
    }[s]

class Vector(list):
    def __init__(self, *argp, **argn):
        list.__init__(self, *argp, **argn)

    def __str__(self):
        return "[" + " ".join(str(c) for c in self) + "]"

    def __op(self, a, op):
        try:
            return self.__class__(op(s,e) for s,e in zip(self, a))
        except TypeError:
            return self.__class__(op(c,a) for c in self)

    def __add__(self, a): return self.__op(a, lambda c,d: c+d)
    def __sub__(self, a): return self.__op(a, lambda c,d: c-d)
    def __div__(self, a): return self.__op(a, lambda c,d: c/d)
    def __mul__(self, a): return self.__op(a, lambda c,d: c*d)

    def __and__(self, a):
        try:
            return reduce(lambda s, (c,d): s+c*d, zip(self, a), 0)
        except TypeError:
            return self.__class__(c and a for c in self)

    def __or__(self, a):
        try:
            return self.__class__(itertools.chain(self, a))
        except TypeError:
            return self.__class__(c or a for c in self)

class Calc(tpg.Parser):
    r"""

    separator spaces: '\s+' ;
    separator comment: '#.*' ;

    token fnumber: '\d+[.]\d*' float ;
    token number: '\d+' int ;
    token op1: '[|&+-]' make_op ;
    token op2: '[*/]' make_op ;
    token id: '\w+' ;

    START/e -> Operator $e=None$ | Expr/e | $e=None$ ;
    Operator -> Assign ;
    Assign -> id/i '=' Expr/e $Vars[i]=e$ ;
    Expr/t -> Fact/t ( op1/op Fact/f $t=op(t,f)$ )* ;
    Fact/f -> Atom/f ( op2/op Atom/a $f=op(f,a)$ )* ;
    Atom/a ->   Vector/a
              | id/i ( check $i in Vars$ | error $"Undefined variable '{}'".format(i)$ ) $a=Vars[i]$
              | fnumber/a
              | number/a
              | '\(' Expr/a '\)' ;
    Vector/$Vector(a)$ -> '\[' '\]' $a=[]$ | '\[' Atoms/a '\]' ;
    Atoms/v -> Atom/a Atoms/t $v=[a]+t$ | Atom/a $v=[a]$ ;

    """

calc = Calc()
Vars={}
PS1='--> '

def exec_caalc(line):
    try:
        res = calc(line)
    except tpg.Error as exc:
        print(exc, file=sys.stderr)
        res = None
    return res

if len(sys.argv) > 1:
    for filename in sys.argv[1:]:
        f = open(filename, 'r')
        lines = f.readlines()
        f.close()
        print(filename, ':', sep='')
        for line in lines:
            res = exec_caalc(line)
            if res != None:
                print(PS1, line.strip('\n'), sep='')
                print(res)
else:
    Stop=False
    while not Stop:
        try:
            line = raw_input(PS1)
        except BaseException as exc:
            print("Goodbye!")
            Stop = True

        if not Stop:
            res = exec_caalc(line)
            if res != None:
                print(res)
