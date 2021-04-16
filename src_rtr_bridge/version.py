#!/usr/bin/python3
def get_ver() :
	fd = open("version.cfg","r");
	i=0;
	str2 = "";
	while True:
		str = fd.readline();
		i += 1;
		str = str.replace("\n","",1);
		str = str.replace("\r","",1);
		str2 += str[str.find("=")+1:];
		if(i>=1) :
			str2 += "."
		# print(str);
		if (str is None) :
			break;
		elif (i == 3):
			break;
	fd.close();
	return str2;

print(get_ver());