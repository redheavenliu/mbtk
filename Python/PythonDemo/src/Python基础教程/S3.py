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

    print '%s plus %s equals %s' % (1, 1, 2)    # 1 plus 1 equals 2
    print '$%d , %x, %f, %i, %s, %r' % (30, 42, pi, pi, 42L, 42L)   # $30 , 2a, 3.141593, 3, 42, 42L
    print '%10f, %10.2f, %.2f, %.5s' % (pi, pi, pi, 'abcdefg')  # '  3.141593,       3.14, 3.14, abcde'
    # 字段宽为10，不足补空格
    print '%.*s' % (5, 'abcdedg')   # abcde
    # *由后面参数指定
    print '%010.2f' % (pi)  # 0000003.14
    # 不足位由 0 补齐
    print 010       # 8
    # 八进制
    print '%-10.2f' % (pi)  # '3.14      '
    # 左对齐
    print ('% 5d' % 10) + '\n' + ('% 5d' % -100)
    #   10
    # -100
    print ('%+5d' % 10) + '\n' + ('%+5d' % -100)    # +号表示显示正负号
    #  +10
    # -100

    print '%-*s%*s' % (10, 'Item', 20, 'Price')
    print '%-*s%*.2f' % (10, 'Apple', 20, 1.92)
    # 'Item                     Price'
    # 'Apple                     1.92'

    # 字符常量
    import string
    print string.digits     # 0123456789
    print string.letters    # abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
    print string.lowercase  # abcdefghijklmnopqrstuvwxyz
    print string.uppercase  # ABCDEFGHIJKLMNOPQRSTUVWXYZ
    print string.printable  # 所有可打印字符
    print string.punctuation    # 所有标点符号

    # 查找第一个匹配的字符串
    print 'With a goot boay.'.find('a')     # 5
    print 'goot boy'.find('oo')         # 1
    print 'goot boy'.find('abc')        # -1
    # 从索引1开始查找
    print '$$$ abc $$$'.find('$$$', 1)  # 8
    # 指查找开始和结束位
    print 'This is good.'.find('is', 4, 5)  # -1

    # 用待定字符串连接连接（与 split 相反）
    seq = ['1', '2', '3', '4']
    sep = '+'
    print sep.join(seq)     # 1+2+3+4
    dirs = '', 'usr', 'bin', 'env'
    print '/'.join(dirs)     # /usr/bin/env
    print 'C:' + '\\'.join(dirs)    # C:\usr\bin\env

    print 'GOOD'.lower()        # good
    print "that's all folks.".title()   # That'S All Folks.
    print "that's all folks.".capitalize()  # That's all folks.
    print 'This is a test.'.replace('is', 'ab')  # Thab ab a test.
    print '/usr/bin/env'.split('/')     # ['', 'usr', 'bin', 'env']
    # 去掉首尾空格
    print '     abc def    '.strip()    # 'abc def'
    # 去掉首尾指定字符
    print ' *** Good * Boy.!!!'.strip(' *!')    # 'Good * Boy.'

    from string import maketrans
    table = maketrans('ab', 'xy')   # a -> x  ; b -> y
    print 'This is a baby.'.translate(table)  # This is x yxyy.
    # 第二个参数为要删除的字符
    print 'This is a baby.'.translate(table, ' ')  # Thisisxyxyy.
    

if __name__ == '__main__':
    demo1()