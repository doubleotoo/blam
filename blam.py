#!/usr/bin/env python
import os
import optparse
import sys
from termcolor import colored
from pymongo import Connection
from pymongo import ASCENDING, DESCENDING
from pymongo.objectid import ObjectId
from pymongo.errors import ConfigurationError
import httplib
import json
import urllib2
import webbrowser


api_root = None
copy_command = "pbcopy"
home = os.getenv("USERPROFILE") or os.getenv("HOME")
config_options = {}
stored_json = {}

def new_list(title):
#    new_list = {"" + title:[]}
    stored_json['lists'][''+title] = []
    print "Created a new list called", colored(title, 'yellow')
    save()

def save():
    open(home + "/.blam", 'w').write(json.dumps({"options": config_options}) + """\n-----\n""" + json.dumps(stored_json))

def api_post(url, send_data):
    h = httplib.HTTPConnection(api_root)
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}    
    h.request('POST', url, json.dumps(send_data), headers)
    r = h.getresponse()
    return r.read()

def api_get(url):
    return urllib2.urlopen(api_root + url).read()

def parse_config():
    if sys.platform == "darwin": copy_command = "pbcopy"
    else: copy_command = "xsel -c"

    x = open(home + '/.blam', 'r')
    split = x.read().split("""-----\n""")
    x.close()
    options = json.loads(split[0])["options"]
    try:
        api_root = options["update_server"]
    except: print "Error in config. Run blam --reset."
    for key in options.keys():
        config_options['' + key] = options[key]
        json_load = json.loads(split[1])
    for key in json_load.keys():
        stored_json['' + key] = json_load[key]
def update_server(self):
    username = config_options["username"]
    password = config_options["password"]
    url = config_options["update_server"]
    
    data = {
        "JSON":stored_json,
        "username":username,
        "password":password
    }
    api_post("/", data)

def main():

    if len(sys.argv) == 1:
        # print lists and their lengths
        for list in stored_json['lists'].keys():
            print list + " (" + str(len(stored_json['lists']['' + list])) + ")"

    elif len(sys.argv) == 2:
        # blam <list>
        # A new list could be created here, 
        # or it could be a copy request. 
        # if arg is not a list and there is no key matching it, create a new list.
        # if arg is not a list and there is a key matching it, ignore the list creation and copy the value.
        # if arg is a list and there is a key matching it, list all keys in the list.
        # if arg is a list and there are no keys matching it, list all keys in the list.
        copy_value = None
        for list in stored_json['lists']:
            for item in stored_json['lists']['' + list]:
                item = item.items()
                if item[0][0] == sys.argv[1]:
                    copy_value = item[0][1]
                    print colored("Blam!", "cyan"), "We just copied", colored(copy_value, 'yellow'), "to your clipboard."
        try: os.popen("echo " + copy_value + " | " + copy_command)
        except TypeError: a = 1

        if sys.argv[1] in stored_json['lists'].keys() and not copy_value:
            for item in stored_json['lists']['' + sys.argv[1]]:
                items = item.items()
                print items[0][0] + ":" + "     " + items[0][1]
        # creates a new list
        elif not copy_value:
            new_list(sys.argv[1])

    elif len(sys.argv) == 3:
        # blam <list> <key>
        # if list exists and key doesnt, print key not found in list.
        # if key exists and list doesn't, create a new list and ignore the key.
        # if both exist, copy the value for the key.

        if sys.argv[1] in stored_json['lists'].keys():
            copy_value = None
            # list exists.
            list = stored_json['lists']['' + sys.argv[1]]
            for x in list:
                for y in x.items():
                    if sys.argv[2] == y[0]:
                        copy_value = y[1]
                        print colored("Blam!", 'cyan'), "We just copied", colored(copy_value, 'yellow'), "to your clipboard."
                        break
            if copy_value:
                os.popen("echo " + copy_value + " | " + copy_command)

            elif not sys.argv[1] in stored_json['lists'].keys():
                # list doesnt exit
                new_list(sys.argv[1])
            else: print colored(sys.argv[2], 'yellow'), colored('not found in', 'red'), colored(sys.argv[1], 'yellow')

    elif len(sys.argv) == 4:
        list = sys.argv[1]
        key = sys.argv[2]
        value = sys.argv[3]
        if not list in [list['' + list.keys()[0]] for list in stored_json['lists']]:
            # list doesnt exist yet
            stored_json['lists'].append({'' + list:[key, value]})
#        elif not key in stored_json['lists']
        os.popen("echo " + "OHAI" + " | " + copy_command)
    save()

if __name__ == "__main__":
#    try:
#        if sys.argv[1] == "--reset":
#            x = open(os.getenv("USERPROFILE") or os.getenv("HOME") + "/.blam", "w")
#            x.write("""{"options":{"username":"hunter", "password": "220802", "update_server": "http://localhost:8080"}}\n-----\n{"lists":[]}""")
#            exit()
#    except IndexError:
#        print "error."
    parse_config()
    main()
