import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query for shares owned
    rows = db.execute("SELECT symbol, SUM(quantity) FROM operations WHERE user_id = :id GROUP BY 1 ORDER BY 1", id=session["user_id"])

    # Query for total cash
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

    # Update info about the stocks
    for row in rows:
        temp = lookup(row["symbol"])
        row["name"] = temp["name"]
        row["total"] = usd(row["SUM(quantity)"] * temp["price"])
        row["price"] = usd(temp["price"])

    # Render index page
    return render_template("index.html", rows=rows, cash=usd(cash[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Handle lack of input
        if not request.form.get("symbol"):
            return apology("enter a stock symbol")

        if not request.form.get("shares"):
            return apology("enter the number of shares")


        # Organize the variables
        info = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))
        total = info["price"] * shares
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # Handle insufficient funds
        if total > cash[0]["cash"]:
            return apology("insufficient funds")

        # Uptade the operations table
        db.execute("INSERT INTO operations (user_id, quantity, symbol, price) VALUES (:id, :shares, :symbol, :price)", id=session["user_id"], shares=shares, symbol=info["symbol"], price=info["price"])

        # Update total amount of cash
        db.execute("UPDATE users SET cash = :new WHERE id = :id", new=cash[0]["cash"] - total, id=session["user_id"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query for the history
    rows = db.execute("SELECT * FROM operations WHERE user_id = :id", id=session["user_id"])

    # Update info about the stocks
    for row in rows:
        temp = lookup(row["symbol"])
        row["name"] = temp["name"]
        row["price"] = usd(row["price"])

        if row["quantity"] > 0:
            row["type"] = "Buy"
        else:
            row["type"] = "Sell"
            row["quantity"] *= -1

    # Render the history page
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # Handle lack of input
        if not request.form.get("symbol"):
            return apology("you must enter a stock symbol")

        # Lookup for info about the stock
        quote = lookup(request.form.get("symbol"))

        # Handle inexistent stock symbol
        if quote == None:
            return apology("invalid stock symbol")

        # Organize the variables
        symbol = quote["symbol"]
        name = quote["name"]
        price = usd(quote["price"])

        # Render the quoted page
        return render_template("quoted.html", symbol=symbol, name=name, price=price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Handle lack of input
        if not request.form.get("username"):
            return apology("you must provide a username")

        if not request.form.get("password") or not request.form.get("confirmation"):
            return apology("you must provide a password")

        # Check if the username already exist
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        if len(rows) != 0:
            return apology("that username is already taken")

        # Check if the passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("the passwords don't match")

        # Register the user in the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        # Redirect to the main page
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # Handle lack of input
        if not request.form.get("symbol"):
            return apology("you must choose a stock to sell")

        if not request.form.get("shares"):
            return apology("you must provide the number of shares")

        # Query for stocks in possession
        rows = db.execute("SELECT symbol, SUM(quantity) AS sum FROM operations WHERE user_id = :id AND symbol = :symbol GROUP BY 1", id=session["user_id"], symbol=request.form.get("symbol"))

        # Check if there is enough shares to sell
        if int(request.form.get("shares")) > rows[0]["sum"]:
            return apology("you don't have that many shares to sell")

        # Organize the variables
        info = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))
        total = info["price"] * shares
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # Update the operations table
        db.execute("INSERT INTO operations (user_id, quantity, symbol, price) VALUES (:id, :shares, :symbol, :price)", id=session["user_id"], shares=(shares * -1), symbol=info["symbol"], price=info["price"])

        # Update total amount of cash
        db.execute("UPDATE users SET cash = :new WHERE id = :id", new=cash[0]["cash"] + total, id=session["user_id"])

        return redirect("/")
    else:

        # Query for the stocks in possession to display
        rows = db.execute("WITH temp AS (SELECT symbol, SUM(quantity) AS sum FROM operations WHERE user_id = :id GROUP BY 1 ORDER BY 1) SELECT * FROM temp WHERE sum > 0", id=session["user_id"])

        return render_template("sell.html", rows=rows)


@app.route("/performance")
@login_required
def performance():

    # Query for the shares and average price of each stock
    rows = db.execute("WITH temp AS (SELECT symbol, SUM(quantity) AS sum, SUM(quantity * price) AS cost FROM operations WHERE user_id = :id GROUP BY symbol) SELECT symbol, ROUND(cost * 1.0 / sum, 2) AS avg FROM temp", id=session["user_id"])

    # Uptade, calculate and format the info about each stock
    for row in rows:
        temp = lookup(row["symbol"])

        row["name"] = temp["name"]

        row["now"] = temp["price"]

        row["formated_now"] = usd(temp["price"])

        row["formated_avg"] = usd(row["avg"])

        # Calculate the current variation of the stock price
        row["percent"] = (row["now"] - row["avg"]) / row["avg"]

        row["formated_percent"] = "{:.2%}".format(row["percent"])

    # Render the performance page
    return render_template("performance.html", rows=rows)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
