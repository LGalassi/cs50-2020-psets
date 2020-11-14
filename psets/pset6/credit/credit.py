from cs50 import get_string
import math

# Prompts the user for the credit card number
card = get_string("Number: ")

# Calculates the number of digits
n = len(card)
csa = []

# Multiplies every other digit by 2
for i in range(n - 2, -1, -2):
    csa.append(int(card[i]) * 2)

# Calculates the sum of all the digits
for i in range(len(csa)):
    if int(csa[i]) >= 10:
        csa[i] = (csa[i] % 10) + (math.floor(csa[i] / 10))

a = sum(csa)

csb = []

# Calculates the sum of the rest of the digits
for i in range(n - 1, -1, -2):
    csb.append(int(card[i]))

b = sum(csb)

cs = a + b

# Checks if the checksu is valid
if cs % 10 == 0:
    # Checks for AMEX cards
    if n == 15:
        if card[0] == '3':
            if card[1] == 4 or card[1] == '7':
                print("AMEX")
            else:
                print("INVALID")
        else:
            print("INVALID")
    # Checks for VISA cards
    elif n == 13:
        if card[0] == '4':
            print("VISA")
        else:
            print("INVALID")
    # Checks for VISA or MASTERCARD cards
    elif n == 16:
        if card[0] == '4':
            print("VISA")
        elif card[0] == '5':
            if int(card[1]) >= 1 and int(card[1]) <= 5:
                print("MASTERCARD")
            else:
                print("INVALID")
        else:
            print("INVALID")
    else:
        print("INVALID")
else:
    print("INVALID")
