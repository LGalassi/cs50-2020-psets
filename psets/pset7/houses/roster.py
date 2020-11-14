from sys import argv, exit
import cs50


# Checks for correct input
if len(argv) != 2:

    print("Usage: python roster.py HOUSE")

    exit(1)


# Connects with the SQL database
db = cs50.SQL("sqlite:///students.db")


# Execute the query to obtain the data
house = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])


# Iterates through the data obtained to print the result
for row in house:

    # Checks if there is a middle name
    if row["middle"] == None:

        print(f"{row['first']} {row['last']}, born {row['birth']}")

    else:

        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")