from time import time,ctime
import nodejs

def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c

print('Today is', ctime(time()))
print('result of node call', nodejs.call('nodeMultiplication', '42', 21))


