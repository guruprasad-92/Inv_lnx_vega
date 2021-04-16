#!/usr/bin/python3
import configparser as cp
import os
import sys

txt = ["PTH := $(shell find -type d -name ${NM} | head -1)\n"]
txt.append( "FLS := $(shell ls ${PTH}/*.c)\n" )
txt.append("OBJS += $(FLS:.c=.o)\n")
txt.append("INCLUDES += -I ${PTH}\n")

txt_r = txt;
# txt_r[1] = "FLS := $(shell find ${PTH} -type f -name \"*.c\")\n";

def get_dir() :
    prs = cp.ConfigParser();    
    prs.read(sys.argv[1]);
    sec = prs.sections()
    itm = prs.items(sec[0])
    dir = itm[0][1].split();
    sz = len(dir);
    for i in range(0,sz) :
        if (dir[i][-1] == "/") :
            dir[i] = dir[i][0:-1];
    return dir;

def get_nm(pth):
    sz = len(pth);
    nm = [];
    for i in range(0,sz) :
        nm.append(pth[i].split("/")[-2]);
    return nm;

def f_path(pth) :
    # print("type(pth) = {}".format(len(pth)));
    sz = len(pth);
    for i in range(sz) :
        if ( pth[i].find("/") == -1 ) :
            pth[i] += '/{}.mk'.format(pth[i]);
        else :
            end = pth[i].split("/")[-1];
            pth[i] += '/{}.mk'.format(end);
        # print(pth[i]);
    return pth

def f_wrtFl(pth) :
    sz = len(pth);
    fd = [];
    tmp = fd;
    for i in range(sz) :
        fd.append(open(pth[i], 'w'));
    return fd;

def wr_content(fd,insrt,txt) :
    sz1 = len(fd);
    sz2 = len(insrt);
    if ( sz1 == sz2 ) :
        for i in range(sz2) :
            if (i == 0) :
                txt.insert(0,"NM = {}\n".format(insrt[i]));
            else :
                txt.pop(0);
                txt.insert(0,"NM = {}\n".format(insrt[i]));
            fd[i].writelines(txt);
            fd[i].close();
        print("Created includes.mk");
    else :
        print("size did not match");
        for i in range(sz1) :
            fd[i].close();

# ------------------------------------------
n = len(sys.argv);
if(n < 2) :
    print("Err : file path is missing");
    print("Error : not enough arguments.");
    exit(0);    

dir = get_dir();
# print("dir = ",dir);
pth = f_path(dir);
nm = get_nm(pth);

# print("dir = ",dir);
# print("pth = ",pth);
# print("nm = ",nm);

fd = f_wrtFl(pth);
wr_content(fd,nm,txt);

# print(txt);