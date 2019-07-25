# coding:utf-8=True
def demo1():
    # 字符串格式化
    formats = '%% Hello,%s.%s enough for you? pi = %.3f'
    from math import pi
    values = ('world', 'Hot', pi)       # values 只能为无组或字典
    print formats % values      # % Hello,world.Hot enough for you? pi = 3.142
    print 'PI = %.5f' % pi      # PI = 3.14159

    from string import Template
    s = Template('!$x, good. $x. and $y')
    print s.substitute(x='ABC', y='DDD')    # !ABC, good. ABC. and DDD

    s = Template('Good ab${x}efg $$ok.')  # 替换单词中的一部份
    print s.substitute(x='cd')  # Good abcdefg $ok.

    s = Template('$arg1 ok, $arg2 error.')
    d = {}
    d['arg1'] = 'a b c'
    d['arg2'] = 'e f g'
    print s.substitute(d)       # a b c ok, e f g error.


    

if __name__ == '__main__':
    demo1()