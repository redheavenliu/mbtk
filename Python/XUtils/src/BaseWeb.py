import requests
import os
from src.Pic import Pic

PIC_FILE_PATH = "E:\\temp\\data"


class BaseWeb:
    __web_name = ''
    __web_host = ''
    __package_file = ''
    # [name, id, url]
    __package_list = []
    # class Pic
    __pic_list = []

    def __init__(self, name, host, pack_file):
        self.__web_name = name
        self.__web_host = host
        self.__package_file = pack_file
        self.__init_package()

    def __del__(self):
        pass

    def __init_package(self):
        f_config = open(self.__package_file)
        for line in f_config:
            if line[0] != '#' and line[0] != '\n':
                # [id, name, url]
                self.__package_list.append(line[0:-1].split(','))

    def __package_parse(self):
        for page in self.__package_list:
            # For each package.
            page_index = 1
            pic_path = self.__pic_path_get(page[0])
            if not os.path.exists(pic_path):
                os.mkdir(pic_path)
            for index in range(1, 100):  # Max 100 page
                # For each page.
                url = self._page_uil_next(page[2], index)
                # print("url:" + url)
                text = requests.get(url)
                no_pic = True
                for line in text.iter_lines():
                    line_s = str(line, encoding="utf8")
                    if line_s.startswith(self._pic_start_text_get()):   # Find pics
                        no_pic = False
                        # print(line_s)
                        for pic_url in line_s.split(self._pic_split_text_get()):
                            if pic_url.endswith('.jpg'):    # Is pic
                                self.__pic_list.append(
                                    Pic(r"{}\{}_{}.jpg".format(pic_path, page[1], page_index), pic_url))
                                page_index += 1
                if no_pic:  # Not find pic,so break.
                    break

    def __pic_path_get(self, name):
        return r'{}\{}\{}'.format(PIC_FILE_PATH, self.__web_name, name)

    # Implemented in subclasses
    def _page_uil_next(self, package_uil, index):
        pass

    # Implemented in subclasses
    def _pic_split_text_get(self):
        pass

    # Implemented in subclasses
    def _pic_start_text_get(self):
        pass

    def pic_down(self):
        for pic in self.__pic_list:
            pic.download()

    def pic_url_get(self):
        self.__package_parse()

    def print(self):
        print(self.__package_list)
        for pic in self.__pic_list:
            pic.print()



