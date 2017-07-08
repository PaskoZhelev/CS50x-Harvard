import cs50
import sys

def main():
   if(len(sys.argv) != 2):
       print("Usage: ./caesar k")
       exit(1)
   else:
       converted = int(sys.argv[1])
       key = 0
       if(converted < 0):
           print("Key must be a non-negative integer.")
           exit(1)
       elif(converted >= 26):
           key = converted - 26
       else:
           key = converted
       print("plaintext: ", end="")
       text = cs50.get_string()
       print("ciphertext: ", end="")
       for i in range(len(text)):
           if not (text[i].isalpha()):
               print("{}".format(text[i]), end="")
               continue
           alphabeticalIndex = ord(text[i].upper()) - 65
           newAlphabeticalIndex = int((alphabeticalIndex + key) % 26)
           asciiIndex = newAlphabeticalIndex + 65
           asciiLetter = chr(asciiIndex)
           if(text[i].isupper()):
               print("{}".format(asciiLetter.upper()), end="")
           else:
               print("{}".format(asciiLetter.lower()), end="")
       print("")
       

if __name__ == "__main__":
    main()