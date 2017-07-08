import cs50

def main():
    owingChangeFloat = 0
    print("O hai! ", end="")
    while True:
        print("How much change is owed?")
        owingChangeFloat = cs50.get_float();
        if (owingChangeFloat >= 0):
            break
        
    owingChangeFloat *= 100.0
    owingChangeInt = int(round(owingChangeFloat))
    
    print("{}".format(coinsNeeded(owingChangeInt)))
    
def coinsNeeded(money):
    neededCoins = 0
    while(money > 0):
        if(money >= 25):
            money -= 25
            neededCoins += 1
        elif(money >= 10):
            money -= 10
            neededCoins += 1
        elif(money >= 5):
            money -= 5
            neededCoins += 1
        elif(money >= 1):
            money -= 1
            neededCoins += 1
        else:
            break
    return neededCoins
            

if __name__ == "__main__":
    main()