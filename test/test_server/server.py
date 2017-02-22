from flask import Flask,request,jsonify,render_template,request,jsonify,make_response,redirect

app = Flask(__name__)

port = 3000

def getPrefix(arr):
    s = ''
    for a in arr[:-1]:
        s += a + '/'
    s += arr[-1]
    return s

@app.route("/first/")
def first_link():
    return render_template('first_page.html',port=port)

@app.route("/intermediate/<p1>/")
def p0_link(p1):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1]))

@app.route("/intermediate/<p1>/<p2>/")
def p1_link(p1,p2):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2]))

@app.route("/intermediate/<p1>/<p2>/<p3>/")
def p2_link(p1,p2,p3):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/")
def p3_link(p1,p2,p3,p4):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/<p5>/")
def p4_link(p1,p2,p3,p4,p5):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4,p5]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/<p5>/<p6>/")
def p5_link(p1,p2,p3,p4,p5,p6):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4,p5,p6]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/<p5>/<p6>/<p7>/")
def p6_link(p1,p2,p3,p4,p5,p6,p7):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4,p5,p6,p7]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/<p5>/<p6>/<p7>/<p8>/")
def p7_link(p1,p2,p3,p4,p5,p6,p7,p8):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4,p5,p6,p7,p8]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/<p5>/<p6>/<p7>/<p8>/<p9>/")
def p8_link(p1,p2,p3,p4,p5,p6,p7,p8,p9):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4,p5,p6,p7,p8,p9]))

@app.route("/intermediate/<p1>/<p2>/<p3>/<p4>/<p5>/<p6>/<p7>/<p8>/<p9>/<p10>/")
def p9_link(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10):
    return render_template('final_page.html',port=port,prefix=getPrefix([p1,p2,p3,p4,p5,p6,p7,p8,p9,p10]))

if __name__ == "__main__":
	app.run(port=port,debug=True)