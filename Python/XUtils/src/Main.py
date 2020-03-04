# coding:utf-8=True
from src.BaseWeb import BaseWeb
from src.MeiTuLuWeb import MeiTuLuWeb
from src.ChangShiFang import ChangShiFang


def down_meitulu():
    web = MeiTuLuWeb('MeiTuLu', 'www.meitulu.com', r'G:\Linux\AvManager\url\meitulu')
    web.pic_url_get()
    # web.pic_down()


def down_changshifang():
    web = ChangShiFang("ChangShiFang", 'www.changshifang.com', r'G:\Linux\AvManager\url\changshifang')
    web.pic_url_get()
    # web.pic_down()


if __name__ == '__main__':
    down_meitulu()
    # down_changshifang()

