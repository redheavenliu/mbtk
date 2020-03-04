# coding:utf-8=True
import os
import src.PicDownload


class Pic:
    __file_path = ''
    __url = ''

    def __init__(self, file_path, url):
        self.__file_path = file_path
        self.__url = url

    def download(self, method=0):
        if not os.path.exists(self.__file_path):
            print("Download:{},{}".format(self.__file_path, self.__url))
            if method == 0:
                src.PicDownload.pic_down_urlretrieve(self.__file_path, self.__url)
            elif method == 1:
                src.PicDownload.pic_down_request(self.__file_path, self.__url)
            else:
                src.PicDownload.pic_down_chunk(self.__file_path, self.__url)

    def print(self):
        print("{},{}".format(self.__file_path, self.__url))
