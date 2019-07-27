# coding:utf-8=True

# 字典
def demo1():
    book = {'L1': 1, 'L2': 2, 'L3': 3}
    print book['L2']    # 2
    person = dict([("name",'Lili'), ('age', 20)])
    print person['name']
    person = dict(name='Lili', age=20)
    print person['age']

    # x = []   # 要添加只能用 x.append('abc')
    x = [None] * 10
    x[0] = 'abc'
    print x     # ['abc', None, None, None, None, None, None, None, None, None]

    y = {}
    y['name'] = 'LiMi'
    y['age'] = 20
    y['high'] = 170
    print y     # {'high': 170, 'age': 20, 'name': 'LiMi'}
    print "LiMi's age is %(age)d." % y  # LiMi's age is 20.

    data = {'title': 'My Home Page', 'text': 'Welcome to my home page!'}
    page = """
    <html>
    <head>%(title)s</head>
    <body>
    <h1>%(text)s</h1>
    </body>
    </html>
    """
    print page % data

    

if __name__ == '__main__':
    demo1()