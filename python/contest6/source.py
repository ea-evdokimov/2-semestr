################1##############
# import sys
#
#
# class ExtendedList(list):
#     def __getattr__(self, item):
#
#         if item == "reversed" or item == "R":
#             return self[::-1]
#
#         if item == "first" or item == "F":
#             return self[0]
#
#         if item == "last" or item == "L":
#             return self[self.__len__() - 1]
#
#         if item == "size" or item == "S":
#             return self.__len__()
#         else:
#             raise AttributeError()
#
#     def __setattr__(self, item, value):
#         if item == "first" or item == "F":
#             self[0] = value
#
#         elif item == "last" or item == "L":
#             self[self.size - 1] = value
#
#         elif item == "size" or item == "S":
#             if value > self.size:
#                 for i in range(value - self.size):
#                     self.append(None)
#             else:
#                 for i in range(self.size - value):
#                     self.pop()
#         else:
#             raise AttributeError()
#
#
# exec(sys.stdin.read())
#############3###################
# import sys
# import traceback
#
#
# def force_load(name_of_module):
#     name_of_file = name_of_module + ".py"
#
#     file = open(name_of_file, "r")
#     lines = file.readlines()
#     file.close()
#     rdict = ldict = {}
#     # получаем список строк
#     # выбросим все с ошибками
#     for line in lines:
#         try:
#             exec(''.join(lines), globals(), ldict)
#         except SyntaxError as error:
#             lines.pop(error.lineno - 1)
#         except Exception:
#             exc_tb = sys.exc_info()[2]
#             lineno = traceback.extract_tb(exc_tb)[-1][1]
#             lines.pop(lineno - 1)
#
#     exec(''.join(lines), globals(), rdict)
#
#     return rdict
##############4#################
from contextlib import contextmanager
import traceback


@contextmanager
def supresser(*args):
    try:
        yield
    # исключения только заданных типов
    except args:
        # не пробрасывается
        pass


@contextmanager
def retyper(type_from, type_to):
    try:
        yield
    except type_from as old_error:
        # заменяем args и traceback
        new_error = type_to()
        new_error.args = old_error.args
        # пробрасываем дальше
        raise new_error


@contextmanager
def dumper(stream):
    try:
        yield
    except Exception as error:
        # возвращает строку
        stream.write(traceback.format_exc())
        raise error

#############2#############
# import collections.abc
# import numbers
# from app import VeryImportantClass, decorator
#
#
# class HackedClass(VeryImportantClass):
#     def __init__(self):
#         super().__init__()
#
#     def __getattribute__(self, item):
#         vic_atr = VeryImportantClass.__getattribute__(self, item)
#         # если он публичный
#         if item[0] != "_":
#             # если значение атрибута - число(опр с помощью Number)
#             if isinstance(vic_atr, numbers.Number):
#                 return 2 * vic_atr
#             # если же это контейнер
#             elif isinstance(vic_atr, collections.abc.Container):
#                 # возвращаем пустой
#                 return type(vic_atr)()
#             elif callable(vic_atr):
#                 return decorator(vic_atr)
#         else:
#             return vic_atr
