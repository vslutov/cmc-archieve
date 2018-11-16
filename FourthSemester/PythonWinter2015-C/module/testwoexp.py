import woexp

exp = woexp.WordExp("[p-t]*.*")
print exp.result()
exp.expand("*.NONEXIST")
print exp.result()
exp.expand("$PWD")
print exp.result()
# TODO fix memoryleak here:
for f in 0, woexp.WRDE_UNDEF:
    try:
        exp.expand("$NONEXIST", f)
        print exp.result()
    except MemoryError as ex:
        print ex.args[0], exp.result()
