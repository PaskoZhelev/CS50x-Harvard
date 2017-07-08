import cs50

c = cs50.get_char()

if c == "Y" or "y":
    print("yes")
elif c == "N" or "n":
    print("no")
else:
    print("error")
    