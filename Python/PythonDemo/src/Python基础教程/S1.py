# coding:utf-8=True
# 禁用整除
# from __future__ import division
import math           # 导入math库
import cmath
from math import floor  # 导入math库中的函数 floor

def demo1():
    print 1/2    # 0 如果添加 from __future__ import division 则为一般除法（禁用整除）
    print 1.0/2  # 0.5
    print 1/2.   # 0.5
    print 1//2.0  # 0.0 整除
    print 10 % 3    # 1
    print 9 % 3     # 0
    print 2.75 % 0.5    # 0.25
    print 2 ** 3    # 2^3 = 8
    print -3 ** 2   # -9
    print (-3) ** 2 # 9
    print 0xAF  # 175
    print 010   # 8 (八进制)
    print pow(2, 3)  # 8
    print abs(-10)  # 10
    print 10.0 / 3  # 3.33333333333
    print round(10.0 / 3)   # 3.0  (四舍五入)
#    print math.floor(32.9)  # 32.0    (向下取整)
    print floor(32.9)  # 32.0    (向下取整)   # math.floor()
    foo = math.floor
    print foo(32.9)     # 32.0
    print int(32.9)     # 32
 #  print math.sqrt(-1) # 报错
    print cmath.sqrt(-4)    # 2j （对负数求平方根，为虚数） 虚数以j/J结尾
    print cmath.sqrt(4)     # (2+0j)
    print (1 + 3j) * (9 + 4j)   # (-3+31j)

def demo2():
    # x = input("x : ")
    # print x
    print "Let's go!"
    print 'Let\'s go!'
    print '"Hello,world!", she said.'
    print "\"Hello,world!\", she said."
    print "Let's say:" '"Hello,world!"'     # 拼接
    x = 'a'
    y = 'b'
  #  print x y  # 错误
    print x + y     # ab
    print 'a' + "b" # ab

    # str和repr/反斜杠 都是将值转化为字符串，但str转化为更易阅读，repr转化为合法的python表达式
    print repr('abc')   # 'abc'
    print repr("abc")  # 'abc'
    print repr(1000000000000000L)  # 1000000000000000L
    print str('abc')    # abc
    print str(1000000000000000L)  # 1000000000000000
    print `1000000000000000L`  # 1000000000000000L 同 repr （python 3.0中不再支持）

    # input : 输入的必须为合法的python语句，raw_input ：会将所有输入放入字符串中
    # python3 中 input 同 raw_input
    '''
    name = input("name:")   # 输入 'abc' 合法
    print name
    name = raw_input('name:')
    print name
    '''

    # 多行代码
    s1 = """1、A
2、B
3、C"""
    print s1
    ''' 结果：
    1、A
    2、B
    3、C
    '''
    s1 = "a\
b\
c"
    print s1    # abc
    print 12 + \
        23          # 35

    # 原始字符串
    print 'C:\nb'
    '''
C:
b
    '''
    print r'C:\nb'  # C:\nb
    print r'Let\'s go.' # Let\'s go.
 #   print r"E:\Qualcomm\HEXAGON_Tools\" # 错误 ：原始字符串最后不能为 \
    print r'E:\Qualcomm\HEXAGON_Tools' '\\' # E:\Qualcomm\HEXAGON_Tools\

    # Unicode
    print u'ABC'    # ABC
    print repr(u'ABC')  # u'ABC'

if __name__ == '__main__':
    demo1()
    demo2()