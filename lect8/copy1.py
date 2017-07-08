import cs50

print("s: ", end = "")
s = cs50.get_string()
if s == None:
    exit(1)

t = s.capitalize()
print(s)
print(t)

exit(0)