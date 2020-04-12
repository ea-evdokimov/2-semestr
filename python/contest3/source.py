#contest3_A
'''from collections import Counter
cnt = Counter()
flag = False

for i in input():
    cnt[i] += 1
    if cnt[i] > 1:
        flag = True
        break
print("False" * flag + "True" * (1 - flag))
'''
#-----------------------------------------------------------------
'''inp_str = input()
list_keys = list({i for i in inp_str})
list_keys.sort()
cnt = {i: 0 for i in list_keys}
i = 0

while i < inp_str.__len__():
    cur = inp_str[i]
    count = 0
    while i < inp_str.__len__() and cur == inp_str[i]:
        count += 1
        i += 1
    if count > cnt[inp_str[i - 1]]:
        cnt[inp_str[i - 1]] = count

for i in cnt.keys():
    print(i, end=' ')
    print(cnt[i])
'''
#------------------------------------------------------------------
'''
lst = input().split()
dct = {i: 0 for i in lst}
time_min = 0
time_max = 0
cur_max = 0
time = 0

for i in lst:
    time += 1
    dct[i] = 1 - dct[i]

    flag_min = 1
    count_max = 0

    for j in range(time):
        if dct[lst[j]] == 1:
            count_max += 1
            flag_min = 0
            break

    if cur_max < count_max:
        time_max = time + 1
        cur_max = count_max

    if flag_min:
        if time_min == 0:
            time_min = time

flag_empty = 1
for i in lst:
    if dct[i] == 1:
        flag_empty = 0

print("True" * flag_empty + "False" * (1 - flag_empty), end=" ")
print(time_min, end=" ")
print(time_max)
'''
#--------------------------------------------------------------
'''
lst = input().split()
dct = {i: 0 for i in lst}
cur_max = 0

for i in lst:
    dct[i] += 1

for i in lst:
    cur_max = max(cur_max, dct[i])

print(cur_max / lst.__len__())
'''
#--------------------------------------------------------------
'''str1 = input()
str2 = input()
s1 = set()
s2 = set()

for i in str2:
    if i != ' ':
        s2.add(i)

for i in str1:
    if i in s2:
        continue
    print(i, end="")
'''