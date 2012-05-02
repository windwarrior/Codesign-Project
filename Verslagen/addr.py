str = "79:134:125:0:246"
strsplit = str.split(":")
for num in strsplit:
	print hex(int(num))

