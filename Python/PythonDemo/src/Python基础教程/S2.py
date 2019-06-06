# coding:utf-8=True
def demo1():
    tag = '<a href="http://www.baidu.com">BaiDu</a>'
    print tag[9:-11]    # http://www.baidu.com
    print tag[9:29]     # http://www.baidu.com

    num = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    print num[3:6]      # [3, 4, 5]
    print num[0:1]      # [0]
    print num[-3:-1]    # [7, 8]
    print num[-3:]      # [7, 8, 9]
    print num[:3]       # [0, 1, 2]
    print num[:]        # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    print num[0:5:2]    # [0, 2, 4]     步长为2，默认为1
    print num[::4]      # [0, 4, 8]
    print num[8:2:-2]   # [8, 6, 4]
    print num[0:10:-2]  # []
    print num[::-2]     # [9, 7, 5, 3, 1]
    print num[5::-2]    # [5, 3, 1]
    print num[:5:-2]    # [9, 7]

    print [1, 2, 3] + [4, 5]    # [1, 2, 3, 4, 5]
    print 'Hello' + "World"     # HelloWorld
    # print 'H' + [1, 2]        # 错误
    print '123' * 3     # 123123123
    print [1, 2] * 2    # [1, 2, 1, 2]
    list1 = [None] * 10
    print list1
    str1 = 'good boy.'
    if 'o' in str1:
        print 'o in'

    if 'good' in str1:
        print 'good in'

    usr = [
        ['Name1', 1001],
        ['Name2', 1002],
        ['Name3', 1003]
    ]
    name = 'Name3'
    uid = 1003
    if [name, uid] in usr:
        print 'Usr in'

    print min(num)  # 0
    print max(num)  # 9
    print len(num)  # 10
    print max('cc', 'dd', 'a')  # dd
    list1 = list('Hollo') # ['H', 'o', 'l', 'l', 'o']
    list1[2] = 'abc'
    print list1     # ['H', 'o', 'abc', 'l', 'o']
    del list1[0]
    print list1     # ['o', 'abc', 'l', 'o']
    list1[2:] = list('54321')
    print list1     # ['o', 'abc', '5', '4', '3', '2', '1']

    n1 = [1, 5]
    n1[1:1] = [2, 3, 4] # '1'后插入
    print n1        # [1, 2, 3, 4, 5]
    n1[1:4] = []    # 删除
    # del n1[1:4]   # 同上
    print n1        # [1, 5]

    n1.append(6)
    n1.append('abc')
    print n1        # [1, 5, 6, 'abc']

    x = ['to', 'be', 'or', 'to', 'be', 'not']
    print x.count('to') # 2

    a = [1, 2, 3]
    a.extend([4, 5, 6]) # 连接
    print a     # [1, 2, 3, 4, 5, 6]
    print x.index('be')     # 1
    x.insert(2, 'good')
    print x     # ['to', 'be', 'good', 'or', 'to', 'be', 'not']
    print x.pop()     # 删除最后一个，返回删除的一个
    print x     # ['to', 'be', 'good', 'or', 'to', 'be']
    print x.pop(1)    # 删除index为1的那个
    print x     # ['to', 'good', 'or', 'to', 'be']

    x.remove('to')  # 删除第1个匹配的
    print x     # ['good', 'or', 'to', 'be']

    x.reverse() # 取反
    print x     # ['be', 'to', 'or', 'good']

    

if __name__ == '__main__':
    demo1()