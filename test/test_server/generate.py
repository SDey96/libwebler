def getUrl(n):
	s = ""
	for i in range(n-1):
		s += '<p' + str((i+1)) + '>/'
	s += '<p' + str(n) + '>'
	return s

def getList(n):
	s = ""
	for i in range(n-1):
		s += 'p' + str((i+1)) + ','
	s += 'p' + str(n)
	return s


for i in range(10):
	print """
@app.route("/intermediate/{url}/")
def p{x}_link({l}):
    return render_template('intermediate_page.html',port=port,prefix=getPrefix([{l}]))

	""".format(x=i,url=getUrl(i+1),l=getList(i+1))