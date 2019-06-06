# coding:utf-8 = True

class People:
    def __init__(self, name):
        print '__init__'
        self.name = name
        self.id = 1

    def info(self):
        print 'People : {}'.format(self.name)

def demo1():
    people = People('ABC')
    people.info()
    print '{} - {}'.format(people.id, people.name)

if __name__ == '__main__':
    demo1()
