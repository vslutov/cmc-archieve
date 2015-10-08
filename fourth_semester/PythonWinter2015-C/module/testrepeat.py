import repeat

print repeat.repeat("qwerty",1,4,5)

try:
    print repeat.repeat(1,4,5,6)
except Exception as ex:
    print ex

try:
    print repeat.repeat("qwerty",1,4,5,6)
except Exception as ex:
    print ex
