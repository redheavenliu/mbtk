# coding:utf-8 = True
import random
# import math
import sys
import string


def random_demo():
    print 'random_demo() start'
    for i in range(0, 3):
        print random.randrange(1, 7)
    print 'random_demo() end'
# random_demo() end


# 列表：有序，可重复
def list_demo():
    print 'list_demo() start'
    lists = [
        1,
        1,
        1.0,
        'a word',
        True,
        [1, 2, 3],          # 列表
        (1, 2),             # 元组
        {1, 2, 3},          # 集合
        {'key': 'value'}    # 字典
    ]

    print lists
    lists.insert(3, 'insert')   # 在第3个位置插入
    # lists[3:3] = ['insert']   # 同上
    print lists
    lists.remove('a word')      # 删除
    # del lists[2:3]            # 同上
    print lists
    print 'list_demo() end'
# list_demo() end


# 字典：无序，key不重复
def dict_demo():
    print 'dict_demo() start'
    dicts = {
        'read': 0xFF0000,
        'green': 0x00FF00
    }
    print dicts
    dicts['write'] = 0xFFFFFF   # 添加/修改
    print dicts
    dicts.update({'black': 0x000000, 'blue': 0x0000FF})  # 添加/修改多个
    print dicts

    print 'dict_demo() end'
# dict_demo() end


# 元组：同列表，但不可修改
def tuple_demo():
    print 'tuple_demo() start'
    tuples = ('a', 'b', 'c', 'd', 'd')
    print tuples
    print tuples[3]
    # tuples[0] = 'e' 元组不可修改
    print 'tuple_demo() end'
# tuple_demo() end


# 集合：无序、不重复
def set_demo():
    print 'set_demo() start'
    sets = {'a', 'b', 'c', 'd', 'd'}
    print sets
    sets.add('f')
    sets.add('e')
    print sets
    sets.discard('d')       # 删除
    print sets
    print 'set_demo() end'
# set_demo() end


# 元组：同列表，但不可修改
def tips_demo():
    print 'tips_demo() start'
    num_list = [6, 2, 7, 4, 1, 3, 5]
    print sorted(num_list)        # 排序(升)
    print sorted(num_list, reverse=True)  # 排序(降)
    print num_list

    list1 = [1, 2, 4, 3]
    list2 = ['a', 'b', 'd', 'c']
    for i, j in zip(list1, list2):
        print i, 'is', j

    # 推倒式赋值
    list3 = [item for item in range(1, 11)]   # [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    print list3
    print [item**2 for item in range(1, 4)]     # [1^2, 2^2, 3^2]
    print [item for item in range(1, 11) if item % 2 == 0]  # 偶数
    print [item.lower() for item in 'ABCDE']
    print {i: i+1 for i in range(5)}    # {0: 1, 1: 2, 2: 3, 3: 4, 4: 5}
    print {i: j for i, j in zip(range(1, 6), 'abcde')}    # {1: 'a', 2: 'b', 3: 'c', 4: 'd', 5: 'e'}
    print {i: j.upper() for i, j in zip(range(1, 6), 'abcde')}  # {1: 'A', 2: 'B', 3: 'C', 4: 'D', 5: 'E'}

    list4 = [item for item in 'abcde']
    for num, item in enumerate(list4):
        print item, 'index is', num

    print 'tips_demo() end'
# tips_demo() end


def words_count():
    print 'words_count() start'
    # path = 'E:\\data\\Walden.txt'
    path = 'E:\\data\\words.txt'
    with open(path, 'r') as text:
        # words = text.read().split()   # 默认以空字符(空格、回车、Tab)分割
        print string.punctuation    # !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
        # 删除两端指定字符并转化为小写
        words = [word.strip(string.punctuation).lower() for word in text.read().split()]
        # 将列表转化为集合，再转化为字典
        word_dict = {word: words.count(word) for word in set(words)}
        print word_dict
        for word in sorted(word_dict, key=lambda x: word_dict[x], reverse=True):
            print word, '-', word_dict[word]
    print 'words_count() end'
# words_count() end


if __name__ == '__main__':
    if sys.version > '3':
        PY3 = True
    else:
        PY3 = False
    if PY3:
        print('start')
    else:
        print 'start'

    random_demo()
    list_demo()
    dict_demo()
    tuple_demo()
    set_demo()
    tips_demo()
    words_count()

    if PY3:
        print('end')
    else:
        print 'end'
# main() end
