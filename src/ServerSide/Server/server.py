from ctypes import *
from http.server import BaseHTTPRequestHandler, HTTPServer
from time import sleep

libc = CDLL("server.so")

libc.MakeShop.restype = c_void_p

libc.DeleteShop.argtypes = [c_void_p]

libc.AddItemToShop.argtypes = [c_void_p, c_ulonglong]

libc.DeleteItemFromShop.argtypes = [c_void_p, c_ulonglong]

libc.ShopOrderIsEmpty.argtypes = [c_void_p]
libc.ShopOrderIsEmpty.restype = c_int

libc.BuyOrderToString.argtypes = [c_void_p]
libc.BuyOrderToString.restype = c_char_p

libc.DBUpdateItem.argtypes = [c_ulonglong, c_int]

libc.DBUpdateBox.argtypes = [c_ulonglong, c_int]

libc.DBInsertItem.argtypes = [c_char_p, c_ulonglong, c_ulonglong, c_ulonglong, c_char_p]
libc.DBInsertItem.restype = c_ulonglong

libc.DBInsertBox.argtypes = [c_char_p, c_ulonglong, c_ulonglong, c_ulonglong, c_char_p]
libc.DBInsertBox.restype = c_ulonglong

libc.DBGetItems.restype = c_char_p

libc.DBGetBoxes.restype = c_char_p

libc.DBGetOrders.restype = c_char_p


print('Start')
libc.OpenDataBase()
print('Opened db')
shop = libc.MakeShop()
print('made shop')

sleep(1)

print('Ready!')


class HandleRequests(BaseHTTPRequestHandler):

    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'plain/text')
        self.end_headers()

    def do_GET(self):
        path = str(self.path)
        args = path.split('/')[1:]
        qtype = args[0]

        global shop

        content = 'ok'
    
        if qtype == 'add_item':
            itemID = int(args[1])
            libc.AddItemToShop(shop, itemID)
        elif qtype == 'delete_item':
            itemID = int(args[1])
            libc.DeleteItemFromShop(shop, itemID)
        elif qtype == 'order_is_empty':
            orderIsEmpty = libc.ShopOrderIsEmpty(shop)
            content = str(orderIsEmpty)
        elif qtype == 'buy':
            content = libc.BuyOrderToString(shop).decode('utf-8')
        elif qtype == 'update_item':
            itemID = int(args[1])
            amount = int(args[2])
            libc.DBUpdateItem(itemID, amount)
        elif qtype == 'update_box':
            boxID = int(args[1])
            amount = int(args[2])
            libc.DBUpdateBox(boxID, amount)
        elif qtype == 'insert_item':
            itemName = args[1]
            weight = int(args[2])
            volume = int(args[3])
            cost = int(args[4])
            image = args[5]
            content = str(libc.DBInsertItem(itemName.encode('utf-8'), weight, volume, cost, image.encode('utf-8')))
        elif qtype == 'insert_box':
            boxName = args[1]
            maxWeight = int(args[2])
            maxVolume = int(args[3])
            cost = int(args[4])
            image = args[5]
            content = str(libc.DBInsertBox(boxName.encode('utf-8'), maxWeight, maxVolume, cost, image.encode('utf-8')))
        elif qtype == 'get_items':
            print(libc.DBGetItems())
            content = libc.DBGetItems().decode('utf-8')
        elif qtype == 'get_boxes':
            content = libc.DBGetBoxes().decode('utf-8')
        elif qtype == 'save_order':
            libc.DBSaveOrder(path.encode('utf-8'))
        elif qtype == 'get_orders':
            content = libc.DBGetOrders().decode('utf-8')
        else:
            content = 'undefined query'
        #print('Response content: ', content)

        self._set_headers()
        self.wfile.write(content.encode('utf-8'))

    def do_POST(self):
        '''Reads post request body'''
        self._set_headers()
        content_len = int(self.headers.getheader('content-length', 0))
        post_body = self.rfile.read(content_len)
        self.wfile.write("received post request:<br>{}".format(post_body))

    def do_PUT(self):
        self.do_POST()



HTTPServer(('localhost', 8080), HandleRequests).serve_forever()

# usage: http://localhost:8080/todo/23

