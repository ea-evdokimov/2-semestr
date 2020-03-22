import random
with open("tests.cpp", "w") as fp:
    fp.write("#include <gtest/gtest.h> \n")
    fp.write('''#include "main.h" \n''')
    for i in range(50):
        fp.write("TEST(test_mult, test_number" + str(i+50)+") { \n")
        a = random.randint(1, 10**100)
        b = random.randint(1, 10**100)
        fp.write('''ASSERT_EQ(BigInteger("'''+str(a)+'''") * BigInteger("'''+str(b)+'''"),BigInteger("'''+str(a * b)+'''")); \n''')
        fp.write("} \n")
    for i in range(50):
        fp.write("TEST(test_mult, test_number"+ str(i+100)+") { \n")
        a = -random.randint(1, 1000000) * random.randint(1, 1000000) * \
            random.randint(1, 1000000) * random.randint(1, 1000000)
        b = random.randint(1, 1000000) * random.randint(1, 1000000) * random.randint(1, 1000000) * random.randint(1, 1000000)
        fp.write('''ASSERT_EQ(BigInteger("'''+str(a)+'''") * BigInteger("'''+str(b)+'''"), BigInteger("'''+str(a * b)+'''") ); \n''')
        fp.write("} \n")
    for i in range(50):
        fp.write("TEST(test_div, test_number"+ str(i)+") { \n")
        a = random.randint(1, 10000) * random.randint(1, 10000)
        b = random.randint(1, 10000) * random.randint(1, 10000) * \
            random.randint(1, 10000)
        if b == 0:
            b += 1
        fp.write('''ASSERT_EQ(BigInteger("'''+str(a)+'''") / BigInteger(
        "'''+str(b)+'''"), BigInteger("'''+str(a // b)+'''") ); \n''')
        fp.write("} \n")
    for i in range(50):
        fp.write("TEST(test_div, test_number" + str(i+50)+") { \n")
        a = random.randint(1, 10000) * random.randint(1, 10000) * \
            random.randint(1, 10000)
        b = random.randint(1, 10000) * random.randint(1, 10000) * \
            random.randint(1, 10000)
        if b == 0:
            b += 1
        fp.write('''ASSERT_EQ(BigInteger("'''+str(a)+'''") / BigInteger(
        "'''+str(b)+'''"),BigInteger("'''+str(a // b)+'''")); \n''')
        fp.write("} \n")
    for i in range(50):
        fp.write("TEST(test_div, test_number"+ str(i+100)+") { \n")
        a = random.randint(1, 1000000) * random.randint(1, 1000000) * \
            random.randint(1, 1000000) * random.randint(1, 1000000)
        b = random.randint(1, 1000000) * random.randint(1, 1000000) * random.randint(1, 1000000) * random.randint(1, 1000000)
        if b == 0:
            b += 1
        fp.write('''ASSERT_EQ(BigInteger("'''+str(a)+'''") / BigInteger(
        "'''+str(b)+'''"), BigInteger("'''+str(a // b)+'''") ); \n''')
        fp.write("} \n")
    for i in range(50):
        fp.write("TEST(test_mod, test_number"+ str(i)+") { \n")
        a = random.randint(1, 10000) * random.randint(1, 10000)
        b = random.randint(1, 10000) * random.randint(1, 10000) * \
            random.randint(1, 10000)
        if b == 0:
            b += 1
        fp.write('''ASSERT_EQ(BigInteger("'''+str(a)+'''") % BigInteger(
        "'''+str(b)+'''"), BigInteger("'''+str(a % b)+'''") ); \n''')
        fp.write("} \n")