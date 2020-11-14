from sys import argv, exit
import cs50
import csv


# Checks for correct input
if len(argv) != 2:

    print("Usage: python import.py file.csv")

    exit(1)


# Connects with the SQL database
db = cs50.SQL("sqlite:///students.db")


# Reads the .csv file
with open(argv[1], "r") as database:

    db_dict = csv.DictReader(database)

    # Iterates through the .csv file to insert the data into the database
    for row in db_dict:

        name = row["name"].split()

        # Checks if there is a middle name
        if len(name) == 2:

            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       name[0], None, name[1], row["house"], row["birth"])

        elif len(name) == 3:

            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       name[0], name[1], name[2], row["house"], row["birth"])