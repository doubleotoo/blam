#!/usr/bin/env python
import os
import optparse
import sys
from termcolor import colored
from pymongo import Connection
from pymongo import ASCENDING, DESCENDING
from pymongo.objectid import ObjectId
from pymongo.errors import ConfigurationError
import urllib
import httplib
import json
import urllib2

api_root = "localhost:8080"

def api_post(url, send_data):
    data = urllib.urlencode(send_data)
    h = httplib.HTTPConnection(api_root)
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}    
    h.request('POST', url, data, headers)
    r = h.getresponse()
    return r.read()
def api_get(url):
     return urllib2.urlopen("http://" + api_root + url).read()
def main():
    if len(sys.argv) == 1:

        home = os.getenv("USERPROFILE") or os.getenv("HOME")
#        try: x = open(home.replace(" ", r'\ ') + "/.blam")
#        except: print  "Failed to open blam file. Try running blam --config"
        response = api_get("/countall")
        x = json.loads(response)['data']
        for a in x:
            print a[0] + " (" + str(a[1]) + ")"
    if len(sys.argv) == 2:
        # blam <list>
        # A new list could be created here, 
        # or it could be a copy request. 
        # if arg is not a list and there is no key matching it, create a new list.
        # if arg is not a list and there is a key matching it, ignore the list creation and copy the value.
        # if arg is a list and there is a key matching it, list all keys in the list.
        # if arg is a list and there are no keys matching it, list all keys in the list.
        
        response = api_post("/newlist", {"name": sys.argv[1]})
        if response == "OK": print "Created a new list called", colored(sys.argv[1], 'yellow') + "."
        else: 
            x = json.loads(response)
            if not len(x) is 0:
                print x
                
    elif len(sys.argv) == 3:
        
        # blam <list> <key>
        # if list exists and key doesnt, print key not found in list.
        # if key exists and list doesn't, create a new list and ignore the key.
        # if both exist, copy the value for the key.

        response = api_get("/key/" + sys.argv[1] + "/" + sys.argv[2])
        if response == "NOT_FOUND":
            print colored("ERROR:", "red"), colored(sys.argv[2], 'yellow'), colored("not found in", 'red'), colored(sys.argv[1], 'yellow')
        elif response == "NEW_LIST":
            print "Created a new list called", colored(sys.argv[1], 'yellow')
        elif response == "DELETED":
            print "Deleted all your " + colored(sys.argv[1], 'yellow')

    elif len(sys.argv) == 4:
        print "Options OK."

if __name__ == "__main__":
    main()
