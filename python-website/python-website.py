from flask import Flask, request, redirect, send_from_directory, json, jsonify, url_for
from time import sleep

app = Flask(__name__)
app.config['JSONIFY_PRETTYPRINT_REGULAR'] = True

@app.route("/")
def index_file():
    return redirect(url_for('html_index_file'))

@app.route("/html")
def html_index_file():
    return send_from_directory('html', 'index.html')

@app.route('/html/<path:path>')
def html_file(path):
    return send_from_directory('html', path)

def request_simplify(request):
    headers = { }
    for name,value in request.headers.items():
        headers[name] = value
    args = { }
    for name,value in request.args.items():
        args[name] = value
    form = { }
    for name,value in request.form.items():
        form[name] = value
    files = { }
    for name,fileStorage in request.files.items():
        value = fileStorage.read().decode('utf-8')
        files[name] = value
    return {
        'method': request.method,
        'headers': headers,
        'args': args,
        'form' : form,
        'files' : files,
        }

@app.route('/api/debug', methods = [ 'GET', 'POST' ] )
def api_debug():
    sleep(0.200)
    return jsonify(request_simplify(request))

@app.route('/api/debug301', methods = [ 'GET', 'POST'] )
def api_debug301():
    sleep(0.200)
    return redirect(url_for('api_debug', **request.args), code=301)

@app.route('/api/debug302', methods = [ 'GET', 'POST'] )
def api_debug302():
    sleep(0.200)
    return redirect(url_for('api_debug', **request.args), code=302)

@app.route('/api/debug303', methods = [ 'GET', 'POST'] )
def api_debug303():
    sleep(0.200)
    return redirect(url_for('api_debug', **request.args), code=303)

@app.route('/api/debug305', methods = [ 'GET', 'POST'] )
def api_debug305():
    sleep(0.200)
    return redirect(url_for('api_debug', **request.args), code=305)

@app.route('/api/debug307', methods = [ 'GET', 'POST'] )
def api_debug307():
    sleep(0.200)
    return redirect(url_for('api_debug', **request.args), code=307)

@app.route('/api/debug308', methods = [ 'GET', 'POST'] )
def api_debug308():
    sleep(0.200)
    return redirect(url_for('api_debug', **request.args), code=308)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=1234)
