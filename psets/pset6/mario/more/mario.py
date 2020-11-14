from cs50 import get_int

# Prompts the user for the height
while True:
    h = get_int("Height: ")

    if h > 0 and h <= 8:
        break

# Prints the pyramid
for i in range(h):
    # Prints the left spaces
    for s in range(h - (i + 1)):
        print(" ", end="")

    # Prints the left hashes
    for p in range(i + 1):
        print("#", end="")

    # Prints the gap spaces
    print("  ", end="")

    # Prints the right spaces
    for p in range(i + 1):
        print("#", end="")

    # Changes the line
    print()