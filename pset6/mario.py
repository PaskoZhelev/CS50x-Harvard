import cs50

def main():
    n = 0
    while True:
        print("Height: ", end = "")
        n = cs50.get_int()
        if(n >= 0 and n <= 23):
            break
        
    for i in range(n, 0 , -1):
        for a in range (i - 1):
            print(" ", end = "")
            
        for b in range(i, n + 1):
            print("#", end = "")
        print("#")
        
if __name__ == "__main__":
    main()
            