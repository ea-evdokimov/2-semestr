# 1 task------------------
# n = int(input())
#
# for i in range(n):
#     print("   _~_   ", end=(i == n - 1) * "\n" + (i != n - 1) * " ")
# for i in range(n):
#     print("  (o o)  ", end=(i == n - 1) * "\n" + (i != n - 1) * " ")
# for i in range(n):
#     print(" /  V  \\ ", end=(i == n - 1) * "\n" + (i != n - 1) * " ")
# for i in range(n):
#     print("/(  _  )\\", end=(i == n - 1) * "\n" + (i != n - 1) * " ")
# for i in range(n):
#     print("  ^^ ^^  ", end=(i == n - 1) * "\n" + (i != n - 1) * " ")
# task 2-----------------
# main_str = input()
# # будем считать максимальную длину палиндрома с краю
# max_len_pal = 0
# rev_main_str = main_str[::-1]
#
#
# def is_palindrom(inp_str):
#     flag = True
#     inp_len = len(inp_str)
#
#     for i in range(inp_len):
#         if inp_str[i] != inp_str[inp_len - 1 - i]:
#             flag = False
#             break
#     return flag
#
#
# for i in range(len(main_str) + 1):
#     if is_palindrom(main_str[0:i]) or is_palindrom(rev_main_str[0:i]):
#         max_len_pal = i
# print(len(main_str) - max_len_pal)
# task 3-----------
# while True:
#     try:
#         line = input()
#         count_tab = 0
#         i = 0
#         while i < len(line):
#             if line[i] == "\t":
#                 count_tab += 1
#                 if count_tab == 4:
#                     i += 1
#                     break
#             i += 1
#         # i -указывает на первый символ после 4го таба
#         len_comment = len(line) - i
#         num_of_dots = 80 - 7 - len_comment
#         dots = "." * num_of_dots
#         print("{}{}{}".format(line[0:7], dots, line[i:]))
#     except:
#         break
# task 4------------------
# password = input()
# flag = True
# symbols = set()
# # показывает, что пароль сильный
# has_digit = False
# for letter in password:
#     if letter.isdigit():
#         has_digit = True
#     symbols.add(letter)
#
# if len(symbols) < 4 or "anna" in password.lower() or len(password) < 8 \
#         or password == password.lower() or not has_digit:
#     flag = False
#
# print("weak" * (1 - flag) + "strong" * flag)
