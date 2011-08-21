#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright 2011 Hunter Lang

# MIT License

import logging
import json
import re
import sys
import uuid
import os.path
import tornado.httpserver
import tornado.httpclient
import tornado.ioloop
import tornado.options
import tornado.web
import tornado.template as template
from tornado.options import define, options
from pygments import highlight
from pygments.lexers import *
from pygments.formatters import HtmlFormatter
from pygments.styles import get_style_by_name
from pygments.util import ClassNotFound
from pymongo import Connection
from pymongo.objectid import ObjectId
from pymongo.errors import ConfigurationError
from pymongo import ASCENDING, DESCENDING

define("port", default=8888, help="run on the given port", type=int)

class Application(tornado.web.Application):
    def __init__(self):
        # Using the \w "word-match" regex.
        # The - is because the wordnik words
        # can have dashes in them.
        handlers = [
            (r"/newlist", NewListHandler),
            (r"/countall", CountHandler),
            (r"/key/([\w-]+)/([\w-]+)", KeyHandler),
        ]
        settings = dict(
            static_path = os.path.join(os.path.dirname(__file__), "static"),
            debug=True
        )
        tornado.web.Application.__init__(self, handlers, autoescape=None, **settings) 

class CountHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous
    def get(self):
        data = []
        for x in lists.find(): data.append([x["name"],lists.find({"name":x['name']}).count()])
        self.finish(json.dumps({"data":data}))
        

class NewListHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous
    def post(self):
        name = self.get_argument("name", None)
        if len([x for x in lists.find({"name": name})]) == 0:
            lists.insert({"name": name, "keys":[]})
            self.finish("OK")
        else: 
            self.finish(json.dumps(lists.find_one({"name":name})['keys']))

class KeyHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous
    def get(self, list, key):
        finish = ""
        
        results = lists.find_one({"name": list})
        
        # if the list doesnt exist
        if not results:
            lists.insert({"name": list, "keys":[]})
            finish = "NEW_LIST"
        
        # if the list and key exist
        elif key in results['keys']:
            print key
            finish = "UNDER_CONSTRUCTION"
            
        # list exists but key wasnt found
        elif key.lower() == "delete":
            lists.remove({"name":list})
            finish = "DELETED"
        else:
            finish = "NOT_FOUND"
        self.finish(finish)

def main():
    # enables headers so it can be run behind nginx
    http_server = tornado.httpserver.HTTPServer(Application(), xheaders=True) 
    http_server.listen(options.port)
    logging.info("Serving on port %s" % options.port)
    tornado.ioloop.IOLoop.instance().start()

if __name__ == "__main__":
    tornado.options.parse_command_line()
    connection = Connection('localhost', 27017)
    db = connection['blam'] # ~= database name
    lists = db['lists'] # ~= database table
    logging.info("Connected to database")
    main()
