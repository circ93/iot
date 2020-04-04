from flask import Flask, render_template
from class_search import SearchJson

app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/lasthour')
def lasthour():
    return render_template('lasthour.html')


@app.route('/datalasthour')
def data():
    data = SearchJson(3600)
    listJson = data.searchFileInterval()
    dataJson = data.parseValue(listJson)
    return dataJson


@app.route('/datalivedata')
def live():
    data = SearchJson(10)
    listJson = data.searchFileInterval()
    dataJson = data.parseValue(listJson)
    return dataJson


if __name__ == '__main__':
    app.run(debug=True)
