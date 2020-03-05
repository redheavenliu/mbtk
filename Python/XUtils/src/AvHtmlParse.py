# coding:utf-8=True
import requests
import urllib.request


"""
https://www.meitulu.com/t/1115 - https://www.meitulu.com/t/1115/6.html
URL = r"https://www.meitulu.com/t/1115/"
"""


def page_parse(url):
    conn = requests.get(url)
    encode = "utf8"
    should_find_url = False
    for line_byte in conn.iter_lines():
        try:
            line = str(line_byte, encoding=encode)
        except UnicodeDecodeError:
            encode = "gbk"
            line = str(line_byte, encoding=encode)
        if should_find_url:
            if line.startswith(r"<p class"):
                # print(line)
                for s in line.split(r'"'):
                    if s.startswith("http"):
                        print(s)
                        break
                should_find_url = False
                continue
        if line.startswith(u"<p>模"):
            if r"<a href" not in line:
                should_find_url = True
                # print(line)
                start = line.index("：")
                end = line.index("</p>")
                # print(str(start) + ":" + str(end))
                print(line[start + 1:end])
            else:
                av_url = ''
                av_name = ''
                for s in line.split(r'"'):
                    if s.startswith("http"):
                        av_url = s
                    elif s.startswith('>'):
                        av_name = s[1:s.index('<')]
                print(av_name + ":" + av_url)


def text_process():
    for file_line in open(r'E:\temp\data\url.txt', 'r', encoding='utf8').readlines():
        texts = file_line.split(',')
        if len(texts) == 2:
            # print(texts[0])
            conn = urllib.request.urlopen(texts[1])
            # print(conn.getcode())
            encode = "utf8"
            index = 1
            for line_byte in conn.readlines():
                try:
                    line = str(line_byte, encoding=encode)
                except UnicodeDecodeError:
                    encode = "gbk"
                    line = str(line_byte, encoding=encode)

                # print(line)
                if line.startswith(r'<a href='):
                    start = line.index('"') + 1
                    end = start + line[start + 1:].index('"') + 1
                    print("{},{},{}".format(texts[0], index, line[start:end]))
                    index += 1


def file_process():
    with open(r"E:\meitulu", 'r', encoding='utf8') as f_1:
        list_1 = []
        for line in f_1.readlines():
            splits = line.split(',')
            if len(splits) == 3:
                list_1.append("{},{}".format(splits[0], splits[1]))

        print(len(list_1))

        from collections import Counter
        b = dict(Counter(list_1))
        # print([key for key, value in b.items() if value > 1])  # 只展示重复元素
        print({key: value for key, value in b.items() if value > 1})  # 展现重复元素和重复次数


if __name__ == '__main__':
    file_process()
