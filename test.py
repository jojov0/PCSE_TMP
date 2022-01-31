import random
import time
while(True):
    s = ""
    for x in range(33):
        if x < 32:
            s += str(random.randint(0,60)) + ","
        else:
            s += str(random.randint(0,60))
    print(s)
