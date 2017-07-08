from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("A symbol must be provided!")
        elif not request.form.get("shares") or not request.form.get("shares").isdigit():
            return apology("A number of shares to be bought must be provided!")
            
        quote = lookup(request.form.get("symbol"))
        if quote == None:
           return apology("Invalid Symbol!")
          
        shares = int(request.form.get("shares"))
        companyName = quote["name"]
        symbol = quote["symbol"]
        price = quote["price"]
        id = session["user_id"]
        
        user = db.execute("SELECT * FROM users WHERE id = :id", id)
        cash = int(user[0]["cash"])
        totalPrice = shares*price
        if cash - totalPrice < 0:
           return apology("Insufficient Funds!")
           
        portfolioRow = db.execute("SELECT * FROM portfolio WHERE company = :company", companyName)
        if len(portfolioRow) != 0:
            db.execute("INSERT INTO portfolio (company, share_price, quantity, user_id, date) VALUES(:company, :share_price, :quantity, :user_id, :date)", company, price, shares, id, date)
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not  request.form.get("symbol"):
            return apology("A symbol must be provided!")
        
        quote = lookup(request.form.get("symbol"))
        if quote == None:
           return apology("Invalid Symbol!")
        else:
            name = quote["name"]
            symbol = quote["symbol"]
            price = quote["price"]
            return render_template("quoted.html", name = name, symbol = symbol, price = price)
    else:
        return render_template("quote.html")
    

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Missing username!")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("Missing password!")
            
        elif not (request.form.get("password") == request.form.get("confirmation")):
            return apology("Passwords do not match!")
        
        hash = pwd_context.encrypt(request.form.get("password"))
        success = db.execute("INSERT INTO users (username,hash) VALUES (:username,:hash)",username=request.form.get("username"),hash=hash)
        
        if not success:
            return apology("Username is already taken!")
        
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = rows[0]["id"]
        
        return redirect(url_for("index"))
    else:       
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")
