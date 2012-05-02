str = "199:34:134:237:97"
strsplit = str.split(":")
for num in strsplit:
	print hex(int(num))

