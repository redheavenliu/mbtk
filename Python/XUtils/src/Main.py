from src.BaseWeb import BaseWeb
from src.MeiTuLuWeb import MeiTuLuWeb


if __name__ == '__main__':
    web = MeiTuLuWeb('MeiTuLu', 'www.meitulu.com', 'E:\\temp\\data\\meitulu\\url')
    web.pic_url_get()

    # web.print()

    web.pic_down()
