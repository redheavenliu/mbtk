from src.BaseWeb import BaseWeb


class MeiTuLuWeb(BaseWeb):

    def __init__(self, name, host, pack_file):
        BaseWeb.__init__(self, name, host, pack_file)

    # /item/20103_5.html
    def _page_uil_next(self, package_uil, index):
        if index <= 1:
            return "{}.html".format(package_uil)
        else:
            return "{}_{}.html".format(package_uil, index)

    def _pic_split_text_get(self):
        return '"'

    def _pic_start_text_get(self):
        return '<center><img src='
