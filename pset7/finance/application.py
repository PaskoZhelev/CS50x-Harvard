from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
import time

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

@app.route("/add-cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Show history of transactions."""
    if request.method == "POST":
        id = session["user_id"]
        if not request.form.get("added-cash").isdigit():
           return apology("Invalid Number!")
        
        cashToBeAdded = int(request.form.get("added-cash"))
        if(cashToBeAdded <= 0):
           return apology("Invalid Number!") 
        
        user = db.execute("SELECT * FROM users WHERE id = :id", id=id)
        cash = user[0]["cash"]
        cash += cashToBeAdded
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash, id = id)
        return redirect(url_for("index"))
    else:
        return render_template("add-cash.html")
    

@app.route("/")
@login_required
def index():
    
    id = session["user_id"]
    user_account = db.execute("SELECT * FROM users WHERE id = :id", id = id)
    user_portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id", user_id = id)
    cash = user_account[0]["cash"]
    
    totalStockValue = 0
    for stock in user_portfolio:
        totalStockValue += float(stock["total"])
    totalGrandSum = totalStockValue + cash
    
    return render_template("index.html", totalGrandSum = round(totalGrandSum, 2), portfolio = user_portfolio, cash = round(cash, 2))
     
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        # check for empty fields
        if not request.form.get("symbol"):
            return apology("A symbol must be provided!")
        elif not request.form.get("shares") or not request.form.get("shares").isdigit():
            return apology("A number of shares to be bought must be provided!")
        
        # check if symbol of the stock exists    
        quote = lookup(request.form.get("symbol"))
        if quote == None:
           return apology("Invalid Symbol!")
        
        # get user's current data  
        shares = int(request.form.get("shares"))
        companyName = quote["name"]
        symbol = quote["symbol"]
        price = quote["price"]
        id = session["user_id"]
        date = time.strftime("%H:%M:%S %d/%m/%Y")
        
        # get user's query
        user = db.execute("SELECT * FROM users WHERE id = :id", id = id)
        cash = user[0]["cash"]
        totalPrice = shares*price
        if cash - totalPrice < 0:
           return apology("Insufficient Funds!")
        
        cash -= totalPrice
        # check if user has already bought stocks from this company   
        portfolioRow = db.execute("SELECT * FROM portfolio WHERE company = :company AND user_id = :user_id", company = companyName, user_id = id)
        if len(portfolioRow) == 0:
            db.execute("INSERT INTO portfolio (company, share_price, quantity, user_id, date, total, symbol) VALUES(:company, :share_price, :quantity, :user_id, :date, :total, :symbol)", company=companyName, share_price=price, quantity = shares, user_id=id, date = date, total = totalPrice, symbol = symbol)
        else:
            # get current number of shares and add the quantity that is planned to be bought
            num_shares = portfolioRow[0]["quantity"] + int(shares)
            totalPrice = num_shares*price
            # update number of shares and total price
            db.execute("UPDATE portfolio SET quantity = :quantity WHERE company = :company AND user_id = :user_id", quantity = num_shares, company = companyName, user_id = id)
            db.execute("UPDATE portfolio SET total = :total WHERE company = :company AND user_id = :user_id", total = totalPrice, company = companyName, user_id = id)
        # update user's cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash, id = id)
        
        # update the history table with the latest buy
        db.execute("INSERT INTO history (symbol,shares,date,user_id) VALUES (:symbol,:shares,:date,:user_id)",symbol=symbol, shares = shares,date=date, user_id = id)
        
        return redirect(url_for("index"))
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    id = session["user_id"]
    history = db.execute("SELECT * FROM history JOIN portfolio ON history.symbol = portfolio.symbol WHERE history.user_id = :user_id", user_id=id)
    
    return render_template("history.html", history = history)

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
    if request.method == "POST":
        # check for empty fields
        if not request.form.get("symbol"):
            return apology("A symbol must be provided!")
        elif not request.form.get("shares") or not request.form.get("shares").isdigit():
            return apology("A number of shares to be sold must be provided!")
        
        # check if symbol of the stock exists    
        quote = lookup(request.form.get("symbol"))
        if quote == None:
           return apology("Invalid Symbol!")
        
        id = session["user_id"]
        symbol = quote["symbol"]
        
        # check if the user actually owns these stocks
        stocks = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id AND symbol = :symbol", user_id = id, symbol = symbol)
        
        if len(stocks) == 0:
           return apology("You don't own this stock!")
           
        # get user's current data  
        shares = int(request.form.get("shares"))
        companyName = quote["name"]
        price = quote["price"]
        quantity = stocks[0]["quantity"]
        date = time.strftime("%H:%M:%S %d/%m/%Y")
        
        if(quantity - shares < 0):
           return apology("You don't have so many stocks!")
        
        moneyToGet = shares * price
        quantity -= shares
        total = price * quantity
        # update the quantity and total sum
        db.execute("UPDATE portfolio SET quantity = :quantity WHERE company = :company AND user_id = :user_id", quantity = quantity, company = companyName, user_id = id)
        db.execute("UPDATE portfolio SET total = :total WHERE company = :company AND user_id = :user_id", total = total, company = companyName, user_id = id)
        
        user = db.execute("SELECT * FROM users WHERE id = :id", id = id)
        cash = user[0]["cash"]
        cash += moneyToGet
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash, id = id)
        if (quantity == 0):
           db.execute("DELETE FROM portfolio WHERE symbol = :symbol", symbol = symbol)
        # update the history table with the latest sell
        db.execute("INSERT INTO history (symbol,shares,date,user_id) VALUES (:symbol,:shares,:date,:user_id)",symbol=symbol, shares = -shares,date=date, user_id = id)
        
        return redirect(url_for("index"))
    else:
        return render_template("sell.html")
