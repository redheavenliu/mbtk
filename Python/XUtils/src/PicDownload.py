# coding:utf-8=True
import requests
from urllib.request import urlretrieve


def pic_down_urlretrieve(file, url):
    urlretrieve(url, file)


def pic_down_request(file, url):
    r = requests.get(url)
    with open(file, 'wb') as f:
        f.write(r.content)


def pic_down_chunk(file, url):
    r = requests.get(url, stream=True)
    with open(file, 'wb') as f:
        for chunk in r.iter_content(chunk_size=32):
            f.write(chunk)


"""
if __name__ == '__main__':
    print("Start.")
    pic_down_urlretrieve("E:\\temp\\data\\pic_1.jpg",
                         'https://ss0.bdstatic.com/94oJfD_bAAcT8t7mm9GUKT-xh_/timg?image&quality=100&size=b4000_4000&sec=1583302964&di=8d1183428e263cedba91018824272051&src=http://a0.att.hudong.com/78/52/01200000123847134434529793168.jpg')
    print("Urlretrieve download success.")
    pic_down_request("E:\\temp\\data\\pic_2.jpg",
                         'https://ss0.bdstatic.com/94oJfD_bAAcT8t7mm9GUKT-xh_/timg?image&quality=100&size=b4000_4000&sec=1583302964&di=8d1183428e263cedba91018824272051&src=http://a0.att.hudong.com/78/52/01200000123847134434529793168.jpg')
    print("Request download success.")
    pic_down_chunk("E:\\temp\\data\\pic_3.jpg",
                         'https://ss0.bdstatic.com/94oJfD_bAAcT8t7mm9GUKT-xh_/timg?image&quality=100&size=b4000_4000&sec=1583302964&di=8d1183428e263cedba91018824272051&src=http://a0.att.hudong.com/78/52/01200000123847134434529793168.jpg')
    print("Chunk download success.")
"""