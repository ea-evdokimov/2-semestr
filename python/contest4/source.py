# ------------------------------------------------------------
# import math
#
#
# def pascal_triangle():
#     num_line = 1
#     nums = []
#     nums_old = []
#
#     while True:
#         nums.append(1)
#         nums_old.append(1)
#         nums[0] = 1
#         for i in range(num_line):
#             nums_old[i] = nums[i]
#         for i in range(num_line - 2):
#             nums[i + 1] = nums_old[i] + nums_old[i + 1]
#         for i in range(num_line):
#             yield nums[i]
#         num_line += 1
# -------------------------------------------------------
# import heapq
#
#
# def merge_sorter(*args):
#     mas = heapq.merge(*args)
#     for i in mas:
#         yield i
# ---------------------------------------------------------
# import functools
# import sys
#
#
# def takes(*types_of_args):
#     def decorator(func):
#         @functools.wraps(func)
#         # don't edit args of the function(__name__, __doc__, __module__)
#         def wrapper(*arguments):
#
#             flag = True
#             if types_of_args.__len__() <= arguments.__len__():
#                 for i in range(types_of_args.__len__()):
#                     if not isinstance(arguments[i], types_of_args[i]):
#                         flag = False
#                         break
#             else:
#                 for i in range(arguments.__len__()):
#                     if not isinstance(arguments[i], types_of_args[i]):
#                         flag = False
#                         break
#             if not flag:
#                 raise TypeError
#             else:
#                 func(*arguments)
#         return wrapper
#     return decorator
#
#
# exec(sys.stdin.read())
# @takes(int, str)
# def f(a, b):
#     print(a, b)
#
#
# try:
#     f(1, 'abc')
# except Exception as e:
#     print(type(e).__name__)
# ----------------------------------------------------------
# import functools
# from collections import OrderedDict
#
#
# def cache(size_of_cache):
#     cache_d = OrderedDict()
#
#     def decorator(func):
#         @functools.wraps(func)
#         # don't edit args of the function(__name__, __doc__, __module__)
#         def wrapper(*args, **kwargs):
#             arguments = (tuple(args), tuple(kwargs.items()))
#             if arguments in cache_d:
#                 return cache_d[arguments]
#
#             # else adding new cache
#             k = func(*args, **kwargs)
#             if cache_d.__len__() == size_of_cache:
#                 for key in cache_d:
#                     del cache_d[key]
#                     break
#             cache_d[arguments] = k
#             return k
#
#         return wrapper
#     return decorator
#
#
# @cache(3)
# def foo(value):
#     print("calculate foo for {}".format(value))
#     return value
#
#
# foo(1)
# foo(2)
# foo(1)
# foo(2)
# foo(3)
# foo(4)
# foo(1)
# --------------------------------------------------------
# import functools
# import time
#
#
# def profiler(func):
#     cur_calls = 0
#     func.last_time_taken = 0
#     func.calls = 0
#
#     @functools.wraps(func)
#     # don't edit args of the function(__name__, __doc__, __module__)
#     def wrapper(*args, **kwargs):
#         # cur_calls = 0 при первом входе в функцию и самом последнем выходе
#         nonlocal cur_calls
#         if cur_calls == 0:
#             wrapper.calls = 0
#         cur_calls += 1
#         start_time = time.time()
#         k = func(*args, **kwargs)
#         wrapper.last_time_taken = time.time() - start_time
#         cur_calls -= 1
#         # вышли из функции и увеличили calls
#         wrapper.calls += 1
#         return k
#
#     return wrapper
#
#
# @profiler
# def ackermann(m, n):
#     if m == 0:
#         return n + 1
#     if n == 0:
#         return ackermann(m - 1, 1)
#     return ackermann(m - 1, ackermann(m, n - 1))
#
#
# ackermann(3, 2)
# print(ackermann.__doc__)
# print(ackermann.last_time_taken)
# print(ackermann.calls)
#
# ackermann(3, 2)
# print(ackermann.__doc__)
# print(ackermann.last_time_taken)
# print(ackermann.calls)

# def sleepy_recursion(num_calls):
#     "I am a strange sleepy recursive function"
#     time.sleep(1)
#     if num_calls > 1:
#         sleepy_recursion(num_calls - 1)
#
#
# sleepy_recursion(3)
#
# print(sleepy_recursion.__doc__)
# print(sleepy_recursion.last_time_taken)
# print(sleepy_recursion.calls)
#------------------------------------------------